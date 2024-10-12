<a name="oben"></a>

<div align="center">

|[:skull:ISSUE](https://github.com/frankyhub/Toni-Player/issues?q=is%3Aissue)|[:speech_balloon: Forum /Discussion](https://github.com/frankyhub/Toni-Player/discussions)|[:grey_question:WiKi](https://github.com/frankyhub/Toni-Player/wiki)||
|--|--|--|--|
| | | | |
|![Static Badge](https://img.shields.io/badge/RepoNr.:-%2045-blue)|<a href="https://github.com/frankyhub/Toni-Player/issues">![GitHub issues](https://img.shields.io/github/issues/frankyhub/Toni-Player)![GitHub closed issues](https://img.shields.io/github/issues-closed/frankyhub/Toni-Player)|<a href="https://github.com/frankyhub/Toni-Player/discussions">![GitHub Discussions](https://img.shields.io/github/discussions/frankyhub/Toni-Player)|<a href="https://github.com/frankyhub/Toni-Player/releases">![GitHub release (with filter)](https://img.shields.io/github/v/release/frankyhub/Toni-Player)|
|![GitHub Created At](https://img.shields.io/github/created-at/frankyhub/Toni-Player)| <a href="https://github.com/frankyhub/Toni-Player/pulse" alt="Activity"><img src="https://img.shields.io/github/commit-activity/m/badges/shields" />| <a href="https://github.com/frankyhub/Toni-Player/graphs/traffic"><img alt="ViewCount" src="https://views.whatilearened.today/views/github/frankyhub/github-clone-count-badge.svg">  |<a href="https://github.com/frankyhub?tab=stars"> ![GitHub User's stars](https://img.shields.io/github/stars/frankyhub)|
</div>



# Toni-Player

## Story
Dieses Projekt beschreibt einen einfach zu bedienenden MP3-Player für Kinder. Die wesentlichen Komponenten sind ein ESP32, ein RFID-Modul, ein SD-Kartenleser, ein Verstärker und ein Lautsprecher. Optional sind noch ein Akku mit Laderegler und ein Step-Up-Modul möglich. Die Grundidee des ToniPlayers besteht darin, dass Tracks einfach ausgewählt werden, indem ein RFID-getaggtes Objekt auf dem Player platziert wird, wodurch ein physischer Schlüssel zu einer Datei oder einen Ordner erstellt wird.

![Der ToniPlayer](/pic/ToniPlayer.png)

## Grundbedienung
Zunächst muss eine SD-Karte mit MP3-Dateien in Ordnern vorbereitet werden. Die Ordner mit MP3-Dateien können am PC kopiert oder über einen Browser nach Aktivierung des Access Point (AP) auf die SD-Karte geladen werden. Der erste RFID-Tag, der vom Lesegerät gescannt wird, wird zum "Master-Tag". Dieser startet keine Audio-Datei, sondern aktiviert die WiFi-Schnittstelle im AP-Modus, SSID "ToniPlayer", kein Passwort. Die gelbe LED leuchtet. Die nächsten gescannten RIFD-Tags werden automatisch nacheinander mit den Ordnern verknüpft, die MP3-Dateien enthalten. Die Verknüpfung zwischen RFID-Tags und Dateien wird in der Datei "tags.txt" im Stammordner der SD-Karte gespeichert. Wenn die automatische Verknüpfung nicht zu den gewünschten Ergebnissen führt, kannst du diese Datei einfach in einem Texteditor bearbeiten. Um neue Tracks auf den ToniPlayer hochzuladen, scannst du den "Master-Tag", verbindest dich in den Netzwerkeinstellungen des Handys/Tablet mit dem ToniPlayer AP und startest im Browser http://192.168.4.1 . Jetzt kannst du einen Track oder die Verzeichnis auf die SD-Karte laden. Entferne den WIFI-Tag, lege einen neuen, noch nicht zugewiesenen RFID-Tag auf den RFID-Scanner um ihn mit dem neu hochgeladenen Verzeichnis/MP3-Datei zu verknüpfen. Wird ein zugewiesener RFID-Tag auf den Kartenleser gestellt, starten die zugewiesenen MP3-Dateien und die grüne LED leuchtet. Beim Entfernen des RFID-Tag stoppt der ToniPlayer und es blinkt die grüne LED. Nach einer Minute geht der ToniPlayer in den Standby-Betrieb, alle LEDs sind dann aus. Die vor- und zurück-Taste erlauben im Play-Betrieb das skippen der MP3-Dateien. Ein kurzer Tastendruck skippt eine MP3-Datei und ein lager Tastendruck bedeutet schneller Vor- oder Rücklauf. Die Reset-Taste startet den ToniPlayer neu. Leuchtet die rote Akku-LED empfiehlt es sich den Akku zeitnah aufzuladen. Dies ist mit einem USB-Mini Kabel mit einem 5V Handy-Ladegerät oder einer 5V Power-Bank möglich. Sie zeigt auch Fehlerzustände an.

Bearbeiten der SD-Karte am PC
Kopiere deine MP3-Dateien in ein Verzeichnis. Z.B.: 01Album / 01xxx.mp3. Verzeichnisse können beliebig verschachtelt werden, aber jedes Verzeichnis sollte nur MP3-Dateien oder Unterverzeichnisse enthalten. Wenn die automatische Zuordnung von Schlüsseln zu Ordnern nicht korrekt ist, kannst du die "tags.txt" manuell bearbeiten. Du kannst ein Tag auch mit mehreren Verzeichnissen oder beliebigen Dateien verknüpfen. Der Verstärker verfügt über ein Lautstärke-Poti mit Schalter. Ist der Schalter EIN, wird der ToniPlayer mit Spannung versorgt und die POWER-LED leuchtet.






---

## Hardware
Die Hardware für den ToniPlayer:

+ ESP32 Dev Board mit Shield
+ RFID-RC522 Kartenleser
+ SD-Kartenleser (SD-Karte: 32GB 90MB/sec)
+ NF-Verstärker mit Lautsprecher
+ 3,7V Akku 18650 mit Laderegler, Akku-Case (3D-Druck)
+ Step-Up-Modul (3,7V>5V)
+ 4 LEDs, gelb, grün, 2x rot
+ 3 Taster (vor, zurück, Reset)
+ Gehäuse (Lasercutter)
+ Abstandshalter (3D-Druck)
+ Verbrauchs- und Hilfsmaterial

---

## Aufbau und Montage

Verdrahtung:

+ ESP32 > RFID-RC522
+ 3.3V -> 3.3V
+ GPIO4 -> RST
+ GND -> GND
+ N/C -> IRQ
+ GPIO19 -> MISO
+ GPIO23 -> MOSI
+ GPIO18 -> SCK
+ GPIO5 -> SDA


![Verdrahtung](/pic/Verdrahtung.png)


![Die Ladeschaltung](/pic/Ladeschaltung.png)

---

+ ESP32 > SD-Card
+ 5V -> 5V
+ GND -> GND
+ GPIO13 -> MISO
+ GPIO14 -> CLK
+ GPIO27 -> MOSI
+ GPIO15 -> CS

---

ESP32 > Audio

GPIO22 -> in
GND -> GND
GPIO39 -> Gnd (max volume) oder VCC/2 (normal volume)
GPIO36 -> 3.3V

---

Verstärker mit Lautsprecher

+ ESP32 > Anzeige
+ 4 Status-LEDs (gemeinsame Kathode, 1000Ohm Vorwiderstände):
+ GPIO21 -> Gelb (leuchtet, wenn WIFI aktiviert ist, blinkt bei WIFI-Aktivität)
+ GPIO17 -> Grün (leuchtet während des Spielens, blinkt im Leerlauf)
+ GPIO16 -> Rot (zeigt Fehlerzustände an und Akku laden)
+ GPIO12 -> Rot (zeigt Power an)


---

## Steuerung

Zwei Tasten
GPIO32 und GPIO33: Tasten, Verbindung zur Masse; Vorheriger / Nächster Titel (kurzes Drücken) - schnelles Vorspulen / Zurückspulen (langes Drücken)
Reset-Taste > ESP32 Reset
GPIO39 Lautstärkeregler

---

## Power

+ Akku > Step-Up-Modul und Laderegler
+ Step up Modul > Verstärker
+ GPIO36 -> Angeschlossen an die Batteriespannung zur Erfassung des Batteriezustands, Verbindung zu 3,3 V
+ GPIO12 -> High, wenn die Stromversorgung eingeschaltet ist

---

## Software

Bibliotheken:
[RFID-Bibliothek](https://github.com/miguelbalboa/rfid)
[ESP8266Audio](https://github.com/earlephilhower/ESP8266Audio) Diese Bibliothek kann viel mehr als nur MP3-Dekodierung!
[ESP8266Audio ESP8266_Spiram](https://github.com/Gianbacchio/ESP8266_Spiram)

Download:
ToniPlayer Software
Gehäuse
Gehäuse und 3D-Druck Dateien für den ToniPlayer:

Download:
3D-Druck Dateien ToniPlayer
Lasercutter Datei ToniPlayer

--- 

## Gehäuse

![Gehäuse](/pic/Gehaeuse.png)

---

## Inbetriebnahme

Serieller Monitor

![Serieller Monitor](/pic/TPSeriellerMonitor2.png)


Access Point

![Access Point](/pic/ToniPlayer01.png)


ToniPlayer WEB-Interface

![ToniPlayer WEB-Interface1](/pic/ToniPlayer10.png)

![ToniPlayer WEB-Interface2](/pic/ToniPlayer11.png)



---

<div style="position:absolute; left:2cm; ">   
<ol class="breadcrumb" style="border-top: 2px solid black;border-bottom:2px solid black; height: 45px; width: 900px;"> <p align="center"><a href="#oben">nach oben</a></p></ol>
</div>  

---

