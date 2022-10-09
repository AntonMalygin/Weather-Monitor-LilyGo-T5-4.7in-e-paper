// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#pragma once

#include "Rect.h"
#include "Display.h"
#include "Format.h"
#include "Fonts.h"
#include "Colors.h"

//==============================================================================
class Ticks {

  int _count;
  int * _values;
public:
  Ticks(): _count(0), _values(NULL) {}
  Ticks(int count, int size): _count(0), _values(NULL) {
    init(count, size);
  }

  ~Ticks() {
    if (_values)
      delete [] _values;
  }
  int Count() const {return _count;}
  int Value(int index) const {return _values[index];}
  void init(int count, int size) {
    if (_values)
      delete [] _values;
    _count = count;
    _values = new int [count];
    for (int i = 0; i < count; ++i) {
      _values[i] = round((i + 1) * (float(size) / float(count + 1))) ;
      //# DEBUG(_values[i]);
    }

  }
};


//==============================================================================
class Chart: public Rect {

public:
  Ticks XTicks;
  Ticks YTicks;

  Color8 FrameColor = ColorChartFrame;
  Color8 GridColor  = ColorChartGrid;

  Chart(int x, int y, int w, int h): Rect(x, y, w, h) {
    XTicks.init(15, w);
    YTicks.init(3, h);
  }

  void drawFrame() {
    display.drawRectFrame(*this, FrameColor);
  }

  void drawGrid() {
    const int stepX = 1;
    const int stepY = 1;
    int i;
    int x;
    int y;
    for (i = 0; i < YTicks.Count(); ++i) {
       y = Top + YTicks.Value(i);
       for (x = Left + stepX; x < Left + Width; x += stepX) {
         display.drawPixel(x, y, GridColor);
       }
    }
    for (i = 0; i < XTicks.Count(); ++i) {
      x = Left + XTicks.Value(i);
      //int s = ((i % 2) == 0) ? stepY * 2 : stepY;
      int s = stepY;
      Color8 color = ((i % 2) == 0) ? ColorChartGrid2 : GridColor;
      for (y = Top + stepY; y < Top + Height; y += s) {
        display.drawPixel(x, y, color);
      }
    }
  }
  void draw() {
    drawFrame();
    drawGrid();
  }
};


//==============================================================================
class ChartData {

protected:
	Chart & _Chart;
  Format & _Format;
public:
	enum Side {sideNone, sideLeft, sideRight};
  Font & FontTitle  = fontChartTitle;
  Font & FontLabel  = fontChartLabel;
  Color8 ColorTitle = ColorChartTitle;
  Color8 ColorLabel = ColorChartLabel;
  Side SideTitle    = sideLeft;
  Side SideLabel    = sideLeft;
  int Count = 0;
  float Min = 0;
  float Max = 0;
  bool   AutoMin = true;
  bool   AutoMax = true;
  String Title = "";

  ChartData(Chart & chart, Format & format):_Chart(chart), _Format(format) {}
  virtual void draw() {
    getRange();
    drawTitle();
    drawLabels();
    drawGraph();
  }

  virtual float getMin() {
    float val;
    float min = getValue(0);
    for (int index = 1; index < Count; ++index) {
      val = getValue(index);
      if (val < min)
        min = val;
    }
    return min;
  }

  virtual float getMax() {
    float val;
    float max = getValue(0);
    for (int index = 1; index < Count; ++index) {
      val = getValue(index);
      if (val > max)
        max = val;
    }
    return max;
  }

  virtual void getRange()  {
    if (AutoMin)
      Min = getMin();
    if (AutoMax)
      Max = round(getMax() + 0.5);
  }

  bool checkIndex(int index) {
    return (index >= 0) && (index < Count);
  }

  virtual float getValue(int index)  {
    if (checkIndex(index)) {
      return 0;
    } else
      return -1;
  }

  float getYValue(int y) {
    if (y > 0)
      return Min + ((Max - Min) * (float(y) / float(_Chart.Height)));
    else
      return Min;
  }


