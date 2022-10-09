// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#include "Display.h"
#include "epd_driver.h"  // https://github.com/Xinyuan-LilyGO/LilyGo-EPD47

Display display;

Display::Display() {
  // framebuffer = (uint8_t *)ps_calloc(sizeof(uint8_t), EPD_WIDTH * EPD_HEIGHT / 2);
};
Display::~Display() {};

void Display::begin() {
  if (!framebuffer) {
    framebuffer = (uint8_t *)ps_calloc(sizeof(uint8_t), EPD_WIDTH * EPD_HEIGHT / 2);
    memset(framebuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);
    epd_init();
  }
}
void Display::powerOn(void) {
  epd_poweron();
}
void Display::powerOff(void) {
  epd_poweroff();
}
void Display::clearScreen(void) {
  epd_clear();
}
void Display::clearBuffer(void) {
  if (framebuffer) {
    memset(framebuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);
  }
}
void Display::update(void) {
  epd_draw_grayscale_image(epd_full_screen(), framebuffer); // Update the screen
}
void Display::drawPixel(int x, int y, Color8 color) {
  epd_draw_pixel(x, y, color, framebuffer);
}
void Display::drawLine(int x0, int y0, int x1, int y1, Color8 color) {
  epd_write_line(x0, y0, x1, y1, color, framebuffer);
}
void Display::drawHLine(int x0, int y0, int length, Color8 color) {
  epd_draw_hline(x0, y0, length, color, framebuffer);
}
void Display::drawVLine(int x0, int y0, int length, Color8 color) {
  epd_draw_vline(x0, y0, length, color, framebuffer);
}
void Display::drawRectFrame(int left, int top, int width, int height, Color8 color){
   epd_draw_rect(left, top, width, height, color, framebuffer);
}
void Display::drawRectFrame(const Rect &rect, Color8 color){
   drawRectFrame(rect.Left, rect.Top, rect.Width, rect.Height, color);
}
void Display::drawRectCross(const Rect &rect, Color8 color) {
  drawLine(rect.Left, rect.Top, rect.Right(), rect.Bottom(), color);
  drawLine(rect.Left, rect.Bottom(), rect.Right(), rect.Top, color);
}
void  Display::fillRect(int x, int y, int w, int h, Color8 color) {
  epd_fill_rect(x, y, w, h, color, framebuffer);
}
void  Display::fillRect(const Rect &rect, Color8 color) {
  epd_fill_rect(rect.Left, rect.Top, rect.Width, rect.Height, color, framebuffer);
}
void  Display::drawCircle(int x0, int y0, int r, uint8_t color) {
  epd_draw_circle(x0, y0, r, color, framebuffer);
}
void  Display::fillCircle(int x0, int y0, int r, uint8_t color) {
  epd_fill_circle(x0, y0, r, color, framebuffer);
}
void Display::drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Color8 color) {
  epd_draw_triangle(x0, y0, x1, y1, x2, y2, color, framebuffer);
}
void Display::fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Color8 color) {
  epd_fill_triangle(x0, y0, x1, y1, x2, y2, color, framebuffer);
}
void  Display::drawImage(int x, int y, int w, int h, uint8_t *data) {
  Rect_t rect = { .x = x, .y = y, .width= w, .height = h,};
  epd_copy_to_framebuffer(rect, data, framebuffer);
}
void  Display::pushImage(int x, int y, int w, int h, uint8_t *data, DrawMode_t mode){
  Rect_t area = { .x = x, .y = y, .width= w, .height = h,};
  epd_draw_image(area, data, mode);
}
void Display::getTextRect(const Font &font, String text, int x, int y, Rect & rect) {
  char *str = const_cast<char *>(text.c_str());
  get_text_bounds(&font._font, str, &x, &y, &rect.Left, &rect.Top, &rect.Width, &rect.Height, &font._properties);
  // Нужно отразить rect зеркально относительно базовой линии  y
  // потому что для шрифта вертикальные координаты возрастают в направлении снизу вверх а на экране сверху вниз
  int dy   = y - rect.Top;
  rect.Top = y + dy - rect.Height;
}
int  Display::getTextWidth(const Font &font, const char * text) {
  int x0 = 0, y0 = 0, x1, y1, w, h;
  get_text_bounds(&font._font, text, &x0, &y0, &x1, &y1, &w, &h, &font._properties);
  return w;
}
int Display::getTextWidth(const Font &font, const String &text) {
  char *str = const_cast<char *>(text.c_str());
  return getTextWidth(font, str);
}
void Display::getTextBounds(const Font &font, String text, int &w, int &h) {
  char *str = const_cast<char *>(text.c_str());
  int x0 = 0;
  int y0 = 0;
  int x1 = 0;
  int y1 = 0;
  get_text_bounds(&font._font, str, &x0, &y0, &x1, &y1, &w, &h, &font._properties);
}
int Display::drawText(const Font &font, const char * text, int x, int y) {
  write_mode(&font._font, text, &x, &y, framebuffer, BLACK_ON_WHITE, &font._properties);
  return x;
}
int Display::drawText(const Font &font, const String text, int x, int y) {
  char *str = const_cast<char *>(text.c_str());
  return drawText(font, str, x, y);
}
int Display::drawText(const Font &font, const char * text, int x, int y, Color8 color) {
  FontProperties fp = font._properties;
  fp.fg_color = 0x0f & (color >> 4);
  write_mode(&font._font, text, &x, &y, framebuffer, BLACK_ON_WHITE, &fp);
  return x;
}
int Display::drawText(const Font &font, const String text, int x, int y, Color8 color) {
  char *str = const_cast<char *>(text.c_str());
  return drawText(font, str, x, y, color);
}
void Display::drawText(const Font &font, const char * text, int x, int y, AlignX align) {
  int x0 = x;
  if (align != alignLeft) {
    int textWidth = getTextWidth(font, text);
    switch(align) {
      case alignRight: x0 -= textWidth; break;
      case alignCenterX: x0 -= textWidth/2; break;
    default:;
    }
  }
  drawText(font, text, x0, y);
}
void Display::drawText(const Font &font, const String text, int x, int y, AlignX align) {
  char *str = const_cast<char *>(text.c_str());
  drawText(font, str, x, y, align);
}
void Display::drawText(const Font &font, const String text, int x, int y, DrawMode_t mode, enum DrawTarget target){
  char *str = const_cast<char *>(text.c_str());
  uint8_t * p = (target == BUFFER) ? 0 : framebuffer;
  write_mode(&font._font, str, &x, &y, p, mode, &font._properties);
}
