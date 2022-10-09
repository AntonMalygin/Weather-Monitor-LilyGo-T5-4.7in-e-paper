/////////////////////////////////////////////////////////////////
/*

  Button2.cpp - Arduino Library to simplify working with buttons.
  Created by Lennart Hennigs.
  Rework gembeider@ya.ru (https://gitflic.ru/user/gembeider)

*/
/////////////////////////////////////////////////////////////////

#include "Button2m.h"

void Button2m::doEvent(Event_t event) {
  if (callback != NULL) {
    callback(*this, event);
  }
};

Button2m::Button2m() {
  pin = 0xff;
};

Button2m::Button2m(byte attachTo, byte buttonMode, boolean isCapacitive, boolean activeLow) {
  begin(attachTo, buttonMode, isCapacitive, activeLow);
};

void Button2m::begin(byte attachTo, byte buttonMode, boolean isCapacitive, boolean activeLow) {
  pin = attachTo;
  longclick_detected_retriggerable = false;
  setDebounceTime(DEBOUNCE_MS);
  setLongClickTime(LONGCLICK_MS);
  setDoubleClickTime(DOUBLECLICK_MS);
  pressed = activeLow ? LOW : HIGH;
  state = activeLow ? HIGH : LOW;
  if (!isCapacitive) {
    pinMode(attachTo, buttonMode);
  } else {
    capacitive = true;
  }
 };

void Button2m::begin(byte attachTo,  Button2m::Callback_t cb, int tag) {
  begin(attachTo);
  callback = cb;
  Tag = tag;
};

void Button2m::setDebounceTime(uint16_t ms) {
  debounce_time_ms = ms;
};

void Button2m::setLongClickTime(uint16_t ms) {
  longclick_time_ms = ms;
};

void Button2m::setDoubleClickTime(uint16_t ms) {
  doubleclick_time_ms = ms;
};

uint16_t Button2m::getLongClickTime() const{
  return longclick_time_ms;
};

uint16_t Button2m::getDoubleClickTime() const {
  return doubleclick_time_ms;
};

byte Button2m::getPin() const {
  return pin;
};

bool Button2m::readPin() const {
  return digitalRead(pin);
};

void Button2m::setLongClickDetectedRetriggerable(bool retriggerable){
  longclick_detected_retriggerable = retriggerable;
};

void Button2m::reset() {
  pin = 0xff;
  click_count = 0;
  last_click_type = 0;
  down_time_ms = 0;
  pressed_triggered = false;
  longclick_detected = false;
  longclick_detected_reported = false;
  callback = NULL;
};

void Button2m::setCallback(Callback_t f) {
  callback = f;
};

unsigned int Button2m::getPressedTime() const {
  return down_time_ms;
};

boolean Button2m::isPressed() const {
  return (state == pressed);
};

boolean Button2m::isPressed(bool debounce) const {
  if (debounce) {
    return (state == pressed);
  } else {
    return (readPin() == pressed);
  }
}

byte Button2m::getClickCount() const {
  return click_count;
};

byte Button2m::getClickType() const {
  return last_click_type;
};

bool Button2m::operator==(Button2m &rhs) {
  return (this==&rhs);
};

void Button2m::loop() {
  if (pin == 0xFF)
    return;

  prev_state = state;
  uint32_t now = millis();
  if (!capacitive) {
    state = digitalRead(pin);
  } else {
    #if defined(ARDUINO_ARCH_ESP32)
      int capa = touchRead(pin);
      state = capa < CAPACITIVE_TOUCH_THRESHOLD ? LOW : HIGH;
    #endif
  }
  // is button pressed?
  if (state == pressed && prev_state != pressed) {
    down_ms = now;
    pressed_triggered = false;
    click_ms = down_ms;

  // trigger pressed event (after debounce has passed)
  } else if (state == pressed && !pressed_triggered && (now - down_ms >= debounce_time_ms)) {
    pressed_triggered = true;
    click_count++;
    doEvent(EventChange);
    doEvent(EventPressed);

  // is the button released?
  } else if (state != pressed && prev_state == pressed) {
    down_time_ms = now - down_ms;
    // is it beyond debounce time?
    if (down_time_ms >= debounce_time_ms) {
      // trigger release
      doEvent(EventChange);
      doEvent(EventReleased);
      // trigger tap
      doEvent(EventTap);
      // was it a longclick? (preceeds single / double / triple clicks)
      if (down_time_ms >= longclick_time_ms) {
        longclick_detected = true;
      }
    }

  // is the button released and the time has passed for multiple clicks?
  } else if (state != pressed && now - click_ms > doubleclick_time_ms) {
    // was there a longclick?
    if (longclick_detected) {
      // was it part of a combination?
      if (click_count == 1) {
        last_click_type = LONG_CLICK;
        doEvent(EventLong);
      }
      longclick_detected = false;
      longclick_detected_reported = false;
      longclick_detected_counter = 0;
      // determine the number of single clicks
    } else if (click_count > 0) {
      switch (click_count) {
        case 1:
          last_click_type = SINGLE_CLICK;
          doEvent(EventClick);
          break;
        case 2:
          last_click_type = DOUBLE_CLICK;
          doEvent(EventDouble);
          break;
        case 3:
          last_click_type = TRIPLE_CLICK;
          doEvent(EventTriple);
          break;
      }
    }
    click_count = 0;
    click_ms = 0;
  }

  bool longclick_period_detected = now - down_ms >= (longclick_time_ms * (longclick_detected_counter + 1));

  // check to see that the longclick_ms period has been exceeded and call the appropriate callback
  if (state == pressed && longclick_period_detected && !longclick_detected_reported) {
    longclick_detected_reported = true;
    longclick_detected = true;
    if (longclick_detected_retriggerable) {
      // if it's retriggerable then we bump the counter and reset the "reported" flag (as the counter will stop the false trigger)
      longclick_detected_counter++;
      longclick_detected_reported = false;
    }
//      longpress_detected_ms = now;
    doEvent(EventLongPress);
  }

};
