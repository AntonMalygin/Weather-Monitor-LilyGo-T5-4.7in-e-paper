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

class PageDemoIcons : public Page {
  public:
  void draw() {
    drawGlyphs(96, 178, iconWeather, 12, 2);
    drawGlyphs(96, 338, iconMeasure, 32, 1);
  }

  void drawGlyphs(int x, int y, Icon &icons, int c, int r) {
    int index = 0;
    for (int row = 0; row < r; ++row) {
      for (int col = 0; col < c; ++col) {
          if (index >= icons.Count())
            return;
          icons.draw(index, x + col * icons.Width(), y + row * icons.Height());
          ++index;
      }
    }
  }
};
