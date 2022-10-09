// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#pragma once

#include "Colors.h"
#include "Config.h"
#include "Display.h"
#include "DrawUtil.h"
#include "Fonts.h"
#include "Format.h"
#include "Icon.h"
#include "Icons.h"
#include "Page.h"
#include "Table.h"
#include "WeatherData.h"

class FrameForecast48hTable : public Frame {
private:
    Table _Table; // = Table();
    const int _HourPerCol = 6;
    const int _RowCount = 8;
    const int _ColCount = 8;
    const int _RowSizes[8] = { TableRowHeightTime, TableRowHeightWeather,
        TableRowHeightLine2, TableRowHeightLine2,
        TableRowHeightLine2, TableRowHeightLine1,
        TableRowHeightLine1, TableRowHeightLine1 };

    Font& _FontValue = fontTableValue;
    Font& _FontUnits = fontTableUnits;
    Font& _FontTime = fontTableTime;

public:
    FrameForecast48hTable(int x = 0, int y = 0, int w = 0, int h = 0)
        : Frame(x, y, w, h)
        , _Table(x, y, w, h)
    {
        _Table.setRows(_RowCount, _RowSizes);
        _Table.setCols(_ColCount);
        _FontUnits.Color(ColorTableUnits);
    };

    void draw()
    {
        drawTime(TableRowTime);
        drawWeather(TableRowWeather);
        drawPrecipitation2row(TableRowPrecipitation);
        drawTemperature2row(TableRowTemperature);
        drawWindSpeed2row(TableRowWindSpeed);
        drawClouds(TableRowClouds);
        drawVisibility(TableRowVisibility);
        drawHumidity(TableRowHumidity);
        drawGrid(ColorTableGrid);
        drawFrame(ColorTableFrame);
    }

    void drawFrame(uint8_t color) { display.drawRectFrame(_Table, color); }

    void drawGrid(uint8_t color)
    {
        Rect rect;
        for (int row = 1; row < _Table.RowCount(); ++row) {
            _Table.getCellRect(0, row, rect);
            display.drawHLine(_Table.Left, rect.Top, _Table.Width, color);
        }
        for (int col = 1; col < _Table.ColCount(); ++col) {
            _Table.getCellRect(col, 0, rect);
            display.drawVLine(rect.Left, _Table.Top, _Table.Height, color);
        }
    }

