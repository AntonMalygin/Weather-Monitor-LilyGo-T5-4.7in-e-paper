// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#pragma once

#include "Rect.h"
#include "Colors.h"
#include "Display.h"

void drawWindDir(float angle, Rect rect, Color8 f, Color8 o, int margin);
void drawBattery(Rect rect, uint8_t level, Color8 color);
void drawRSSI(int x, int y, int w, int h, int level, Color8 color1, Color8  color2);
