// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#pragma once

#include <Arduino.h>

#define MS_TO_S_FACTOR 1000  /* Conversion factor for micro seconds to seconds */

#define SLEEP_MSEC(us) (((uint64_t)us) * 1000L)
#define SLEEP_SEC(us) (((uint64_t)us) * 1000000L)
#define SLEEP_MIN(us) (((uint64_t)us) * 60L * 1000000L)
#define SLEEP_HR(us) (((uint64_t)us) * 60L * 60L * 1000000L)

class Power {
  public:
    Power();
    ~Power();
    void deepSleep(uint64_t time_in_us = 0);
    void lightSleep(uint64_t time_in_us = 0);
    void setWakeupPin(uint8_t pin);
    bool isWakeupByTimer();
    float  getBatteryVoltage();
    uint8_t getBatteryLevel();
    uint8_t getBatteryLevel(float voltage);
  private:
    uint8_t _wakeupPin   = 0xFF;
    uint8_t _wakeupLevel = LOW;
    int     _vref = 1100;
};

extern Power power;
