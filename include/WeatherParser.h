// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#pragma once

#include <Arduino.h>
#include <HTTPClient.h>         // In-built
#include <WiFi.h>               // In-built
#include <SPI.h>                // In-built
#include <time.h>               // In-built
#include <ArduinoJson.h>        // https://github.com/bblanchon/ArduinoJson
#define SERIAL_DEBUG true
#include <FormattingSerialDebug.h>
#include "WeatherData.h"
#include "Format.h"
#include "Config.h"
#include "Private.h"


// #define DEBUDWX_ENABLE
// Отладочный вывод историии погоды
// #define DEBUG_HISTORY_1

#define UTC2ISO(time)  (String(time) + " " + fmtTime.iso(time))
#define UTC2ISOtz(time)  (String(time) + " " + fmtTime.isotz(time))

#ifdef DEBUDWX_ENABLE

  #define DEBUGW0(A) DEBUG(A)
  #define DEBUGW1(A) DEBUG(A)
  #define DEBUGW2(A) DEBUG(A)
  #define DEBUGW3(A) DEBUG(A)
  #define DEBUGW4(A) DEBUG(A)
  #define DEBUGW6(A) DEBUG(A)

#else

  #define DEBUGW0(A)
  #define DEBUGW1(A)
  #define DEBUGW2(A)
  #define DEBUGW3(A)
  #define DEBUGW4(A)
  #define DEBUGW6(A)

#endif


#ifdef DEBUG_HISTORY_1
  #define  DEBUG_H1(A)  DEBUG(A)
#else
  #define DEBUG_H1(A)
#endif


