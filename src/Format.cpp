// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#include "Format.h"

#include "Config.h"
#include "Lang.h"


Format fmtPercent(S_Unit_Percent, 0, 1.0);
Format fmtTemperature(S_Unit_Degree, 0, 1.0, 0, true);
Format fmtTemperatureChart(S_Unit_DegreeCelsius, 1, 1.0, 5.001);
Format fmtPrecipitation(S_Unit_mm, 1, 1.0, 5.0);
Format fmtPrecipitationInHour(S_Unit_mmh, 1, 1.0, 5.0);
Format fmtHumidity(S_Unit_Percent, 0);
Format fmtWindSpeed(S_Unit_kmh, 0, 3.59);
Format fmtPressure(S_Unit_hPa, 0);
Format fmtVisibility(S_Unit_km, 1, 0.001, 2000);
Format fmtMoonAge(S_Unit_Days, 0);
Format fmtRSSI(S_Unit_dBm, 0);
FormatTime fmtTime;
FormatMoon fmtMoon(S_Unit_Days);
