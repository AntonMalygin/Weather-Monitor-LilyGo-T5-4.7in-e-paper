// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#pragma once

#include "Config.h"
#include "Application.h"
#include "Page.h"
#include "Display.h"
#include "Fonts.h"
#include "Colors.h"
#include "Internet.h"
#include "Icons.h"

#ifdef SERIAL_DEBUG
#define DEBUG_ELAPSED(text, time) DEBUG(text + (String(millis() - time) + "ms"))
#else
#define DEBUG_ELAPSED(text, time)
#endif

class PageLogo : public Page {
  public:
  void draw() {
    #ifdef SERIAL_DEBUG
    uint32_t time0 = millis();
    #endif
    DEBUG("PageLogo.draw:begin");
    Page::draw();
    drawGrid();
    drawText();
    drawIcon();
    DEBUG_ELAPSED("PageLogo.draw:done ", time0);
  };
  void drawGrid() {
    #define COLOR1 Color_13
    #define COLOR2 Color_13
    for (int y = 9; y < EPD_HEIGHT; y = y + 20) {
      display.drawHLine(0, y, EPD_WIDTH, COLOR1);
    };
    for (int x = 9; x < EPD_WIDTH; x = x + 20) {
      display.drawVLine(x, 0, EPD_HEIGHT, COLOR1);
    };

    for (int y = 19; y < EPD_HEIGHT; y = y + 20) {
      display.drawHLine(0, y, EPD_WIDTH, COLOR2);
    };
    for (int x = 19; x < EPD_WIDTH; x = x + 20) {
      display.drawVLine(x, 0, EPD_HEIGHT, COLOR2);
    };
  }
  void print(const String text1, const String text2, int x1, int x2, int y){
    display.drawText(font1, text1, x1, y, Display::alignRight);
    display.drawText(font1, text2, x2, y, Display::alignLeft);
  }
  void drawText() {
    #ifdef SERIAL_DEBUG
    uint32_t time0 = millis();
    #endif
    int x1   = 235;
    int x2   = 255;
    int y    = 160;
    int h    = 22;

    font1.Color(Color_Black);
    print("Application Title", AppTitle, x1,x2, y);

    y += h;
    String s = (__DATE__);
    s += " ";
    s += (__TIME__);
    print("Firmware Date", s, x1, x2, y);

    //y += h;
    // Расчет контрольноой суммы выполняется достаточно долго ~950ms
    // print("Firmware Hash", ESP.getSketchMD5(), x1, x2, y);

    y += h;
    s = ESP.getChipModel();
    s += " Rev ";
    s += String(ESP.getChipRevision());
    print("ESP32 Chip model", s, x1, x2, y);

    y += h;
    s = ESP.getSdkVersion();
    print("SDK Version", s, x1, x2, y);

    y += h;
    uint32_t chipId = 0;
    for (int i = 0; i < 17; i = i + 8) {
      chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
    }
    print("Chip Id", String(chipId), x1, x2, y);

    y += h;
    print("Boot count", String(bootCount), x1, x2, y);

    y += h;
    // После аппаратного сброса получать локалное время без ошибок не поучистя
    // поэтому зададим короткий таймаут для функции getLocalTime()
     print("Local Time",  getLocalTimeISOtz(), x1, x2, y);

    y += h;
    print("Battery Voltage", String(power.getBatteryVoltage()), x1, x2, y);

    y += h;
    s = String(power.getBatteryLevel());
    s += "%";
    print("Battery Level", s, x1, x2, y);

    DEBUG_ELAPSED("PageLogo.drawText:done ", time0);
  }

  void drawIcon() {
    iconWeather.drawX2(WI_CLOUDS_D, 800, 32);
  }
};