bool DecodeWeatherJson(DynamicJsonDocument & json) {
  bool result = false;
  DEBUG(F("DecodeWeatherJson"));
  JsonObject root = json.as<JsonObject>();
  DEBUG(F("Weather.Current:"));
  int count = 0;
  if (! root.containsKey("lon")) {
    DEBUG(F("WeatherData EMTPY"));
    return false;
  }
  #ifdef SERIAL_DEBUG
  uint32_t time0 = millis();
  #endif
  WeatherRoot.Lon             = root["lon"];                      DEBUGW1("Lon: " + String(WeatherRoot.Lon));
  WeatherRoot.Lat             = root["lat"];                      DEBUGW1("Lat: " + String(WeatherRoot.Lat));
  if (WeatherRoot.Lon == 0)
    return false;


  WeatherRoot.TimezoneName    = root["timezone"].as<String>();      DEBUG("Timezone:    " + String(WeatherRoot.TimezoneName));
  WeatherRoot.TimezoneOffset  = root["timezone_offset"];            DEBUG("UTCOffset:   " + String(WeatherRoot.TimezoneOffset));
  setTimeZoneOffset(WeatherRoot.TimezoneOffset);                    DEBUG("TZ UTC:      " + fmtTime.tz());
  WeatherCurrent.UTC          = root["current"]["dt"];              DEBUG("TimeStamp:   " + UTC2ISOtz(WeatherCurrent.UTC));
  WeatherCurrent.Sunrise      = root["current"]["sunrise"];       DEBUGW1("Sunrise:     " + UTC2ISOtz(WeatherCurrent.Sunrise));
  WeatherCurrent.Sunset       = root["current"]["sunset"];        DEBUGW1("Sunset:      " + UTC2ISOtz(WeatherCurrent.Sunset));

  WeatherCurrent.Temperature  = root["current"]["temp"];          DEBUGW1("Temperature: " + String(WeatherCurrent.Temperature));
  WeatherCurrent.FeelsLike    = root["current"]["feels_like"];    DEBUGW1("FeelsLike:   " + String(WeatherCurrent.FeelsLike));
  WeatherCurrent.Pressure     = root["current"]["pressure"];      DEBUGW1("Pressure:    " + String(WeatherCurrent.Pressure));
  WeatherCurrent.Humidity     = root["current"]["humidity"];      DEBUGW1("Humidity:    " + String(WeatherCurrent.Humidity));
  WeatherCurrent.DewPoint     = root["current"]["dew_point"];     DEBUGW1("DewPoint:    " + String(WeatherCurrent.DewPoint));
  WeatherCurrent.UVI          = root["current"]["uvi"];           DEBUGW1("UVI:         " + String(WeatherCurrent.UVI));
  WeatherCurrent.Clouds       = root["current"]["clouds"];        DEBUGW1("Clouds:      " + String(WeatherCurrent.Clouds));

  WeatherCurrent.Visibility   = root["current"]["visibility"];    DEBUGW1("Visibility:  " + String(WeatherCurrent.Visibility));
  WeatherCurrent.WindSpeed    = root["current"]["wind_speed"];    DEBUGW1("WindSpeed:   " + String(WeatherCurrent.WindSpeed));
  WeatherCurrent.WindGust     = root["current"]["wind_gust"];     DEBUGW1("WindGust:    " + String(WeatherCurrent.WindGust));
  WeatherCurrent.WindDir      = root["current"]["wind_deg"];      DEBUGW1("WindDir:     " + String(WeatherCurrent.WindDir));
  WeatherCurrent.Rain1Hour    = root["current"]["rain"]["1h"];    DEBUGW1("Rain1Hour:   " + String(WeatherCurrent.Rain1Hour));
  WeatherCurrent.Snow1Hour    = root["current"]["snow"]["1h"];    DEBUGW1("Snow1Hour:   " + String(WeatherCurrent.Snow1Hour));

  WeatherCurrent.Weather.Id            = root["current"]["weather"][0]["id"];            DEBUGW1("Weather.Id:   " + String(WeatherCurrent.Weather.Id));
  WeatherCurrent.Weather.Main          = root["current"]["weather"][0]["main"].as<String>();        DEBUGW1("Weather.Main: " + String(WeatherCurrent.Weather.Main));
  WeatherCurrent.Weather.Description   = root["current"]["weather"][0]["description"].as<String>(); DEBUGW1("Weather.Text: " + String(WeatherCurrent.Weather.Description));
  WeatherCurrent.Weather.Icon          = root["current"]["weather"][0]["icon"].as<String>();        DEBUGW1("Weather.Icon: " + String(WeatherCurrent.Weather.Icon));

  JsonArray list               = root["minutely"];
  count = list.size();
  DEBUGW2("Weather.Minutely: " +  String(count));
  if (count) {
	  for (int i = 0; i < Cnt_WeatherMinutely; ++i) {
      WeatherMinutely.UTC[i]                = list[i]["dt"];
      WeatherMinutely.Precipitation[i]      = list[i]["precipitation"];
      DEBUGW2("[" + String(i)  + "]: " + UTC2ISO(WeatherMinutely.UTC[i]) + " " +  String(WeatherMinutely.Precipitation[i]) );
	  }
  }
  list  = root["hourly"];
  count = list.size();
  DEBUGW3("Weather.Hourly: " +  String(count));
  if (count) {
    for (int i = 0; i < Cnt_WeatherHourly; ++i) {
      WeatherHourly.UTC[i]          = list[i]["dt"];          DEBUGW3("TimeStamp:   " + UTC2ISO(WeatherHourly.UTC[i]) +  "  " + String(i));
      WeatherHourly.Temperature[i]  = list[i]["temp"];        DEBUGW3("Temperature: " + String(WeatherHourly.Temperature[i]));
      WeatherHourly.FeelsLike[i]    = list[i]["feels_like"];  DEBUGW3("FeelsLike:   " + String(WeatherHourly.FeelsLike[i]));
      WeatherHourly.Pressure[i]     = list[i]["pressure"];    DEBUGW3("Pressure:    " + String(WeatherHourly.Pressure[i]));
      WeatherHourly.Humidity[i]     = list[i]["humidity"];    DEBUGW3("Humidity:    " + String(WeatherHourly.Humidity[i]));
      WeatherHourly.DewPoint[i]     = list[i]["dew_point"];   DEBUGW3("DewPoint:    " + String(WeatherHourly.DewPoint[i]));
      WeatherHourly.UVI[i]          = list[i]["uvi"];         DEBUGW3("UVI:         " + String(WeatherHourly.UVI[i]));
      WeatherHourly.Clouds[i]       = list[i]["clouds"];      DEBUGW3("Clouds:      " + String(WeatherHourly.Clouds[i]));

      WeatherHourly.Visibility[i]   = list[i]["visibility"];  DEBUGW3("Visibility:  " + String(WeatherHourly.Visibility[i]));
      WeatherHourly.Windspeed[i]    = list[i]["wind_speed"];  DEBUGW3("Windspeed:   " + String(WeatherHourly.Windspeed[i]));
      WeatherHourly.WindGust[i]     = list[i]["wind_gust"];   DEBUGW3("WindGust:    " + String(WeatherHourly.WindGust[i]));
      WeatherHourly.WindDir[i]      = list[i]["wind_deg"];    DEBUGW3("WindDir:     " + String(WeatherHourly.WindDir[i]));

      WeatherHourly.Pop[i]          = list[i]["pop"];         DEBUGW3("Pop:         " + String(WeatherHourly.Pop[i]));
      WeatherHourly.Rain1Hour[i]    = list[i]["rain"]["1h"];  DEBUGW3("Rain1Hour:   " + String(WeatherHourly.Rain1Hour[i]));
      WeatherHourly.Snow1Hour[i]    = list[i]["snow"]["1h"];  DEBUGW3("Snow1Hour:   " + String(WeatherHourly.Snow1Hour[i]));

      WeatherHourly.Weather[i].Id           = list[i]["weather"][0]["id"];                       DEBUGW3("Weather.Id:   " + String(WeatherHourly.Weather[i].Id));
      WeatherHourly.Weather[i].Main         = list[i]["weather"][0]["main"].as<String>();        DEBUGW3("Weather.Main: " + String(WeatherHourly.Weather[i].Main));
      WeatherHourly.Weather[i].Description  = list[i]["weather"][0]["description"].as<String>(); DEBUGW3("Weather.Text: " + String(WeatherHourly.Weather[i].Description));
      WeatherHourly.Weather[i].Icon         = list[i]["weather"][0]["icon"].as<String>();        DEBUGW3("Weather.Icon: " + String(WeatherHourly.Weather[i].Icon));
    }
  }
  list  = root["daily"];
  count = list.size();
  DEBUGW4("Weather.Daily: " +  String(count));
  if (count) {
    for (int i = 0; i < Cnt_WeatherDaily; ++i) {
      WeatherDaily.UTC[i]                 = list[i]["dt"];                      DEBUGW4("TimeStamp:        " + UTC2ISO(WeatherDaily.UTC[i]) +  "  " + String(i));
      WeatherDaily.Sunrise[i]             = list[i]["sunrise"];                 DEBUGW4("Sunrise:          " + UTC2ISO(WeatherDaily.Sunrise[i]));
      WeatherDaily.Sunset[i]              = list[i]["sunset"];                  DEBUGW4("Sunset:           " + UTC2ISO(WeatherDaily.Sunset[i]));

      WeatherDaily.Moonrise[i]            = list[i]["moonrise"];                DEBUGW4("Moonrise:         " + UTC2ISO(WeatherDaily.Moonrise[i]));
      WeatherDaily.Moonset[i]             = list[i]["moonset"];                 DEBUGW4("Monset:           " + UTC2ISO(WeatherDaily.Moonset[i]));
      WeatherDaily.MoonPhase[i]           = list[i]["moon_phase"];              DEBUGW4("MoonPhase:        " + String(WeatherDaily.MoonPhase[i]));

      WeatherDaily.TemperatureMorn[i]     = list[i]["temp"]["morn"];            DEBUGW4("TemperatureMorn:  " + String(WeatherDaily.TemperatureMorn[i]));
      WeatherDaily.TemperatureDay[i]      = list[i]["temp"]["day"];             DEBUGW4("TemperatureDay:   " + String(WeatherDaily.TemperatureDay[i]));
      WeatherDaily.TemperatureEve[i]      = list[i]["temp"]["eve"];             DEBUGW4("TemperatureEve:   " + String(WeatherDaily.TemperatureEve[i]));
      WeatherDaily.TemperatureNight[i]    = list[i]["temp"]["night"];           DEBUGW4("TemperatureNight: " + String(WeatherDaily.TemperatureNight[i]));
      WeatherDaily.TemperatureMin[i]      = list[i]["temp"]["min"];             DEBUGW4("TemperatureMin:   " + String(WeatherDaily.TemperatureMin[i]));
      WeatherDaily.TemperatureMax[i]      = list[i]["temp"]["max"];             DEBUGW4("TemperatureMax:   " + String(WeatherDaily.TemperatureMax[i]));

      WeatherDaily.FeelsLikeMorn[i]       = list[i]["feels_like"]["morn"];      DEBUGW4("FeelsLikeMorn:    " + String(WeatherDaily.FeelsLikeMorn[i]));
      WeatherDaily.FeelsLikeDay[i]        = list[i]["feels_like"]["day"];       DEBUGW4("FeelsLikeDay:     " + String(WeatherDaily.FeelsLikeDay[i]));
      WeatherDaily.FeelsLikeEve[i]        = list[i]["feels_like"]["eve"];       DEBUGW4("FeelsLikeEve:     " + String(WeatherDaily.FeelsLikeEve[i]));
      WeatherDaily.FeelsLikeNight[i]      = list[i]["feels_like"]["night"];     DEBUGW4("FeelsLikeNight:   " + String(WeatherDaily.FeelsLikeNight[i]));

      WeatherDaily.Pressure[i]            = list[i]["pressure"];                DEBUGW4("Pressure:         " + String(WeatherDaily.Pressure[i]));
      WeatherDaily.Humidity[i]            = list[i]["humidity"];                DEBUGW4("Humidity:         " + String(WeatherDaily.Humidity[i]));
      WeatherDaily.DewPoint[i]            = list[i]["dew_point"];               DEBUGW4("DewPoint:         " + String(WeatherDaily.DewPoint[i]));

      WeatherDaily.Windspeed[i]           = list[i]["wind_speed"];              DEBUGW4("Windspeed:        " + String(WeatherDaily.Windspeed[i]));
      WeatherDaily.WindGust[i]            = list[i]["wind_gust"];               DEBUGW4("WindGust:         " + String(WeatherDaily.WindGust[i]));
      WeatherDaily.WindDir[i]             = list[i]["wind_deg"];                DEBUGW4("WindDir:          " + String(WeatherDaily.WindDir[i]));

      WeatherDaily.UVI[i]                 = list[i]["uvi"];                     DEBUGW4("UVI:              " + String(WeatherDaily.UVI[i]));
      WeatherDaily.Clouds[i]              = list[i]["clouds"];                  DEBUGW4("Clouds:           " + String(WeatherDaily.Clouds[i]));
      WeatherDaily.Pop[i]                 = list[i]["pop"];                     DEBUGW4("Pop:              " + String(WeatherDaily.Pop[i]));

      WeatherDaily.Rain[i]                = list[i]["rain"]["1h"];              DEBUGW4("Rain:             " + String(WeatherDaily.Rain[i]));
      WeatherDaily.Snow[i]                = list[i]["snow"]["1h"];              DEBUGW4("Snow:             " + String(WeatherDaily.Snow[i]));

      WeatherDaily.Weather[i].Id           = list[i]["weather"][0]["id"];       DEBUGW4("Weather.Id:       " + String(WeatherDaily.Weather[i].Id));
      WeatherDaily.Weather[i].Main         = list[i]["weather"][0]["main"].as<String>();         DEBUGW4("Weather.Main:     " + String(WeatherDaily.Weather[i].Main));
      WeatherDaily.Weather[i].Description  = list[i]["weather"][0]["description"].as<String>();  DEBUGW4("Weather.Text:     " + String(WeatherDaily.Weather[i].Description));
      WeatherDaily.Weather[i].Icon         = list[i]["weather"][0]["icon"].as<String>();         DEBUGW4("Weather.Icon:     " + String(WeatherDaily.Weather[i].Icon));
    }
  }
  #ifdef SERIAL_DEBUG
  DEBUG("DecodeWeatherJson Done " + String(millis() - time0) +  "ms");
  #endif
  result = true;
  return result;
};


