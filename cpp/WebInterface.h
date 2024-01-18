
/*
Webinterface.h
 */

#ifndef WEBINTERFACE_H
#define WEBINTERFACE_H

void startWebInterface(bool access_point, const char* sess_id=0, const char *sess_pass=0);
void stopWebInterface();
char bitsToHex(byte bits);

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include "StatusIndicator.h"

AsyncWebServer *server = 0;
bool isWebInterfaceActive() { return server; };

String urlencode(String in) {
  String ret;
  for (int i =0; i < in.length(); i++){
    char c = in.charAt(i);
    if (isalnum(c)){
      ret += c;
    } else{
      ret += '%';
      ret += bitsToHex(c >> 4);
      ret += bitsToHex(c & 0xf);
    }
  }
  return ret;
}

const char htmlhead[] = "<!DOCTYPE html><html><head><title>ToniPlayer WebInterface</title></head><body>";
const char htmlfoot[] = "</body></html>";

String backPage(const String &message) {
  return (htmlhead + message + "<p><a href=\"javascript:window.location = document.referrer;\">&laquo; Back</a></p>");
}

/** Remove file / dir, recursively */
void rmRf(const String &path) {
  File f = SD.open(path);
  if (!f) return;

  if (f.isDirectory()) {
    File e = f.openNextFile();
    while (e) {
      String p = e.name();
      e = f.openNextFile();
      rmRf(p);
    }
    f.close();
    SD.rmdir(path);
  } else {
    f.close();
    SD.remove(path);
  }
}

void startWebInterface(bool access_point, const char* sess_id, const char *sess_pass) {
  if (server) return;
  Serial.println("Starting WIFI");

  if (access_point) {
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig (IPAddress (192,168,4,1), IPAddress (0,0,0,0), IPAddress (255,255,255,0));
    WiFi.softAP("ToniPlayer", "");
  } else {
    WiFi.mode(WIFI_STA);
    if (sess_id) WiFi.begin(sess_id, sess_pass);
    else WiFi.begin();
  }

  Serial.println("Starting Webinterface");
  server = new AsyncWebServer(80);

  // On root ("/?path=XYZ") show a listing of files at the given path
  server->on("/", HTTP_GET, [] (AsyncWebServerRequest *request) {
    indicator.setTransientStatus(StatusIndicator::WIFIActivity);
    String path = "/";
    if (request->hasParam("path")) path = request->getParam("path")->value();
    String backpath;
    String cur = "ROOT";
    int pos = 1;
    while (pos < path.length()) {
      backpath += "/<a href=\"/?path=" + urlencode(path.substring(0, (pos == 1) ? 1 : (pos - 1))) + "\">" + cur + "</a>";
      int next = path.indexOf("/", pos);
      if (next < 0) {
        backpath += "/" + path.substring(pos);
        break;
      }   
      cur = path.substring(pos, next);
      pos = next + 1;
    }

    File dir = SD.open(path);
    if (!dir) {
      request->send(500, "text/html", String(htmlhead) + "<h1>Could not be opened</h1>" + htmlfoot);
    } else {
      if (!dir.isDirectory()) {
        request->send(dir, dir.name());
      } else {
        AsyncResponseStream *response = request->beginResponseStream("text/html");
        response->print(htmlhead);
        response->printf("<h1>Listing path:</h1><h2>%s</h2>\n", backpath.c_str());
        response->print("<table border=\"1\">");
        File entry = dir.openNextFile();
        while(entry)  {
          String uri = urlencode(entry.name());
          response->printf("<tr><td><a href=\"/?path=%s\">%s</a></li></td><td>%s</td><td><a href=\"/rm?path=%s\">REMOVE</a></td></tr>\n", uri.c_str(), entry.name(), entry.isDirectory() ? "" : String(entry.size()).c_str(), uri.c_str());
          entry = dir.openNextFile();
        }
        response->print("</table>\n");
        response->printf("<form action=\"/mkdir\">Create subdir: <input type=\"text\" name=\"dir\"><input type=\"hidden\" name=\"parent\" value=\"%s\"><input type=\"submit\" value=\"Create\"></form>\n", path.c_str());
        // cannot easily pass the parent dir via post, for some reason
        response->printf("<form action=\"/put?parent=%s\" method=\"POST\" enctype=\"multipart/form-data\">Upload Directory: <input type=\"file\" name=\"file\" multiple webkitdirectory><input type=\"submit\" value=\"Upload\"></form>\n", urlencode(path).c_str());
        response->printf("<form action=\"/put?parent=%s\" method=\"POST\" enctype=\"multipart/form-data\">Upload File: <input type=\"file\" name=\"file\"><input type=\"submit\" value=\"Upload\"></form>\n", urlencode(path).c_str());
        response->print(htmlfoot);
        request->send(response);
      }
    }
  });
  server->on("/mkdir", HTTP_GET, [] (AsyncWebServerRequest *request) {
    indicator.setTransientStatus(StatusIndicator::WIFIActivity);
    String path = "/";
    if (request->hasParam("parent")) path = request->getParam("parent")->value();
    if (request->hasParam("dir")) {
      if (!path.endsWith("/")) path += "/";
      path += request->getParam("dir")->value();
    }
    Serial.println(path.c_str());
    SD.mkdir(path);
    request->send(200, "text/html", backPage("<h1>Directory created</h1>").c_str());
  });
  server->on("/rm", HTTP_GET, [] (AsyncWebServerRequest *request) {
    indicator.setTransientStatus(StatusIndicator::WIFIActivity);
    String path;
    if (request->hasParam("path")) path = request->getParam("path")->value();
    if (path.length() < 1) return;
    rmRf(path);
    request->send(200, "text/html", backPage("<h1>Directory deleted</h1>"));
  });
  server->on("/put", HTTP_POST, [] (AsyncWebServerRequest *request) {
    request->send(200);
  }, [] (AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
    indicator.setTransientStatus(StatusIndicator::WIFIActivity);
    static File out;
    if(!index) {
      if (out) out.close();
      String dir = "/";
      if (request->hasParam("parent")) dir = request->getParam("parent")->value();
      else Serial.println("no upload dir specified");
      if (!dir.endsWith("/")) dir += "/";
      int slashpos = filename.lastIndexOf("/");
      if (slashpos > 0) {
        dir += filename.substring(0, slashpos);
        SD.mkdir(dir);
        filename = dir + filename.substring(slashpos); // includes the slash itself
      } else {
        filename = dir + filename;
      }
      if (SD.exists(filename)) SD.remove(filename);
      out = SD.open(filename, FILE_WRITE);
    }

    if (len) out.write(data, len);

    if(final){
      out.close();
      String dir;
      request->send(200, "text/html", backPage("<h1>Upload complete</h1>"));
    }
  });
  server->begin();

  indicator.setPermanentStatus(StatusIndicator::WIFIEnabled);
}

void stopWebInterface() {
  if (!server) return;
  delete server;
  server = 0;
  WiFi.mode(WIFI_OFF);
  Serial.println("WIFI stopped");
  indicator.setPermanentStatus(StatusIndicator::WIFIEnabled, false);
}

#endif
