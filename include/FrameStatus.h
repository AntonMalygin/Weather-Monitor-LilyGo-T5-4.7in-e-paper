// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#pragma once

#include "Config.h"
#include "Page.h"
#include "Display.h"
#include "Colors.h"
#include "Format.h"
#include "Fonts.h"
#include "Icons.h"
#include "DrawUtil.h"
#include "Power.h"
#include "Internet.h"

class FrameStatus: public Frame {
protected:

public:
	FrameStatus(int x, int y, int w, int h): Frame(x, y, w, h){};
  void draw() {
    int x = Left;
    int y = Bottom();
    x = drawBatteryLevel(x, y);
    x = drawSignalLevel(x + 8, y);
  }
 int drawBatteryLevel(int x, int y) {
    uint8_t   level = power.getBatteryLevel();
    const int BatteryWidth  = 28;
    const int BatteryHeight = 14;
    drawBattery(Rect(x,  y - BatteryHeight, BatteryWidth, BatteryHeight), level, ColorBattery);
    x += BatteryWidth + 1;
    x = display.drawText(fontChartTitle, fmtPercent.format(level), x, y);
    x = display.drawText(fontTableUnits, fmtPercent.Units, x, y);
    return x;
  }
  int drawSignalLevel(int x, int y) {
    int   level = internet.RSSI();
    const int LevelWidth  = 16;
    const int LevelHeight = 16;
    drawRSSI(x, y - LevelHeight, LevelWidth, LevelHeight, level, ColorRSSIDark, ColorRSSILight);
    x += LevelWidth + 1;
    x = display.drawText(fontChartTitle, fmtRSSI.format(level), x, y);
    x = display.drawText(fontTableUnits, fmtRSSI.Units, x, y, ColorTableUnits);
    return x;
  }
};