    void drawText2(Rect& rect, String text1, String text2, Font& font1,
        Font& font2, Color8 color1, Color8 color2)
    {
        int w1 = display.getTextWidth(font1, text1);
        int w2 = display.getTextWidth(font2, text2);
        int x1 = rect.Left + (rect.Width - (w1 + w2)) / 2;
        int x2 = x1 + w1 + 1;
        int y = rect.CenterY() + (font1.Ascent() + font1.Descent()) / 2;
        display.drawText(font1, text1, x1, y, color1);
        display.drawText(font2, text2, x2, y, color2);
    }
    void drawText2(Rect& rect, String value, String unit,
        Color8 colorValue = ColorTableValue,
        Color8 colorUnits = ColorTableUnits)
    {
        drawText2(rect, value, unit, _FontValue, _FontUnits, colorValue,
            colorUnits);
    }
    void drawCell(int col, int row, int icon, const String value,
        const String unit, Color8 color = ColorTableValue)
    {
        Rect cellRect;
        Rect iconRect;
        Rect textRect;
        _Table.getCellRect(col, row, cellRect);
        iconRect = cellRect.splitLeft(iconMeasure.Width());
        iconMeasure.draw(icon, iconRect);
        textRect = cellRect.splitRight(-iconMeasure.Width());
        drawText2(textRect, value, unit, color);
    }
    void drawTime(int row)
    {
        Rect rect;
        String text;
        for (int col = 0; col < _ColCount; ++col) {
            _Table.getCellRect(col, row, rect);
            text = fmtTime.day_time(WeatherHourly.UTC[col * _HourPerCol + 3]);
            int x = rect.Left + (rect.Width - (display.getTextWidth(_FontTime, text))) / 2;
            int y = rect.CenterY() + (_FontTime.Ascent() + _FontTime.Descent()) / 2;
            display.drawText(_FontTime, text, x, y);
        }
    }
    void drawWeather(int row)
    {
        Rect rect;
        int id;
        for (int col = 0; col < _ColCount; ++col) {
            _Table.getCellRect(col, row, rect);
            int index = selectWeatherItem(WeatherHourly.Weather, col * _HourPerCol,
                _HourPerCol);
            int weatherId = WeatherHourly.Weather[index].Id;
            String iconName = WeatherHourly.Weather[index].Icon;
            DEBUG("Weather: " + String(weatherId) + " " + iconName + " " + WeatherHourly.Weather[index].Main + " '" + WeatherHourly.Weather[index].Description + "'");
            id = getIconId(weatherId, iconName);
            iconWeather.draw(id, rect);
        }
    }
    void drawPrecipitation2row(int row)
    {
        Rect cellRect;
        Rect iconRect;
        Rect textRect;
        Color8 color;
        float rain;
        float snow;
        float mm;
        float pop;
        int iconId;
        int index;
        bool low;
        for (int col = 0; col < _ColCount; ++col) {
            index = (col * _HourPerCol);
            rain = getSum(WeatherHourly.Rain1Hour, index, _HourPerCol);
            snow = getSum(WeatherHourly.Snow1Hour, index, _HourPerCol);
            pop = getMax(WeatherHourly.Pop, index, _HourPerCol) * 100;
            mm = rain + snow;
            _Table.getCellRect(col, row, cellRect);
            iconRect = cellRect.splitLeft(iconMeasure.Width());
            iconId = (snow > rain) ? MI_SNOW : MI_UMBELLA; // MI_RAIN;
            iconMeasure.draw(iconId, iconRect);
            low = (mm < TableThresholdPrecipitation);
            color = (low) ? ColorTableValue2 : ColorTableValue;
            textRect = cellRect.splitRight(-iconMeasure.Width());
            textRect.Height = cellRect.Height / 2;
            drawText2(textRect, fmtPrecipitation.format(mm), fmtPrecipitation.Units,
                color);
            textRect.Top = cellRect.CenterY();
            color = ((pop < TableThresholdProbability) || low) ? ColorTableValue2
                                                               : ColorTableValue;
            drawText2(textRect, fmtPercent.format(pop), fmtPercent.Units, color);
        }
    }
    void drawTemperature(int row)
    {
        Rect rect;
        int min;
        int max;
        int index;
        for (int col = 0; col < _ColCount; ++col) {
            _Table.getCellRect(col, row, rect);
            index = col * _HourPerCol;
            min = round(getMin(WeatherHourly.Temperature, index, _HourPerCol));
            max = round(getMax(WeatherHourly.Temperature, index, _HourPerCol));
            int y = rect.CenterY() + (_FontValue.Ascent() + _FontValue.Descent()) / 2;
            int x = rect.CenterX();
            if (min == max) {
                display.drawText(_FontValue, fmtTemperature.format(max) + "°", x, y,
                    Display::alignCenterX);
            } else {
                display.drawText(_FontValue, fmtTemperature.format(max) + " ", x, y,
                    Display::alignRight);
                display.drawText(_FontValue, " " + fmtTemperature.format(min) + "°", x,
                    y, ColorTableValue2);
            }
        }
    }
    void drawTemperature2row(int row)
    {
        Rect rect;
        int min;
        int max;
        int index;
        for (int col = 0; col < _ColCount; ++col) {
            _Table.getCellRect(col, row, rect);
            index = col * _HourPerCol;
            min = round(getMin(WeatherHourly.Temperature, index, _HourPerCol));
            max = round(getMax(WeatherHourly.Temperature, index, _HourPerCol));
            if (min == max) {
                drawText2(rect, fmtTemperature.format(max), fmtTemperature.Units,
                    _FontValue, _FontValue, ColorTableValue, ColorTableUnits);
            } else {
                rect.Height = rect.Height / 2;
                drawText2(rect, fmtTemperature.format(max), fmtTemperature.Units,
                    _FontValue, _FontValue, ColorTableValue, ColorTableUnits);
                rect.Top += rect.Height;
                drawText2(rect, fmtTemperature.format(min), fmtTemperature.Units,
                    _FontValue, _FontValue, ColorTableUnits, ColorTableUnits);
            }
        }
    }
    void drawWindSpeed2row(int row)
    {
        Rect cellRect;
        Rect iconRect;
        Rect textRect;
        float wind;
        float gust;
        float dir;
        int index;
        for (int col = 0; col < _ColCount; ++col) {
            _Table.getCellRect(col, row, cellRect);
            index = col * _HourPerCol;
            wind = getMean(WeatherHourly.Windspeed, index, _HourPerCol);
            gust = getMax(WeatherHourly.WindGust, index, _HourPerCol);
            dir = getMean(WeatherHourly.WindDir, index, _HourPerCol);

            iconRect = cellRect.splitLeft(iconMeasure.Width());
            drawWindDir(dir, iconRect, ColorWindDirFill, ColorWindDirLine, 2);
            textRect = cellRect.splitRight(-iconMeasure.Width());
            textRect.Height = cellRect.Height / 2;
            drawText2(textRect, fmtWindSpeed.format(wind), fmtWindSpeed.Units);
            textRect.Top = cellRect.CenterY();
            drawText2(textRect, fmtWindSpeed.format(gust), fmtWindSpeed.Units);
        }
    }
    void drawClouds(int row)
    {
        float value;
        String text;
        for (int col = 0; col < _ColCount; ++col) {
            value = getMean(WeatherHourly.Clouds, col * _HourPerCol, _HourPerCol);
            text = fmtPercent.format(value);
            drawCell(col, row, MI_CLOUD, text, fmtPercent.Units);
        }
    }
    void drawVisibility(int row)
    {
        float value;
        String text;
        for (int col = 0; col < _ColCount; ++col) {
            // Консервативный подход - берем минимальную видимость
            value = getMin(WeatherHourly.Visibility, col * _HourPerCol, _HourPerCol);
            text = fmtVisibility.format(value);
            drawCell(col, row, MI_ALIEN, text, fmtVisibility.Units);
        }
    }
    void drawHumidity(int row)
    {
        float value;
        String text;
        for (int col = 0; col < _ColCount; ++col) {
            value = getMean(WeatherHourly.Humidity, col * _HourPerCol, _HourPerCol);
            text = fmtHumidity.format(value);
            Color8 color = ((value < TableThresholdHumidityLow) || (value > TableThresholdHumidityHigh))
                ? ColorTableValue
                : ColorTableValue2;
            drawCell(col, row, MI_HUMIDITY, text, fmtHumidity.Units, color);
        }
    }
};
