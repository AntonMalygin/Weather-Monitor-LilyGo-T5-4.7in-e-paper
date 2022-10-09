// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#pragma once

struct Rect {
  int Left, Top, Width, Height;

  Rect (int x = 0, int y = 0, int w = 0, int h = 0): Left(x), Top(y), Width(w), Height(h) {}
  Rect (const Rect& src) : Left(src.Left), Top(src.Top), Width(src.Width), Height(src.Height) {}
  int Bottom() const {
    return Top + Height;
  };
  int Right() const {
    return Left + Width;
  };
  void inflate(int d) {
    Left -= d;
    Top -= d;
    Width += d * 2;
    Height += d * 2;
  };
  void inflate(int dx, int dy) {
    Left -= dx;
    Top -= dy;
    Width += dx * 2;
    Height += dy * 2;
  }
  bool isEmpty() {
    return (Width == 0) && (Height == 0);
  }
  int CenterX() {
    return Left + Width / 2;
  }
  int CenterY() {
    return Top + Height / 2;
  }
  Rect splitLeft(int w) {
    if (w > 0)
      return Rect(Left, Top, w, Height);
    else
      return Rect(Left + w, Top, Width + w, Height);
  }
  Rect splitRight(int w) {
    if (w > 0)
      return Rect(Left + Width - w, Top, w, Height);
    else
      return Rect(Left - w, Top, Width + w, Height);
  }
  Rect splitTop(int h) {
    if (h > 0)
      return Rect(Left, Top, Width, h);
    else
      return Rect(Left, Top, Width, Height + h);
  }
  Rect splitBottom(int h) {
    if (h > 0)
      return Rect(Left, Top + Height - h, Width, h);
    else
      return Rect(Left, Top - h, Width, Height + h);
  }
  Rect joinLeft(int w) {
    return Rect(Left - w, Top, w, Height);
  }
  Rect joinRight(int w) {
    return Rect(Left + Width, Top, w, Height);
  }
  Rect joinTop(int h) {
    return Rect(Left, Top - h, Width, h);
  }
  Rect joinBottom(int h) {
    return Rect(Left, Top + Height, Width, h);
  }
};
