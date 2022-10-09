// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#pragma once

#include "Rect.h"
#include "Display.h"


class Icon {
protected:
	int _Width  ;
	int _Height ;
	int _Count  ;
  const uint8_t * _Data = NULL;
public:
  Icon(int width, int height, int count, const uint8_t * data):
    _Width(width), _Height(height), _Count(count), _Data(data) {}
  int Count() const {return _Count;}
  int Width() const {return _Width;}
  int Height() const {return _Height;}

  void draw(int index, int x, int y) {
    if ((index < 0) || (index >= _Count))
    	 return;
    int offset = (_Width * _Height / 2) * index;
		const uint8_t * ptr = &_Data[offset];
		display.drawImage(x, y, _Width, _Height, (uint8_t *)ptr);
  }
  void draw(int index, Rect &rect) {
    int x = rect.Left + (rect.Width  - _Width) / 2;
    int y = rect.Top + (rect.Height - _Height) / 2;
    draw(index, x, y);
  }
  void drawX2(int index, int x, int y) {
    if ((index < 0) || (index >= _Count))
      return;
    int offset = (_Width * _Height / 2) * index;
    const uint8_t * ptr = &_Data[offset];
    uint8_t * line = new uint8_t [_Width * 2];
    for(int row = 0; row < _Height; ++row) {
      for (int x0 = 0; x0 < _Width; ++x0) {
        int i0 = row * _Height /2  + x0;
        uint8_t c =  ptr[i0];
        uint8_t c0 = (c & 0x0f) | ((c & 0x0f)<<4);
        int x1 = x0 * 2;
        line[x1]     = c0;
        uint8_t c1 = (c & 0xf0) | ((c & 0xf0)>>4);
        line[x1 + 1] = c1;
      }

      display.drawImage(x , y + row * 2, _Width * 2, 1, (uint8_t *)line);
      display.drawImage(x , y + row * 2 + 1, _Width * 2, 1, (uint8_t *)line);
    }
    delete [] line;
  }
  void drawX2(int index, Rect &rect) {
    int x = rect.Left + (rect.Width  - _Width * 2) / 2;
    int y = rect.Top + (rect.Height - _Height * 2) / 2;
    drawX2(index, x, y);
  }
};