  virtual void drawTitle() {
    drawTitle( Title + " " + _Format.Units);
  }

  virtual void drawTitle(String title) {
  	if (SideTitle == sideNone)
  		return;
    const int spaceX = 4;
    const int spaceY = 0;
    int y = _Chart.Top + FontTitle.Descent() - spaceY;
    int x = _Chart.Left;
    switch(SideTitle) {
    case sideLeft:
      x = _Chart.Left + spaceX;
      break;
    case sideRight:
      x = _Chart.Left + _Chart.Width  - display.getTextWidth(FontTitle, title) - spaceX;
      break;
    default:
      ;
    }
    Color8 fontColor = FontTitle._properties.fg_color;
    FontTitle._properties.fg_color = ColorTitle;
    display.drawText(FontTitle, title, x, y);
    FontTitle._properties.fg_color = fontColor;
  }


  virtual void drawLabels() {
  	if (SideLabel == sideNone)
  		return;
  	const int spaceX = 4;
    int x = _Chart.Left;
    Display::AlignX align = Display::alignCenterX;
    String s;
    switch(SideLabel) {
    case sideLeft:
      x = _Chart.Left - spaceX;
      align = Display::alignRight;
      break;
    case sideRight:
      x = _Chart.Left + _Chart.Width + spaceX;
      align = Display::alignLeft;
      break;
    default:
      ;
    }
    int dy = 0 ; //FontLabel.Descent();
    s = _Format.format(Max);
    display.drawText(FontLabel, s, x, _Chart.Top + dy, align);
    s = _Format.format(Min);
    display.drawText(FontLabel, s, x, _Chart.Bottom() + dy, align);

    for (int i = 0; i < _Chart.YTicks.Count(); ++i){
      int y = _Chart.Bottom()  - _Chart.YTicks.Value(i) + dy;
      s = _Format.format(getYValue(_Chart.YTicks.Value(i)));
      display.drawText(FontLabel, s, x, y, align);
    }
  }

  virtual void drawGraph() {

  }

  int scaleY(float value) {
    return round((constrain(value, Min, Max) - Min) / (Max - Min) * _Chart.Height);
  }

};


//==============================================================================
class ChartLine: public ChartData {
  protected:
  const float * _Data;
  public:
    ChartLine(Chart & chart, Format & format):ChartData(chart, format), _Data(NULL) {}
    Color8 ColorLine = ColorChartLine;
    void attachData(int count, const float * data){
      Count = count;
      _Data = data;
    }
    float getValue(int index) {
    if (checkIndex(index))
      return _Data[index];
    else
      return 0;
    }

    void  drawGraph() {
      DEBUG("Chart", Title);
      if (Count <= 0)
        return;
      if (_Data == NULL)
        return;
      DEBUG("Chart Count", Count);
      DEBUG("Chart Min", Min);
      DEBUG("Chart Max", Max);
      int x1, x2;
      int y1, y2;
      float value = _Data[0];
      int   h = scaleY(value);
      x1 = _Chart.Left;
      y1 = _Chart.Top + _Chart.Height - h;
      float w =  float(_Chart.Width) / float(Count - 1);
      Color8 color2 = (ColorLine + 4);
      for(int index = 1; index < Count; ++index) {
        value = _Data[index];
        h = scaleY(value);
        x2 = round(_Chart.Left + index * w);
        y2 = _Chart.Top + _Chart.Height - h;
        display.drawLine(x1, y1, x2, y2, ColorLine);
        display.drawLine(x1, y1+1, x2, y2+1, color2);
        x1 = x2;
        y1 = y2;
      }
    }
};

//==============================================================================
class ChartTemperature: public ChartLine {
public:
  ChartTemperature(Chart & chart, Format & format):ChartLine(chart, format) {}
  void getRange()  {
    const float step = 4.0;
    if (AutoMin) {
      float low = getMin();
      if (low > 0){
        low = 0;
      } else {
        low = min(-step, float(floor(low / step) * step));
      }
      Min = low;
    }
    if (AutoMax) {
      float high = getMax();
      if (high < 0) {
        high = 0;
      } else {
        high = max(step, float(ceil(high / step) * step));
      }
      Max = high;
    }
  }

};

