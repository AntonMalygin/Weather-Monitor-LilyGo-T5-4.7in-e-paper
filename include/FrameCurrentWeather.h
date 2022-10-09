// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#pragma once

#include "Config.h"
#include "Page.h"
#include "Display.h"
#include "Colors.h"
#include "Format.h"
#include "Fonts.h"
#include "Icons.h"
#include "WeatherData.h"
#include "DrawUtil.h"

class FrameCurrentWeather: public Frame {
protected:
  const int  _IconSize     = 28;
  const int  _RowHeight1   = 35;
  const int  _RowHeight2   = 26;
  Font & _FontTime    = fontTableTime;
  Font & _FontUnits   = fontTableUnits;
  Font & _FontValue1  = fontTableValue2;
  Font & _FontValue2  = fontTableValue2;
public:
	FrameCurrentWeather(int x = 0, int y = 0, int w = 0, int h = 0): Frame(x, y, w, h){};
  void draw() {
    drawView0();
  }
  void drawRectFrame(Rect rect) {
    display.drawRectFrame(rect.Left, rect.Top, rect.Width + 1, rect.Height +1, ColorTableGrid);
  }
  void drawWeatherTimeStamp(Rect rect) {
    String text = fmtTime.date_time(WeatherCurrent.UTC);
    int x  = rect.Left + (rect.Width - (display.getTextWidth(_FontTime, text))) / 2;
    int y  = rect.CenterY() + (_FontTime.Ascent() + _FontTime.Descent())/ 2;
    display.drawText(_FontTime, text, x, y);
    drawRectFrame(rect);
  }
  void drawValue(Rect rect, int icon, int align, String value, String unuts, Font fontValue, Font fontUnits,
    Color8 colorValue = ColorTableValue, Color8 colorUnits = ColorTableUnits ) {
    Rect iconRect = rect.splitLeft(_IconSize);
    if (icon == -2)
      iconRect.Width = 0;
    if (icon >= 0)
      iconMeasure.draw(icon, iconRect);
    Rect textRect = rect.splitRight( - iconRect.Width); //rect.Height);
    int y  = textRect.CenterY() + (fontValue.Ascent() + fontValue.Descent())/ 2;
    int x1, x2;
    if (align == 0) {
      int w1 = display.getTextWidth(fontValue, value);
      int w2 = display.getTextWidth(fontUnits, unuts);
      x1 = textRect.CenterX() - (w1 + w2) /2;
      x2 = x1 + w1;
    } else {
      x1 = textRect.CenterX() + align - display.getTextWidth(fontValue, value);
      x2 = textRect.CenterX() + align;
    }
    display.drawText(fontValue, value, x1, y, colorValue);
    display.drawText(fontUnits, unuts, x2, y, colorUnits);
    drawRectFrame(rect);
  }

