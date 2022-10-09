// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#pragma once

#define MI_SNOW                  0
#define MI_RAIN                  1
#define MI_DROP                  2
#define MI_CLOUD                 4
#define MI_CLOUDS                5
#define MI_VISIBILITY            8
#define MI_HUMIDITY              3
#define MI_UMBELLA              14 //22 //14
#define MI_PRESSURE              6
#define MI_ALIEN                 9
#define MI_MAX                  30  //12  // 10 12 28
#define MI_MIN                  31  //13  // 11 30 31
#define MI_UP                   10  // 26
#define MI_DOWN                 11  // 27
#define MI_FLAT                 12  // 16
#define MI_MOON                 15
#define MI_TEMPERATURE           7
#define MI_SUNRISE              24
#define MI_SUNSET               25
#define MI_UVI                  19
#define MI_DEVPOINT              2
#define MI_MOONRISE             22
#define MI_MOONSET              23



#define WI_CLEAR_D               0
#define WI_CLEAR_N               1
#define WI_CLOUDS_D              2
#define WI_CLOUDS_N              3
#define WI_SCATTERED_CLOUDS_D    4
#define WI_SCATTERED_CLOUDS_N    5
#define WI_BROKEN_CLOUDS_D       6
#define WI_BROKEN_CLOUDS_N       7
#define WI_SNOWER_D              8
#define WI_SNOWER_N              9
#define WI_RAIN_D                10
#define WI_RAIN_N                11
#define WI_STORM_D               12//10
#define WI_STORM_N               13//11
#define WI_SNOW_D                14//10
#define WI_SNOW_N                15//11
#define WI_MIST_D                16
#define WI_MIST_N                17


#define IW_CLEAR        1
#define IW_CLOUD10      2
#define IW_CLOUD25      3
#define IW_CLOUD85      4
#define IW_SNOWER	      5
#define IW_RAIN         6
#define IW_STORM        7
#define IW_SNOW         8
#define IW_MIST         9

#include "Icon.h"

extern Icon  iconMeasure;
extern Icon  iconWeather;

int getIconId(int weatherId, String iconName);
