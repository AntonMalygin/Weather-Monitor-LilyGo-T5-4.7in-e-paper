// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#pragma once

#define SERIAL_DEBUG true

#define AppTitle "Weather-Monitor"

// Buttons

#define BUTTON_A_TAG (1)
#define BUTTON_B_TAG (2)
#define BUTTON_C_TAG (3)

#define BUTTON_A_MASK 1
#define BUTTON_B_MASK 2
#define BUTTON_C_MASK 4


#define BUTTON_A_PIN (39)
#define BUTTON_B_PIN (34)
#define BUTTON_C_PIN (35)

// #define BUTTON_D

#ifdef BUTTON_D
  #define BUTTON_D_PIN  0
  #define BUTTON_D_TAG  8
#endif

#define BATT_PIN         36

// Содержимое сторк в таблице прогноза погоды
#define TableRowTime            0
#define TableRowWeather         1
#define TableRowPrecipitation   2
#define TableRowTemperature     3
#define TableRowWindSpeed       4
#define TableRowClouds          5
#define TableRowVisibility      6
#define TableRowHumidity        7

// Высота некотрых строк в таблице прогоноза породы
#define TableRowHeightTime                20
#define TableRowHeightWeather             78
#define TableRowHeightLine1               24
#define TableRowHeightLine2               38

// Пороги при котрых значения в таблице отображаются серым цветом
#define TableThresholdPrecipitation       1.5
#define TableThresholdProbability         25
#define TableThresholdHumidityLow         30
#define TableThresholdHumidityHigh        70
// За сколько часов назад определяется тенденция изменения давления
#define PRESSURE_TREND_HISTRORY_HOUR      8

#define LANG_RU
// #define LANG_EN


// Период получения прогноза погоды в минутах
#define GetWeatherForecastPeriodMin   		30
// Задежка в секундах перез получением прогноза
#define GetWeatherForecastDelaySec    		30
// Тайм-аут неактивности пользователя
// после которого устройство засыпает в миллисеундах
#define ApplicationInactivityTimeout            30000
// Время легкого сна приожения в цикле в миллисеундах
#define ApplicationLightSleepTimeout               10
// Время сна между неудачными попытками соединения с интернетом
#define OnFailureRepeatDelay                    60000

// Лимит попыток подключения к WiFi в случае неудачи
#define WiFiReconnectLimit   10
// Задержка в миллисекундах между попытками подключения к WiFi
#define WiFiReconnectDelay   500

// Демонстроационные данные о погоде
// #define DEMO_WEATHER

// IP адресс DNS (Google)
#define DNS_IP 8,8,8,8

// Источники точного времени
#define NtpServer1  "pool.ntp.org"
#define NtpServer2  "ntp4.stratum2.ru"
#define NtpServer3  "time.nist.gov"

// Лимит попыток запросов по протоколу HTTP в случае неудачи
#define HTTPReconnectLimit   4
// Задержка в миллисекундах между повторными запросами по HTTP
#define HTTPReconnectDelay   1000
