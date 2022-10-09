// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#pragma once

#include "Page.h"
#include "Display.h"
#include "Fonts.h"
#include "Format.h"

#include "FrameCurrentWeather.h"
#include "FrameForecast48hTable.h"
#include "FrameForecast48hChart.h"
#include "FrameStatus.h"

class PageWeather : public Page {
  public:
    FrameCurrentWeather   CurrentWeather   = FrameCurrentWeather(10, 0, 260, Display::Height);
    FrameForecast48hTable Forecast48hTable = FrameForecast48hTable(310, 0, 640, 284);
    FrameForecast48hChart Forecast48hChart = FrameForecast48hChart(310, 290, 640, 20 + 100 + 20 + 100);
    FrameStatus           Status           = FrameStatus(10, Display::Height - 10 - 16, 248, 16);
  public:
  PageWeather():Page() {
    addFrame(&CurrentWeather);
    addFrame(&Forecast48hTable);
    addFrame(&Forecast48hChart);
    addFrame(&Status);
  };
  void draw() {
    DEBUG("draw Page '" + Id + "' begin");
    Page::draw();
    DEBUG("draw Page '" + Id + "' end");
  };
};

Page * initPageWeather(void) {
  Page * page = new PageWeather();
  page->Id = "Weather";
  return page;
};
