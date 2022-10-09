// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#pragma once

#include "Page.h"
#include "Display.h"
#include "Colors.h"
#include "Chart.h"
#include "Format.h"
#include "Lang.h"
#include "WeatherData.h"

class FrameForecast48hChart: public Frame {
public:
  Chart  * chart1;
  Chart  * chart2;
  ChartTemperature    * chartTemperature;
  ChartTemperature    * chartDewPoint;
  ChartPressure       * chartPressure;
  ChartPrecipitation  * chartPrecipitation;

  FrameForecast48hChart(int x = 0, int y = 0, int w = 0, int h = 0): Frame(x, y, w, h){

    const int chartWidth    = 80 * 8;
    const int chartHeight   = 100;
    const int titleHeight   = 20;

    int y0 = Top + titleHeight;
    chart1 = new Chart(Left, y0, chartWidth, chartHeight);
    y0 +=  titleHeight + chartHeight;
    chart2 = new Chart(Left, y0, chartWidth, chartHeight);

    chartTemperature = new  ChartTemperature(* chart1, fmtTemperatureChart);
    chartTemperature->Title = S_Temperatuere;
    chartTemperature->attachData(Cnt_WeatherHourly,  WeatherHourly.Temperature);

    chartDewPoint = new  ChartTemperature(* chart1, fmtTemperatureChart);
    chartDewPoint->Title = S_DevPoint;
    chartDewPoint->SideTitle = ChartData::sideRight;
    chartDewPoint->SideLabel = ChartData::sideNone;
    chartDewPoint->ColorLine  = ColorChartLine2;
    chartDewPoint->ColorTitle = ColorChartTitle2;
    chartDewPoint->AutoMin    = false;
    chartDewPoint->AutoMax    = false;
    chartDewPoint->attachData(Cnt_WeatherHourly,  WeatherHourly.DewPoint);

    chartPressure = new ChartPressure(* chart2, fmtPressure);
    chartPressure->Title = S_Pressure;
    chartPressure->attachData(Cnt_WeatherHourly,  WeatherHourly.Pressure);

    chartPrecipitation = new ChartPrecipitation(* chart2, fmtPrecipitation);
    chartPrecipitation->Title = S_Precipitation;
    chartPrecipitation->attachData(Cnt_WeatherHourly,  WeatherHourly.Rain1Hour, WeatherHourly.Snow1Hour, WeatherHourly.Pop);
    //?? chartPrecipitation->ChartData::sideRight;
    chartPrecipitation->SideTitle = ChartData::sideRight;
    chartPrecipitation->SideLabel = ChartData::sideNone;

  };

  void draw() {
    chart1->draw();
    chart2->draw();
    //chartTemperature->getRange();
    //chartDewPoint->getRange();
    chartTemperature->draw();
    chartDewPoint->Min = chartTemperature->Min;
    chartDewPoint->Max = chartTemperature->Max;
    chartDewPoint->draw();

    chartPressure->draw();
    chartPrecipitation->draw();
  }

};