class WeatherUrl {
  public:
    enum Units
    {
      standard = 0,
      metric,
      imperial
    };
    const String & Host = _Host;
    int    Port = 80;
    String Key   = "";
    String Lat   = "";
    String Lon   = "";
    String Lang  = "";
    WeatherUrl(String key, String lat, String lon, String lang): Key(key), Lat(lat), Lon(lon), Lang(lang) {
    };
    String getUrl() {
      String url = _Url + "?" + getUrlTail();
      return url;
    }
    String getUrlHistory(int utc) {
      String url = _Url + "/timemachine?dt=" + String(utc) + "&" +getUrlTail();
      return url;
    }
  private:
    const String _Host = "api.openweathermap.org";
    const String _Url  = "/data/2.5/onecall";
    String _Mode  = "json";
    String _Units = "metric";
    String getUrlTail() {
      return "lat=" + Lat + "&lon=" + Lon + "&appid=" + Key + "&mode=" + _Mode + "&units=" + _Units + "&lang=" + Lang;
    }
};


WeatherUrl weatherUrl(OWM_Key, Latitude, Longitude, Language);


class WeatherParser {
  public:
    WeatherParser() {}
    bool parseCurrentAndForecast(DynamicJsonDocument & json){
      return false;
    };
    int parseHistory24h(DynamicJsonDocument & json, int index){
      int result = 0;
      DEBUG(F("WeatherParser.parseHistory:"));
      JsonObject root = json.as<JsonObject>();
      JsonArray list  = root["hourly"];
      int count = list.size();
      int x = index;
      if (count > 0) {
        for (int i = 0; i < count; ++i, ++x) {
          WeatherHistory.UTC[x]          =  list[i]["dt"];
          WeatherHistory.Temperature[x]  =  list[i]["temp"];
          WeatherHistory.Pressure[x]     =  list[i]["pressure"];
          DEBUG_H1("TimeStamp:   " + UTC2ISO(WeatherHistory.UTC[x]) +  "  " + String(x));
          DEBUG_H1("Temperature: " + String(WeatherHistory.Temperature[x]));
          DEBUG_H1("Pressure:    " + String(WeatherHistory.Pressure[x]));
        }
        WeatherHistory.Count += count;
      }

      result = count;
      return result;
    };

