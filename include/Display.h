// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#pragma once

#include "Arduino.h"
#include "Rect.h"
#include "epd_driver.h"  // https://github.com/Xinyuan-LilyGO/LilyGo-EPD47

typedef uint8_t Color8;

class Font;

class Display;

class Font {
  private:
    const GFXfont & _font;
    const String _name;
  public:
  Font (const GFXfont  & font, String name = ""): _font(font), _name(name) {
    _properties.fg_color = 0;
    _properties.bg_color = 15; };

  Font (Font  & font): _font(font._font), _name(font._name) {
    _properties.fg_color = font._properties.fg_color;
    _properties.bg_color = font._properties.bg_color;
  }

  ~Font (){};
    int Size() const { return _font.advance_y;}
    int Ascent() const {return _font.ascender;}
    int Descent() const {return _font.descender;}
    String Name() const {return _name;}
    Color8 Color() const {return _properties.fg_color;}
    Color8 Color(Color8 color) {
      Color8 result = (_properties.fg_color << 4);
      _properties.fg_color = (0x0f & (color >> 4));
      return result;
    }
  FontProperties _properties;

  friend class Display;
}; //class Font


enum DrawTarget { BUFFER, SCEEN };

class Display {
  public:
    enum AlignX {alignLeft, alignRight, alignCenterX};
    enum alignY {alignTop, alignBottom, alignCenterY};
    Display(); //{};
    ~Display();// {};
    static const int Width  = EPD_WIDTH;
    static const int Height = EPD_HEIGHT;
    void begin();
    void powerOn(void);
    void powerOff(void);
    void clearScreen(void);
    //void clearScreen(Rect &rect);
    //void clearScreen(Rect &rect, int cycles = 4, int time = 50);
    void clearBuffer(void);
    void update(void);
    void drawLine(int x0, int y0, int x1, int y1, Color8 color);
    void drawPixel(int x, int y, Color8 color);
    void drawHLine(int x0, int y0, int length, Color8 color);
    void drawVLine(int x0, int y0, int length, Color8 color);
    void drawRectFrame(int left, int top, int width, int height, Color8 color);
    void drawRectFrame(const Rect &rect, Color8 color);
    void drawRectCross(const Rect &rect, Color8 color);
    void fillRect(int x, int y, int w, int h, Color8 color);
    void fillRect(const Rect &rect, Color8 color);
    void drawCircle(int x0, int y0, int r, Color8 color);
    void fillCircle(int x0, int y0, int r, Color8 color);
    void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Color8 color);
    void fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Color8 color);
    void drawImage(int x, int y, int w, int h, uint8_t *data);
    void pushImage(int x, int y, int w, int h, uint8_t *data, DrawMode_t mode);
    void getTextRect(const Font &font, String text, int x, int y, Rect & rect);
    int  getTextWidth(const Font &font, const String &text);
    int  getTextWidth(const Font &font, const char * text);
    void getTextBounds(const Font &font, String text, int &w, int &h);
    int drawText(const Font &font, const char * text, int x, int y);
    int drawText(const Font &font, const String text, int x, int y);
    int drawText(const Font &font, const char * text, int x, int y, Color8 color);
    int drawText(const Font &font, const String text, int x, int y, Color8 color);
    void drawText(const Font &font, const char * text, int x, int y, AlignX align);
    void drawText(const Font &font, const String text, int x, int y, AlignX align);
    void drawText(const Font &font, const String text, int x, int y, DrawMode_t mode, enum DrawTarget target =  BUFFER);
  private:
    uint8_t *framebuffer = NULL;
}; //class Display

extern Display display;