  void drawView0(){
    const int hugeRectWidth  = 160;
    const int hugeRectHeight = 140;

    int h = _RowHeight1;
    int align = 14;

    drawWeatherTimeStamp(splitTop(TableRowHeightTime));

    Rect  rectLeft  = Rect(Left, TableRowHeightTime, hugeRectWidth, hugeRectHeight);
    drawWeatherIcon(rectLeft);
    rectLeft.Top  += hugeRectHeight;
    drawTemperature(rectLeft);

    Rect rectRight = Rect(Left + hugeRectWidth, TableRowHeightTime, Width - hugeRectWidth, h);
    drawPrecipitation(rectRight, align);
    rectRight.Top += h;
    drawHumidity(rectRight, align);
    rectRight.Top += h;
    drawClouds(rectRight, align);
    rectRight.Top += h;
    drawVisibility(rectRight, align);

    rectRight.Top += h;
    drawTemperatureMax(rectRight, align);
    rectRight.Top += h;
    drawTemperatureMin(rectRight, align);
    rectRight.Top += h;
    drawTemperatureDelta(rectRight, align);
    rectRight.Top += h;
    drawTemperatureDevPoint(rectRight, align);

    align = 10;
    rectRight.Top += h;
    drawWindGust(rectRight, align);
    rectRight.Top += h;
    drawPressureDelta(rectRight, align);
    align = 30;
    rectLeft.Top += hugeRectHeight;
    rectLeft.Height = h;
    drawWindSpeed(rectLeft, align);
    rectLeft.Top += h;
    drawPressure(rectLeft, align);

    rectLeft.Top += h + 10;
    rectLeft.Width = Width / 2 - 8;
    drawSunRise(rectLeft);
    rectRight.Top   = rectLeft.Top;
    rectRight.Width = rectLeft.Width;
    rectRight.Left  = rectLeft.Right();
    drawSunSet(rectRight);

    align = 30;
    rectLeft.Top += h;
    drawMoonAge(rectLeft, align);
    rectRight.Top += h;
    drawMoonIllumination(rectRight, align);
    rectLeft.Top += h;
    drawMoonRise(rectLeft);
    rectRight.Top += h;
    drawMoonSet(rectRight);
  }
  void drawWeatherIcon(Rect rect){
    int icon = getIconId(WeatherCurrent.Weather.Id, WeatherCurrent.Weather.Icon);
    iconWeather.drawX2(icon, rect);
    drawRectFrame(rect);
  }
  void drawTemperature(Rect rect){
    int  value = round(WeatherCurrent.Temperature);
    String text = fmtTemperature.format(value) + fmtTemperature.Units;
    int  w = display.getTextWidth(fontHugeDigital, text);
    int  x = rect.CenterX() - w /2;
    int  y = rect.CenterY() + (fontHugeDigital.Ascent() + fontHugeDigital.Descent()) / 2;
    display.drawText(fontHugeDigital, text, x, y);
    drawRectFrame(rect);
  }
  void drawPrecipitation(Rect rect, int align) {
    int icon = (WeatherCurrent.Rain1Hour >= WeatherCurrent.Snow1Hour)? MI_UMBELLA : MI_SNOW;
    float Precipitation = WeatherCurrent.Rain1Hour + WeatherCurrent.Snow1Hour;
    drawValue(rect, icon, align, fmtPrecipitation.format(Precipitation), fmtPrecipitation.Units, _FontValue1, _FontUnits);
  }
  void drawHumidity(Rect rect, int align) {
    drawValue(rect, MI_HUMIDITY, align, fmtHumidity.format(WeatherCurrent.Humidity), fmtHumidity.Units, _FontValue1, _FontUnits);
  }
  void drawClouds(Rect rect, int align) {
    drawValue(rect, MI_CLOUD, align, fmtPercent.format(WeatherCurrent.Clouds), fmtPercent.Units, _FontValue1, _FontUnits);
  }
  void drawVisibility(Rect rect, int align) {
    drawValue(rect, MI_ALIEN, align, fmtVisibility.format(WeatherCurrent.Visibility), fmtVisibility.Units, _FontValue1, _FontUnits);
  }
  void drawTemperature(Rect rect, int align, int icon, float value) {
    drawValue(rect, icon, align, fmtTemperature.format(value), fmtTemperature.Units, _FontValue1, _FontValue1);
  }
  void drawTemperatureMax(Rect rect, int align) {
    drawTemperature(rect, align, MI_MAX, WeatherDaily.TemperatureMax[0]);
  }
  void drawTemperatureMin(Rect rect, int align) {
    drawTemperature(rect, align, MI_MIN, WeatherDaily.TemperatureMin[0]);
  }
  void drawTemperatureDelta(Rect rect, int align) {
    float delta = getDeltaTemperature();
    int icon = (delta > 0) ? MI_UP: MI_DOWN;
    if (round(delta) == 0)
      icon = MI_FLAT;
    drawTemperature(rect, align, icon, delta);
  }
  void drawTemperatureDevPoint(Rect rect, int align) {
    drawTemperature(rect, align, MI_RAIN, WeatherCurrent.DewPoint);
  }
  void drawWindGust(Rect rect, int align) {
    drawValue(rect, MI_MAX, align, fmtWindSpeed.format(WeatherCurrent.WindGust), fmtWindSpeed.Units, _FontValue1, _FontUnits);
  }
  void drawPressureDelta(Rect rect, int align) {
    float delta = getDeltaPressure(PRESSURE_TREND_HISTRORY_HOUR);
    int icon = (delta > 0) ? MI_UP: MI_DOWN;
    if (round(delta) == 0)
      icon = MI_FLAT;
    drawValue(rect, icon, align, fmtPressure.format(delta), fmtPressure.Units, _FontValue1, _FontUnits);
  }
  void drawWindSpeed(Rect rect, int align) {
    // Стрелка с направлением ветра
    Rect iconRect = rect.splitLeft(rect.Height);
    drawWindDir(WeatherCurrent.WindDir, iconRect, ColorWindDirFill, ColorWindDirLine, 6);
    // Краткое название напваления ветра
    Rect dirRect =  iconRect.joinRight(rect.Height);
    String text = fmtWindDirection.format(WeatherCurrent.WindDir);
    // Serial.printf("Wind Direction %f  %s\n", WeatherCurrent.WindDir,  text.c_str());
    // Символы "СЮВЗ" есть только в шрифте font1
    // Если есть желание сменить шрифт то в него нужно добавить недостающие символы
    int  w = display.getTextWidth(font1, text);
    int  x = dirRect.CenterX() - w /2;
    int  y = dirRect.CenterY() + (font1.Ascent() + font1.Descent()) / 2 + 2;
    display.drawText(font1, text, x, y, ColorTableValue);
    // Скорость ветра
    drawValue(rect, -1, align, fmtWindSpeed.format(WeatherCurrent.WindSpeed), fmtWindSpeed.Units, _FontValue1, _FontUnits);
  }
  void drawPressure(Rect rect, int align) {
    drawValue(rect, MI_PRESSURE, align, fmtPressure.format(WeatherCurrent.Pressure), fmtPressure.Units, _FontValue1, _FontUnits);
  }
  void drawSunRise(Rect rect) {
    drawValue(rect, MI_SUNRISE, 0, fmtTime.time(WeatherCurrent.Sunrise), "", _FontValue1, _FontValue1);
  }
  void drawSunSet(Rect rect) {
    drawValue(rect, MI_SUNSET, 0, fmtTime.time(WeatherCurrent.Sunset), "", _FontValue1, _FontValue1);
  }
  void drawMoonAge(Rect rect, int align) {
    //float moonAge = fmtMoon.moonAge(WeatherCurrent.UTC);
    float moonAge = WeatherDaily.MoonPhase[0] * fmtMoon.MoonDay;
    drawValue(rect, MI_MOON, align, fmtMoon.formatAge(moonAge), fmtMoon.Units, _FontValue1, _FontUnits);
  }
  void drawMoonRise(Rect rect) {
    drawValue(rect, MI_MOONRISE, 0, fmtTime.time(WeatherDaily.Moonrise[0]), "", _FontValue1, _FontValue1);
  }
  void drawMoonSet(Rect rect) {
    uint32_t moonset = WeatherDaily.Moonset[0];
    // Если время захода луны имеет нулевое значение
    // Возмем значение на следующие сутки
    if (moonset == 0) {
      moonset = WeatherDaily.Moonset[1];
    }
    drawValue(rect, MI_MOONSET, 0, fmtTime.time(moonset), "", _FontValue1, _FontValue1);
  }
  void drawMoonIllumination(Rect rect, int align) {
    // Пологаю что следует указывать светимость в предстоящую полночь (24:00)
    // а не в 00:00 как в rp5.ru
    const uint32_t SID = 86400UL;
    uint32_t utc = WeatherCurrent.UTC;
    utc =  utc - (utc % SID) + SID;  // Смещение UTC можно не учитывать
    float moonAge = fmtMoon.moonAge(utc);
    int8_t Illumination = fmtMoon.moonIllumination(moonAge);
    int icon = (Illumination >= 0) ? MI_UP: MI_DOWN;
    drawValue(rect, icon, align, fmtMoon.formatIllumination(Illumination),
      fmtPercent.Units, _FontValue1, _FontUnits);
  }
};
