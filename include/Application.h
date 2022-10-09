// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#pragma once

#include <Arduino.h>
#include <time.h>               // In-built
#include "Config.h"
#include "Button2m.h"
#include "Power.h"
#include "Page.h"

const uint8_t  idPageLogo        = 0;
const uint8_t  idPageWeather     = 1;

class Application {
  public:
    Application();
    ~Application();
    void begin();
    Button2m BtnA = Button2m();
    Button2m BtnB = Button2m();
    Button2m BtnC = Button2m();
    #ifdef BUTTON_D
    Button2m BtnD = Button2m();
    #endif
    PageList Pages = PageList(8);
    void startUp();
    bool downloagWeather();
    void update();
    void onButton(Button2m& btn, Button2m::Event_t event);
    bool isAnyButtonDown(bool debounce = true);
    uint32_t getUserInactiveTime();
    uint16_t getDownButtons(bool debounce = true);
    bool checkInactivityTimeout(uint32_t ms);
    bool checkUserActivity();
    void sleep(uint32_t ms);
    void sleep();
    void shutdown();
    void shutdown(uint64_t time_in_us);
    void drawPage();
    void drawPage(int8_t index);
    void resetUserInactiveTime();
    int addPage(Page * page);
    int addPage(Page * page, const String &name, int tag = 0);
    uint32_t InactivityTimeout = ApplicationInactivityTimeout;
    uint32_t LightSleepTimeout = ApplicationLightSleepTimeout;
  protected:
    uint64_t getDeepSleepTime();
  	void updateButtons();
    void initPages();
  private:
    void initButtons();
    uint32_t _UserActiveTimestamp;
};


extern Application application;

extern uint32_t bootCount;

extern uint32_t bootParam;
