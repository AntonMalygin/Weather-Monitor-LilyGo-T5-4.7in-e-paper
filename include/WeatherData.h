// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#pragma once

#include <Arduino.h>
#include <SerialDebug.h>

#define Cnt_WeatherMinutely  61
#define Cnt_WeatherHourly    48
#define Cnt_WeatherDaily      8
#define Cnt_WeatherForecast  24
#define Cnt_WeatherHistory   48

typedef struct {
  int         Id;                                   // Идентификатор погодных условий
  String      Main;                                 // Группа параметров погоды (дождь, снег, экстрим и т.д.)
  String      Description;                          // Погодные условия внутри группы
  String      Icon;                                 // Идентификатор значка погоды
} WeatherItem_t;

typedef struct {
  int         UTC[Cnt_WeatherMinutely];             // Время прогнозируемых данных, unix, UTC
  float       Precipitation[Cnt_WeatherMinutely];   // Объем осадков, мм
} WeatherMinutely_t;


typedef struct {
  int         UTC[Cnt_WeatherHourly];                // Время прогнозируемых данных, unix, UTC
  float       Temperature[Cnt_WeatherHourly];        // Температура
  float       FeelsLike[Cnt_WeatherHourly];          // Этот температурный параметр объясняет восприятие человеком погоды
  float       Pressure[Cnt_WeatherHourly];           // Атмосферное давление на уровне моря, hPa
  float       Humidity[Cnt_WeatherHourly];           // Влажность, %
  float       DewPoint[Cnt_WeatherHourly];           // Температура воздуха (в зависимости от давления и влажности), ниже которой начинают конденсироваться капли воды и может образовываться роса
  float       UVI[Cnt_WeatherHourly];                // Текущий УФ-индекс
  float       Clouds[Cnt_WeatherHourly];             // Облачность, %
  int         Visibility[Cnt_WeatherHourly];         // Средняя видимость, метры
  float       Windspeed[Cnt_WeatherHourly];          // Скорость ветра  метр/сек
  float       WindGust[Cnt_WeatherHourly];           // Порыв ветра
  float       WindDir[Cnt_WeatherHourly];            // Направление ветра, градусы (метеорологические)
  float       Pop[Cnt_WeatherHourly];                // Вероятность осадков
  float       Rain1Hour[Cnt_WeatherHourly];          // Объем осадков за последний час, мм
  float       Snow1Hour[Cnt_WeatherHourly];          // Объем осадков за последний час, мм
  WeatherItem_t Weather[Cnt_WeatherHourly];
} WeatherHourly_t;

typedef struct {
  int         UTC[Cnt_WeatherDaily];
  int         Sunrise[Cnt_WeatherDaily];            // Время восхода солнца, Unix, UTC
  int         Sunset[Cnt_WeatherDaily];             // Время заката, Unix, UTC
  int         Moonrise[Cnt_WeatherDaily];           // Время восхода луны
  int         Moonset[Cnt_WeatherDaily];            // Время заката луны
  float       MoonPhase[Cnt_WeatherDaily];          // Фаза луны 0..1.0

  float       TemperatureMorn[Cnt_WeatherDaily];    // Утренняя температура.
  float       TemperatureDay[Cnt_WeatherDaily];     // Температура днем.
  float       TemperatureEve[Cnt_WeatherDaily];     // Вечерняя температура.
  float       TemperatureNight[Cnt_WeatherDaily];   // Ночная температура.
  float       TemperatureMin[Cnt_WeatherDaily];     // Мин дневная температура.
  float       TemperatureMax[Cnt_WeatherDaily];     // Максимальная дневная температура.

  float       FeelsLikeMorn[Cnt_WeatherDaily];      // Утренняя температура.
  float       FeelsLikeDay[Cnt_WeatherDaily];       // Температура днем.
  float       FeelsLikeEve[Cnt_WeatherDaily];       // Вечерняя температура.
  float       FeelsLikeNight[Cnt_WeatherDaily];     // Ночная температура.

  float       Pressure[Cnt_WeatherDaily];           // Атмосферное давление на уровне моря, hPa
  float       Humidity[Cnt_WeatherDaily];           // Влажность, %
  float       DewPoint[Cnt_WeatherDaily];           // Температура воздуха (в зависимости от давления и влажности), ниже которой начинают конденсироваться капли воды и может образовываться роса

  float       Windspeed[Cnt_WeatherDaily];          // Скорость ветра  метр/сек
  float       WindGust[Cnt_WeatherDaily];           // Порыв ветра
  float       WindDir[Cnt_WeatherDaily];            // Направление ветра, градусы (метеорологические)

  float       Clouds[Cnt_WeatherDaily];             // Облачность, %
  float       UVI[Cnt_WeatherDaily];                // Текущий УФ-индекс
  float       Pop[Cnt_WeatherDaily];                // Вероятность осадков
  float       Rain[Cnt_WeatherDaily];               // Объем осадков, мм
  float       Snow[Cnt_WeatherDaily];               // Объем осадков, мм
  WeatherItem_t Weather[Cnt_WeatherDaily];
} WeatherDaily_t;

