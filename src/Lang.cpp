// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#include "Lang.h"
#include "Arduino.h"

#include "Config.h"

#ifdef LANG_RU
  #include "Lang/Lang_ru.h"
#endif

#ifdef LANG_EN
  #include "Lang/Lang_en.h"
#endif
