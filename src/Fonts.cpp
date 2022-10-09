// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#include "Display.h"
#include "Config.h"

#ifdef LANG_RU
#include "Fonts/NotoSansCond7r-ru.h"
#include "Fonts/OpenSans8-en-ru.h"
#endif

#include "Fonts/OpenSans10s-digital.h"
#include "Fonts/OpenSans14s-digital.h"
#include "Fonts/OpenSans16s-digital.h"
#include "Fonts/OpenSans36s-digital.h"

#define Font1 OpenSans8
#define Font2 NotoSansCond7r
#define Font3 OpenSans10s
#define Font4 OpenSans14sd
#define Font5 OpenSans16sd
#define Font6 OpenSans36s

Font font1(Font1, "OpenSans8");
Font font2(Font2, "NotoSansCond7");
Font font3(Font3, "OpenSans10s");
Font font4(Font4, "OpenSans14s");
Font font5(Font5, "OpenSans16s");
Font font6(Font6, "OpenSans36s");
