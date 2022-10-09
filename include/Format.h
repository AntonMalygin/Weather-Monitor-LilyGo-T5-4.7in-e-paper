// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#pragma once

#include "Arduino.h"
#include "Config.h"
#include "Lang.h"
#include <time.h>               // In-built


class Format{
public:
	String Units = "";
	int Precision = 1;
	float Factor   = 1;
	float Threshold = 0;
	bool Sign = false;
	Format(String units = "", int precision = 1, float factor = 1.0, float threshold = 0, bool sign = false):
	  Units(units), Precision(precision), Factor(factor), Threshold(threshold), Sign(sign) {
    }
	virtual String format(float value) {
		String result;
		if ((Precision == 0) || ((Threshold > 0) && (abs(value) >= Threshold))) {
			result = String(int(round(value * Factor)));
		} else {
      result = String(value * Factor, Precision);
 		}
    if ((Sign) && (value > 0))
    	result = "+" + result;
		return result;
	}
};

extern Format fmtPercent;
extern Format fmtTemperature;
extern Format fmtTemperatureChart;
extern Format fmtPrecipitation;
extern Format fmtPrecipitationInHour;
extern Format fmtHumidity;
extern Format fmtWindSpeed;
extern Format fmtPressure;
extern Format fmtVisibility;
extern Format fmtMoonAge;
extern Format fmtRSSI;

class FormatTime {
protected:
  struct tm * getlocaltime(uint32_t time) {
    time_t tm = time + Offset;
    return localtime(&tm);
  }
public:
  long Offset = 0;
  FormatTime() {}
  String format(uint32_t time, String format) {
	  struct tm *ts = getlocaltime(time);
	  char output[40];
	  strftime(output, sizeof(output), format.c_str(), ts);
	  return output;
  }
  String time(uint32_t time) {
  	struct tm *ts = getlocaltime(time);
  	char output[40];
  	sprintf(output, "%02d:%02d", ts->tm_hour, ts->tm_min);
  	return output;
  }
  String day_time(uint32_t time){
    struct tm *ts = getlocaltime(time);
  	char output[40];
  	sprintf(output, "%s %02d:%02d",  S_Weekday[ts->tm_wday], ts->tm_hour, ts->tm_min);
  	return output;
  }
  String date_time(uint32_t time){
    struct tm *ts = getlocaltime(time);
  	char output[40];
  	sprintf(output, "%s %2d %s %02d:%02d",  S_Weekday[ts->tm_wday], ts->tm_mday, S_Month[ts->tm_mon], ts->tm_hour, ts->tm_min);
  	return output;
  }
  String tz(){
    char out[17] = {0};
    if(Offset % 3600){
      sprintf(out, "UTC%ld:%02u:%02u", Offset / 3600, uint16_t(abs((Offset % 3600) / 60)), uint16_t(abs(Offset % 60)));
    } else {
      sprintf(out, "UTC%ld", Offset / 3600);
    }
    return out;
  }
  String iso(uint32_t time){
    struct tm *ts = getlocaltime(time);
  	char output[40];
  	sprintf(output, "%04d-%02d-%02d %02d:%02d:%02d",  ts->tm_year +  1900, ts->tm_mon+1, ts->tm_mday, ts->tm_hour, ts->tm_min, ts->tm_sec);
  	return output;
  }
  String isotz(uint32_t time){
    return iso(time) + " " + tz();
  }
};

extern FormatTime fmtTime;

class FormatMoon {

public:
	String Units = "";
  const double MoonDay = 29.530588853;
  FormatMoon(String units = ""): Units(units)  {
  }
  // Возраст луны в днях
  double moonAge(const time_t time) {
  	struct tm *ts = localtime(&time);
    int day    = ts->tm_mday;
    int month  = ts->tm_mon  + 1;
    int year   = ts->tm_year + 1900;
    if (month < 3) {
	    --year;
	    month += 12;
	  }
  	++month;
    int c = std::floor(365.25 * year);
    int e = std::floor(30.6000001 * month);
    double jd = c + e + day - 694039.09;
    double age = std::fmod(jd, MoonDay);
    return age;
  }
  // Процент светимости луны. Отрицательное значение - луна убывает
    int8_t moonIllumination(float age) {
  	float factor = age / MoonDay;
    float percent = (1 - cos(TWO_PI * factor) )/2 * 100;
    if (age > (MoonDay / 2))
      percent *= -1;
  	return round(percent);
  }
  String formatAge(float age) {
    return String(int(round(age)));
  }
  String formatIllumination(int8_t percent) {
    String result = String(percent);
	  if (percent > 0)
	    result = "+" + result;
	  return result;
  }
};

extern FormatMoon fmtMoon;

class FormatWindDirection {
  public:
  FormatWindDirection() {
    ;
  };
  String format(float value) {
    int index = int((value + 22.5) / 45);
    if  (index < 0) {
      index = 0;
    };
    if  (index > 7) {
      index = index % 8;
    };
    return String(S_Direction[index]);
  };
};

extern FormatWindDirection fmtWindDirection;
