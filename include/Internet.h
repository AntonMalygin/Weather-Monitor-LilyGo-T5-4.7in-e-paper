// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#pragma once

#include <Arduino.h>            // In-built
#include <HTTPClient.h>         // In-built
#include <WiFi.h>               // In-built
#include <SPI.h>                // In-built
#include <time.h>               // In-built

#define WIFI_RSSI_OUT -120

extern int32_t timeZoneOffset;

class Internet {
  public:
    Internet();
    ~Internet();
    bool isConnected();
    bool connect();
    void disconnect();
    void synchronizeTime();
    int  RSSI();
  private:
    bool _connected;
    int  _RSSI;
    int  _reconnectLimit;
    int  _reconnectInterval;
    //#ifdef SERIAL_DEBUG
    uint32_t _timer;
    //#endif
};

String getLocalTimeISOtz();
void printLocalTimeISOtz();
void setTimeZoneOffset(long offset);

extern Internet internet;
