/////////////////////////////////////////////////////////////////
/*

  Button2.cpp - Arduino Library to simplify working with buttons.
  Created by Lennart Hennigs.
  Rework gembeider@ya.ru (https://gitflic.ru/user/gembeider)

*/
/////////////////////////////////////////////////////////////////
#pragma once

/////////////////////////////////////////////////////////////////

#include "Arduino.h"

/////////////////////////////////////////////////////////////////

#define DEBOUNCE_MS 50
#define LONGCLICK_MS 250
#define DOUBLECLICK_MS 400
#define CAPACITIVE_TOUCH_THRESHOLD 35

#define SINGLE_CLICK 1
#define DOUBLE_CLICK 2
#define TRIPLE_CLICK 3
#define LONG_CLICK 4

/////////////////////////////////////////////////////////////////

class Button2m {

public:
  typedef enum {
    EventPressed,
    EventReleased,
    EventChange,
    EventTap,
    EventClick,
    EventLong,
    EventDouble,
    EventTriple,
    EventLongPress
  } Event_t;

  typedef void (*Callback_t)(Button2m & btn, Event_t event);

public:
  int Tag;
  Button2m();

  Button2m(byte attachTo, byte buttonMode = INPUT_PULLUP, boolean isCapacitive = false, boolean activeLow = true);
  void begin(byte attachTo, byte buttonMode = INPUT_PULLUP, boolean isCapacitive = false , boolean activeLow  = true);
  void begin(byte attachTo,  Button2m::Callback_t cb, int tag);

  void setDebounceTime(uint16_t ms);
  void setLongClickTime(uint16_t ms);

  void setDoubleClickTime(uint16_t ms);
  uint16_t getLongClickTime() const;

  uint16_t getDoubleClickTime() const;
  byte getPin() const;
  bool readPin() const;
  void setLongClickDetectedRetriggerable(bool retriggerable);
  void reset();
  void setCallback(Callback_t f);
  unsigned int getPressedTime() const;
  boolean isPressed() const;
  boolean isPressed(bool debounce) const;
  byte getClickCount() const;
  byte getClickType() const;
  bool operator==(Button2m &rhs);

  void loop();

protected:
  byte pin;
  bool capacitive = false;
  byte prev_state;
  byte state;
  byte pressed;
  byte click_count = 0;
  byte last_click_type = 0;
  uint32_t click_ms;
  uint32_t down_ms;

  bool longclick_detected_retriggerable;
  uint16_t longclick_detected_counter;
  bool longclick_detected = false;
  bool longclick_detected_reported = false;

  uint16_t debounce_time_ms;
  uint16_t longclick_time_ms;
  uint16_t doubleclick_time_ms;

  uint16_t down_time_ms = 0;
  bool pressed_triggered = false;

  Callback_t  callback = NULL;

  void doEvent(Event_t event);

};
