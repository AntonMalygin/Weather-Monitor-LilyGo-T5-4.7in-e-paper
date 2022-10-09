// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#include "DrawUtil.h"
#include "Display.h"

void drawWindDir(float angle, Rect rect, Color8 f, Color8 o, int margin) {
  float sector = radians(40);
  float diametr = min(rect.Width, rect.Height) - margin;
  float radius  = diametr / 2;
  float cx = rect.CenterX();
  float cy = rect.CenterY();
  // Метеорологическое направление ветра указывает азимут точки, откуда дует ветер.
  // Соответвенно вершину направления стрелки следует развернуть на 180°
  angle = radians(float(int(angle + 180.0) % 360));

  display.drawCircle(cx, cy, round(radius), o);

  float  a[] = {angle, angle + (float) PI, angle + (float) PI - sector, angle + (float) PI + sector};
  float  r[] = {1, 0.5, 1, 1};
  float  x[] = {0, 0, 0, 0};
  float  y[] = {0, 0, 0, 0};

  for (int i = 0; i < 4; ++i) {
    x[i] = cx + sin(a[i]) * radius * r[i];
    y[i] = cy - cos(a[i]) * radius* r[i];
  }

  display.fillTriangle(x[0], y[0], x[1], y[1], x[2], y[2], f);
  display.fillTriangle(x[0], y[0], x[1], y[1], x[3], y[3], f);
}

void drawBattery(Rect rect, uint8_t level, Color8 color) {
  Rect r1, r2;
  int w1, w2;
  w2 = rect.Height / 3;
  r2 = rect.splitRight(w2);
  r2.Top += w2;
  r2.Height -= w2 * 2;
  r1 = rect.splitLeft(rect.Width -w2);
  display.fillRect(r2, color);
  display.drawRectFrame(r1, color);
  r1.inflate(-2);
  w1 = r1.Width;
  w1 = round(w1 * level / 100.0);
  r1.Width = w1;
  display.fillRect(r1, color);
}

void drawRSSI(int x, int y, int w, int h, int level, Color8 color1, Color8  color2){
  const int count = 4;
  int w2 = 1;
  int w1 = round((w - (w2 * (count - 1))) / count);
  int x1 = x;
  int threhold[5] = { -120, -100, -90, -80, -70};
  for (int i = 1; i <= count; ++i) {
    int h1 = round(((float)h / count) * i);
    Color8 c = (level >= threhold[i]) ? (color1) : (color2);
    display.fillRect(x1, y + h - h1, w1, h1, c);
    x1 += w1 + w2;
  }
}