  private:

};


WeatherParser weatherParser;

// DODO: Перенести GetWeatherData в метод класса WeatherDownloader
bool GetWeatherData(WiFiClient & client) {
  bool result = false;
  HTTPClient http;
  String url = weatherUrl.getUrl();
  #ifdef SERIAL_DEBUG
  DEBUG("Download Weather: " + url);
  uint32_t time0 = millis();
  #endif

  for (int i = 0; i < HTTPReconnectLimit; ++i) {
    client.stop();
    http.begin(client, weatherUrl.Host, weatherUrl.Port, url);
    int httpCode = http.GET();
    if (httpCode > 0) {
      DEBUG("[HTTP] GET... code: ", String(httpCode) + " " + String(millis() - time0) + "ms");

      if (httpCode == HTTP_CODE_OK) {
        DynamicJsonDocument json(64 * 1024);
        DeserializationError error = deserializeJson(json, http.getStream());
        if (error == DeserializationError::Ok) {
          result = DecodeWeatherJson(json);
          if (result) {
            break;
          } else {
            DEBUG("DecodeWeatherJson - Failed");
          }

        } else {
          DEBUG(F("deserializeJson() failed"), error.c_str());
        }
      }
    } else {
      DEBUG("[HTTP] GET... failed, error", http.errorToString(httpCode));
    }
    http.end();
    delay(HTTPReconnectDelay);
  } // for
  client.stop();
  http.end();
  #ifdef SERIAL_DEBUG
  DEBUG("Download Weather Done  " + String(millis() - time0) + "ms");
  #endif
  return result;
}