//==============================================================================
class ChartPressure: public ChartLine {
public:
  ChartPressure(Chart & chart, Format & format):ChartLine(chart, format) {}
  void getRange() {
    const float step =  2.0;
    const float range = 4.0;
    float max = getMax();
    float min = getMin();
    float amp = (max - min) /2;
    float mid = min + amp;
    #ifdef SERIAL_DEBUG
    Serial.printf("min %.1f max %.1f mid %.1f  amp %.1f\n", min, max, mid, amp);
    #endif
    if (amp < range) {
      mid = round(mid / step) * step;
      max = mid + range;
      min = mid - range;
      amp = (max - min) /2;
    }
    #ifdef SERIAL_DEBUG
    Serial.printf("min %.1f max %.1f mid %.1f  amp %.1f\n", min, max, mid, amp);
    #endif
    min = floor(min / step) * step - step;
    max = ceil(max / step) * step + step;
    #ifdef SERIAL_DEBUG
    Serial.printf("min %.1f max %.1f mid %.1f  amp %.1f\n", min, max, mid, amp);
    #endif
    Max = max;
    Min = min;
  }

};

//==============================================================================
class ChartPrecipitation: public  ChartData {
  protected:
  const float * Rain        = NULL;
  const float * Snow        = NULL;
  const float * Probability = NULL;

  public:
    Color8 LineColor = Color_Black;
    ChartPrecipitation(Chart & chart, Format & format):ChartData(chart, format){
      AutoMin = false;
    }

  void attachData(int count, float * rain, float * snow, float * probability) {
    Count = count;
    Rain  = rain;
    Snow  = snow;
    Probability = probability;
  }

  float getValue(int index) {
    float value = 0;
    if (Rain != NULL)
      value = value + Rain[index];
    if (Snow != NULL)
      value = value + Snow[index];
    return value;
  }

  void getRange()  {
    Min = 0;
    float max = getMax();
    if (max <= 5)
      max = 5;
    else
      max = round(max / 10) * 10;
    Max = max;
  }

  void drawGraph() {
     DEBUG("Chart", Title);
    if (Count <= 0)
      return;
    DEBUG("Chart Count", Count);
    DEBUG("Chart Min", Min);
    DEBUG("Chart Max", Max);

    float value;
    int x;
    int y;
    // Ширина столбика
    int w = floor(_Chart.Width / Count) - 1;
    int h;
    // Количество часовых столбиков в колонке таблицы
    int n = (Count / 8);
    x = _Chart.Left;
    for (int index = 0; index < Count; ++index) {
      value = getValue(index);
      h = scaleY(value);
      y = _Chart.Top + _Chart.Height - h;
      // Выравниваение координаты x на начало каждой клонки таблицы
      if ((index % n) == 0) {
        x = int(_Chart.Width / 8) * int(index / n) + _Chart.Left;
      }
      // Вероятность осадков
      float p = Probability[index];
      // Рисуем если уровень осадков более 0.1 мм
      if (value >= 0.1) {
        // Цвет заливки в зависимости от вероятности осадков
        Color8 c = getBarColor(p);
        display.fillRect(x, y, w, h, c);
        display.drawRectFrame(x, y, w, h, LineColor);
      }
      x = x + w + 1;
      // println("# " + index + " value: " + value + " p: " + p + " h: " + h + " x: " + x + " y: " + y);
    }
  }

  Color8 getBarColor(float p) {
    Color8 c = round(255 * (1 - p));
    return c;
  }

  void drawBar(int x, int y, int w, int h, Color8 c) {
    display.fillRect(x, y, w, h, c);
    display.drawRectFrame(x, y, w, h, LineColor);
  }

  void drawTitle() {
    String scale = " 0.." + _Format.format(Max) + " " + _Format.Units;
    ChartData::drawTitle(Title + scale);
  }
};
