
/*
Statusindicator.h
 */

#ifndef STATUSINDICATOR_H
#define STATUSINDICATOR_H

#include "config.h"

#define CHANNEL_LED_GREEN 0

class StatusIndicator {
public:
  StatusIndicator() {
    transient_bits = 0;
    permanent_bits = 0;
    transient_timeout = 0;
    pinMode(LED_BLUE_PIN, OUTPUT);
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(LED_RED_PIN, OUTPUT);
    ledcSetup(CHANNEL_LED_GREEN, 5000, 8);
    ledcAttachPin(LED_GREEN_PIN, CHANNEL_LED_GREEN);
  };
  enum StatusBits {
    WIFIEnabled  = 1,
    WIFIActivity = 1 << 1,
    BatteryLow   = 1 << 2,
    Playing      = 1 << 3,
    AtFileEOF    = 1 << 4,
    Error        = 1 << 5
  };
  bool isIdle() {
    uint16_t bits = permanent_bits | transient_bits;
    if (bits & (Playing | AtFileEOF | WIFIActivity | WIFIEnabled)) return false;
    return true;
  }
  void update() {
    uint16_t bits = permanent_bits;
    uint32_t now = millis();
    if (transient_bits) {
      uint32_t delta = now - transient_timeout;
      if (delta < 0xffff) { // no overflow, i.e. time now is larger than timeout
        transient_bits = 0;
      } else {
        bits |= transient_bits;
      }
    }

    if (bits & WIFIActivity) {
      digitalWrite(LED_BLUE_PIN, (now / 500) % 2);
    } else {
      digitalWrite(LED_BLUE_PIN, bits & WIFIEnabled);
    }

    if (bits & Error) {
      digitalWrite(LED_RED_PIN, (now / 250) % 2);
    } else {
      digitalWrite(LED_RED_PIN, bits & (BatteryLow | AtFileEOF));
    }

    uint16_t error_bits = bits & (Error | AtFileEOF);
    if ((bits & Playing) && !error_bits) {
      ledcWrite(CHANNEL_LED_GREEN, 255);
    } else if (isIdle()) {
      ledcWrite(CHANNEL_LED_GREEN, abs(((now / 12) % 320) - 160) + 10);  // NOTE: Max brightness 170, min brightness 10. Looks better than 0...255
    } else {
      ledcWrite(CHANNEL_LED_GREEN, 0);
    }
  }
  // Set a status bit that will be clearer, automatically, after 500ms
  void setTransientStatus(StatusBits which) {
    transient_bits |= which;
    transient_timeout = millis() + 500;
  }
  // Set a status bit that will stick, until cleared, explicitly
  void setPermanentStatus(StatusBits which, bool on=true) {
    if (on) permanent_bits |= which;
    else permanent_bits -= (permanent_bits & which);
  }
private:
  uint16_t transient_bits;
  uint16_t permanent_bits;
  uint32_t transient_timeout;
};

StatusIndicator indicator;

#endif
