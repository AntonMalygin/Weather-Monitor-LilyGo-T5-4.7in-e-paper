// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#include <Arduino.h>

#include "Icon.h"
#include "Icons.h"

#include "Icons/iml_24x24x4.h"
#include "Icons/iml_64x64x4.h"

Icon  iconMeasure(24, 24, 32, (uint8_t *)iml_24x24x4);
Icon  iconWeather(64, 64, 32, (uint8_t *)iml_64x64x4);


// Weather condition codes
// https://openweathermap.org/weather-conditions#Weather-Condition-Codes-2

const int16_t IID_Weather [][2] = {
  {	200	,	IW_STORM	},  //	гроза с легким дождем
  {	201	,	IW_STORM	},  //	гроза с дождем
  {	202	,	IW_STORM	},  //	гроза с проливным дождем
  {	210	,	IW_STORM	},  //	легкая гроза
  {	211	,	IW_STORM	},  //	гроза
  {	212	,	IW_STORM	},  //	тяжелая гроза
  {	221	,	IW_STORM	},  //	оборванный гроза
  {	230	,	IW_STORM	},  //	гроза с легким моросить
  {	231	,	IW_STORM	},  //	гроза с дождем
  {	232	,	IW_STORM	},  //	гроза с сильным моросить

  {	300	,	IW_MIST	},  //	светоемкость моросить
  {	301	,	IW_MIST	},  //	моросить
  {	302	,	IW_MIST	},  //	тяжелая интенсивность моросить
  {	310	,	IW_MIST	},  //	легкая интенсивность моросить  дождь
  {	311	,	IW_RAIN	},  //	моросить дождь
  {	312	,	IW_RAIN	},  //	тяжелая интенсивность моросить  дождь
  {	313	,	IW_RAIN	},  //	дождь и дождь
  {	314	,	IW_RAIN	},  //	сильный дождь и дождь
  {	321	,	IW_RAIN	},  //	душ дождь

  {	500	,	IW_RAIN	},  //	небольшой дождь
  {	501	,	IW_RAIN	},  //	умеренный дождь
  {	502	,	IW_RAIN	},  //	сильный дождь интенсивности
  {	503	,	IW_RAIN	},  //	очень сильный дождь
  {	504	,	IW_SNOWER	},  //	экстремальный дождь
  {	511	,	IW_SNOW	},  //	ледяной дождь
  {	520	,	IW_SNOWER	},  //	свет интенсивности дождь дождь
  {	521	,	IW_SNOWER	},  //	дождь ливень
  {	522	,	IW_SNOWER	},  //	тяжелая интенсивность дождя дождь
  {	531	,	IW_SNOWER	},  //	оборванный дождь душ

  {	600	,	IW_SNOW	},  //	легкий снег
  {	601	,	IW_SNOW	},  //	снег
  {	602	,	IW_SNOW	},  //	Сильный снег
  {	611	,	IW_SNOW	},  //	крупа
  {	612	,	IW_SNOW	},  //	Легкий душ мокрый снег
  {	613	,	IW_SNOW	},  //	Душ мокрый снег
  {	615	,	IW_SNOW	},  //	Легкий дождь и снег
  {	616	,	IW_SNOW	},  //	Дождь и снег
  {	620	,	IW_SNOW	},  //	Легкий душ снег
  {	621	,	IW_SNOW	},  //	Душ снег
  {	622	,	IW_SNOW	},  //	Сильный ливень снег

  {	701	,	IW_MIST	},  //	туман
  {	711	,	IW_MIST	},  //	дым
  {	721	,	IW_MIST	},  //	мгла
  {	731	,	IW_MIST	},  //	песок / пыль кружится
  {	741	,	IW_MIST	},  //	туман
  {	751	,	IW_MIST	},  //	песок
  {	761	,	IW_MIST	},  //	пыль
  {	762	,	IW_MIST	},  //	вулканический пепел
  {	771	,	IW_MIST	},  //	шквалы
  {	781	,	IW_MIST	},  //	торнадо

  {	800	,	IW_CLEAR    },  //	ясное небо
  {	801	,	IW_CLOUD10	},  //	мало облаков: 11-25%
  {	802	,	IW_CLOUD25	},  //	рассеянные облака: 25-50%
  {	803	,	IW_CLOUD85	},  //	разбитые облака: 51-84%
  {	804	,	IW_CLOUD85  },  //	пасмурная облачность: 85-100%
  { 0 , 0  }
};

typedef struct {
  const int16_t weather;
  const int16_t icon[2];
} WeatherIconId_t;

const WeatherIconId_t IID_WeatherIcon [] = {
  { IW_CLEAR    , { WI_CLEAR_D  ,WI_CLEAR_N} },
  { IW_CLOUD10  , { WI_CLOUDS_D ,WI_CLOUDS_N } },
  { IW_CLOUD25  , { WI_SCATTERED_CLOUDS_D, WI_SCATTERED_CLOUDS_N } },
  { IW_CLOUD85  , { WI_BROKEN_CLOUDS_D , WI_BROKEN_CLOUDS_N } },
  { IW_SNOWER   , { WI_SNOWER_D, WI_SNOWER_N} },
  { IW_RAIN     , { WI_RAIN_D , WI_RAIN_N } },
  { IW_STORM    , { WI_STORM_D ,WI_STORM_N } },
  { IW_SNOW     , { WI_SNOW_D , WI_SNOW_N } },
  { IW_MIST     , { WI_MIST_D , WI_MIST_N } },
  { 0           , {0 ,0 } }
};



int getIconId(int weatherId, String iconName) {
  int IconId  = -1;
  int Weather = -1;
  int Night = 0;
  if (iconName.length() == 3) {
    if (iconName.endsWith("n")) {
      Night = 1;
    }
  }
  for (int i = 0; IID_Weather[i][0] > 0; ++i) {
    if (IID_Weather[i][0] == weatherId) {
      Weather = IID_Weather[i][1];
      break;
    }
  }
  if (Weather > 0) {
    for (int i = 0; IID_WeatherIcon[i].weather > 0; ++i) {
      if (IID_WeatherIcon[i].weather == Weather) {
        IconId = IID_WeatherIcon[i].icon[Night];
        break;
      }
    }
  }

  // Serial.printf("WeaterId %d IconName %s iconId %d\n", weatherId, iconName, IconId);

  return IconId;
}