class WeatherDownloader {
  public:
  WeatherDownloader() {}
  bool download() {
    bool result;
    WiFiClient client;
    result = getCurrentAndForecast(client);
    if (result) {
      result = getHistory(client);
    }
    return result;
  }
  private:
  // Загрузка текущей погоды и прогноза погоды
  bool getCurrentAndForecast(WiFiClient client) {
    return GetWeatherData(client);
  }
  // Загрузка истории погоды за текуще и предыдущие сутки
  bool getHistory(WiFiClient client) {
    bool result = true;
    int utc = WeatherCurrent.UTC; // (86400UL);
    WeatherHistory.Count = 0;
    // Исторические данные за предыдуще сутки
    getHistory24h(client, utc - (86400UL), 0);
    // Исторические данные за текуще сутки
    getHistory24h(client, utc, 24);
    return result;
  }

  int getHistory24h(WiFiClient client, int utc, int index) {
    int result = 0;
    HTTPClient http;
    String url = weatherUrl.getUrlHistory(utc);
    #ifdef SERIAL_DEBUG
    DEBUG("Download History: " + url);
    uint32_t time0 = millis();
    #endif
    for (int i = 0; i < HTTPReconnectLimit; ++i) {
      client.stop();
      http.begin(client, weatherUrl.Host, weatherUrl.Port, url);
      int httpCode = http.GET();
      if (httpCode > 0) {
        DEBUG("[HTTP] GET... code: ", String(httpCode) + " " + String(millis() - time0) + "ms");
        if (httpCode == HTTP_CODE_OK) {
          DynamicJsonDocument json(64 * 1024);
          DeserializationError error = deserializeJson(json, http.getStream());
          if (error == DeserializationError::Ok) {
            result =  weatherParser.parseHistory24h(json, index);
            break;
          } else {
            DEBUG(F("deserializeJson() failed"), error.c_str());
          }
        }
      } else {
        DEBUG("[HTTP] GET... failed, error", http.errorToString(httpCode));
      }
      http.end();
      delay(HTTPReconnectDelay);
    }
    client.stop();
    http.end();
    #ifdef SERIAL_DEBUG
    DEBUG("Download History Done " + String(millis() - time0) + "ms");
    #endif
    return result;
  }
};

WeatherDownloader weatherDownloader;