typedef struct {
  String  SenderName;           // Название источника оповещения.
  String    Event;              // Имя события оповещения
  int       Start;              // Дата и время начала оповещения, Unix, UTC
  int       End;                // Дата и время окончания оповещения, Unix, UTC
  String    Description;        // Описание оповещения
} WeatherAlert_t;

typedef struct {
  int       UTC;                // Текущее время, Unix, UTC
  int       Sunrise;            // Время восхода солнца, Unix, UTC
  int       Sunset;             // Время заката, Unix, UTC
  float     Temperature;        // Температура
  float     FeelsLike;          // Этот температурный параметр объясняет восприятие человеком погоды
  float     Pressure;           // Атмосферное давление на уровне моря, hPa
  float     Humidity;           // Влажность, %
  float     DewPoint;           // Температура воздуха (в зависимости от давления и влажности), ниже которой начинают конденсироваться капли воды и может образовываться роса
  float     Clouds;             // Облачность, %
  float     UVI;                // Текущий УФ-индекс
  int       Visibility;         // Средняя видимость, метры
  float     WindSpeed;          // Скорость ветра  метр/сек
  float     WindGust;           // Порыв ветра
  float     WindDir;            // Направление ветра, градусы (метеорологические)
  float     Rain1Hour;          // Объем осадков за последний час, мм
  float     Snow1Hour;          // Объем осадков за последний час, мм
  WeatherItem_t Weather;
} WeatherCurrent_t;


typedef struct {                // One Call API
  float   Lat;                  // Географические координаты местоположения (широта)
  float   Lon;                  // Географические координаты местоположения (долгота)
  String  TimezoneName;         // Название тайм-зоны
  int     TimezoneOffset;       // Сдвиг в секундах от UTC
}  WeatherRoot_t;


typedef struct {
  int         Count;
  int         UTC[Cnt_WeatherHistory];             // Время исторических данных, Unix, UTC
  float       Temperature[Cnt_WeatherHistory];     // Температура
  float       Pressure[Cnt_WeatherHistory];        // Атмосферное давление на уровне моря, hPa
}  WeatherHistory_t;

typedef struct {
  String      Cod;
  int         Message;
  int         Count;
  int         UTC[Cnt_WeatherForecast];
  float       Temperature[Cnt_WeatherForecast];     // Температура
  float       FeelsLike[Cnt_WeatherForecast];       // Этот температурный параметр объясняет восприятие человеком погоды
  float       TemperatureMin[Cnt_WeatherForecast];  // Мин дневная температура.
  float       TemperatureMax[Cnt_WeatherForecast];  // Максимальная температура.

  float       Pressure[Cnt_WeatherForecast];        // Атмосферное давление на уровне моря, hPa
  float       Humidity[Cnt_WeatherForecast];        // Влажность, %

  float       Windspeed[Cnt_WeatherForecast];       // Скорость ветра  метр/сек
  float       WindDir[Cnt_WeatherForecast];         // Направление ветра, градусы (метеорологические)
  int         Visibility[Cnt_WeatherForecast];      // Средняя видимость, метры

  float       Clouds[Cnt_WeatherForecast];          // Облачность, %
  float       Pop[Cnt_WeatherForecast];             // Вероятность осадков
  float       Rain3Hour[Cnt_WeatherForecast];       // Объем осадков за последние 3 часа, мм
  float       Snow3Hour[Cnt_WeatherForecast];       // Объем осадков за последние 3 часа, мм
  String      SysPod[Cnt_WeatherForecast];          //  Часть дня (n - ночь, d - день)
  WeatherItem_t Weather[Cnt_WeatherForecast];
  String      UTCTxt[Cnt_WeatherForecast];
  int         CityId;
  String      CityName;
  String      Country;
  float       Lat;                                  // Географические координаты местоположения (широта)
  float       Lon;                                  // Географические координаты местоположения (долгота)
  int         Timezone;                             // Сдвиг в секундах от UTC
  int         Sunrise;                              // Время восхода солнца, Unix, UTC
  int         Sunset;                               // Время заката, Unix, UTC
} WeatherForecast_t;




extern WeatherRoot_t         WeatherRoot;
extern WeatherCurrent_t      WeatherCurrent;
extern WeatherHourly_t       WeatherHourly;
extern WeatherMinutely_t     WeatherMinutely;
extern WeatherDaily_t        WeatherDaily;
extern WeatherAlert_t        WeatherAlert;
extern WeatherHistory_t      WeatherHistory;

float getMean(float * values, int start, int count);
float getMean(int * values, int start, int count);
float getMin(int * values, int start, int count);
float getMin(float * values, int start, int count);
float getMax(float * values, int start, int count);
float getSum(float * values, int start, int count);
// Разность средней температуры за предыдущие и прогнозные 24 часа
float getDeltaTemperature();
// Разность текщего атмосферного давления по сравнению со среднем за предыдущеи 24 часа
float getDeltaPressure(int hour = 24);
// Выбираем из массива наиболее часто встречающуюся погоду
int selectWeatherItem(WeatherItem_t * Weather, int start, int count);
