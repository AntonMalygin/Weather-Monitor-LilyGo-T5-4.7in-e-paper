// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#include "Application.h"
#include "epd_driver.h"         // https://github.com/Xinyuan-LilyGO/LilyGo-EPD47

#define SERIAL_DEBUG true
// uncomment the following to disable serial debug statements
// #define SERIAL_DEBUG false
#include <SerialDebug.h>
#include "Print.h"
#include "Display.h"

#include "PageLogo.h"
#include "PageWeather.h"
#include "WeatherParser.h"

// Сетчик перезагрузок в энергонезависимой памяти
RTC_DATA_ATTR uint32_t bootCount = 0;
// Параметр перезагрузки в энергонезависимой памяти
RTC_DATA_ATTR uint32_t bootParam = 0;

Application application;

// void buttonEvent(Button2m& btn, Button2m::Event_t event);
void buttonEvent(Button2m& btn, Button2m::Event_t event) {
  #ifdef SERIAL_DEBUG
  // print_button_event(btn, event);
  #endif
  application.onButton(btn, event);
};

Application::Application() {

};

Application::~Application() {

};

void Application::begin() {
  // Инициализируем самое необходимое
	initButtons();
	resetUserInactiveTime();
};

void Application::startUp() {
  // Разрешаем будить контроллер по кнопке BtnA
  power.setWakeupPin(application.BtnA.getPin());
  // Инициализируем дисплей и страницы
  initPages();

};


bool Application::downloagWeather() {
  bool success = false;
  if (internet.connect()) {
    // Синхронизация времени из интернета
    internet.synchronizeTime();
    #ifdef SERIAL_DEBUG
    printLocalTimeISOtz();
    #endif
    // Загрузка погоды
    success = weatherDownloader.download();
    // При получении погоды будут установлены параметры часового пояса:
    //   WeatherRoot.TimezoneName
    //   WeatherRoot.TimezoneOffset
    // Смещение часового пояса будет записано в память RTC
  }
  internet.disconnect();
  return success;
};

void Application::initButtons() {
  #ifdef BUTTON_D
  BtnD.begin(0, INPUT_PULLDOWN, false, false);
  BtnD.Tag = 0;
  BtnD.setCallback(buttonEvent);
  #endif
  BtnA.begin(BUTTON_A_PIN, buttonEvent, BUTTON_A_TAG);
  BtnB.begin(BUTTON_B_PIN, buttonEvent, BUTTON_B_TAG);
  BtnC.begin(BUTTON_C_PIN, buttonEvent, BUTTON_C_TAG);
};
void Application::initPages() {
  display.begin();
  // Добавляем страницы для отображения
  addPage(new PageLogo(), "Logo", idPageLogo); // initPageLogo();
  addPage(new PageWeather(), "Weather", idPageWeather); //initPageWeather();
  //  addPage(initPageDemo());
  //  addPage(initPageFonts());

  #ifdef SERIAL_DEBUG
  DEBUG("Done PageCount: ", Pages.count());
  #endif

};
void Application::drawPage() {
  #ifdef SERIAL_DEBUG
    uint32_t time0 = millis();
    String pageId = "";
    Page * page = Pages.activePage();
    if (page)
     pageId = page->Id;
  #endif
  display.powerOn();
  display.clearScreen();
  display.clearBuffer();
  #ifdef SERIAL_DEBUG
    Serial.println("clear Screen " + String(millis() - time0) + "ms");
  #endif
  Pages.draw();

  display.update();
  display.powerOff();

  #ifdef SERIAL_DEBUG
    time0 = millis() - time0;
    Serial.print("draw Page: #" + String(Pages.activeIndex()) + " Id: '" + pageId + "' ");
    Serial.println(String(time0) + "ms");
  #endif
}
void Application::drawPage(int8_t index){
  Pages.activeIndex(index);
  drawPage();
}

void Application::onButton(Button2m& btn, Button2m::Event_t event) {
  resetUserInactiveTime();
  uint16_t mask = getDownButtons();
  #ifdef SERIAL_DEBUG
  // DEBUG("onButton");
  print_button_event(btn, event);
  // print_button_mask(mask);
  #endif

  // ToDo: Почему для BtnA не работает событя EventLongPress и EventLong?
  if ((btn.Tag == BUTTON_A_TAG) && ( event == Button2m::EventLong)) {
    DEBUG("Button A Long!!!");
  }
  // Загрузка и отображение погоды по длинному ~2с клику кнопки А
  if ((btn.Tag == BUTTON_A_TAG) && ( event == Button2m::EventReleased)) {
    unsigned int time = btn.getPressedTime();
    if (time > 2000) {
      DEBUG("Button A Long press " + String(time) + "ms");
      if (downloagWeather()) {
        drawPage(idPageWeather);
      }
    }
    return;
  }

  if ((btn.Tag == BUTTON_C_TAG) && ( event == Button2m::EventDouble)) {
    Pages.nextPage();
    DEBUG("nextPage", Pages.activeIndex());
    drawPage();
    return;
  }

  Pages.onButton(btn, event, mask);
};


bool Application::isAnyButtonDown(bool debounce) {
	if (BtnA.isPressed(debounce)) return true;
	if (BtnB.isPressed(debounce)) return true;
	if (BtnC.isPressed(debounce)) return true;
  return false;
}

uint32_t Application::getUserInactiveTime() {
  return millis() - _UserActiveTimestamp;
};

bool Application::checkInactivityTimeout(uint32_t ms) {
  return getUserInactiveTime() > ms;
};

bool Application::checkUserActivity() {
  return not checkInactivityTimeout(InactivityTimeout);
}

uint16_t Application::getDownButtons(bool debounce) {
	uint16_t result = 0;
	if (BtnA.isPressed(debounce)) result = result | BUTTON_A_MASK;
	if (BtnB.isPressed(debounce)) result = result | BUTTON_B_MASK;
	if (BtnC.isPressed(debounce)) result = result | BUTTON_C_MASK;
  return result;
};

void Application::resetUserInactiveTime() {
  _UserActiveTimestamp = millis();
};

int Application::addPage(Page * page) {
  if (page != NULL) {
    int index = Pages.addPage(page);
    #ifdef SERIAL_DEBUG
    Serial.println("addPage: '" + page->Id + "' Tag:" +  String(page->Tag) + " #" + String(index));
    #endif
    return index;
  } else {
    return -1;
  }
}
int Application::addPage(Page * page, const String &name, int tag) {
  if (page != NULL) {
    page->Id = name;
    page->Tag = tag;
    return addPage(page);
  } else {
    return -1;
  }
}


void Application::update() {
  updateButtons();
};

void Application::updateButtons() {
  #ifdef BUTTON_D
  BtnD.loop();
  #endif
  BtnA.loop();
  BtnB.loop();
  BtnC.loop();
};

void Application::sleep(uint32_t ms) {
  uint64_t time = SLEEP_MSEC(ms);
  power.lightSleep(time);
};

void Application::sleep(){
  sleep(LightSleepTimeout);
};


uint64_t Application::getDeepSleepTime() {
  time_t now;
  struct tm timeinfo;
  time(&now);
  localtime_r(&now, &timeinfo);
  // Вычислаем время следующего сеанса получения прогноза погоды
  uint32_t sleepTime = (GetWeatherForecastPeriodMin * 60 - ((timeinfo.tm_min % GetWeatherForecastPeriodMin) * 60 + timeinfo.tm_sec));
  // Добавляем небольшую задержку
  sleepTime  += GetWeatherForecastDelaySec; //Some ESP32 have a RTC that is too fast to maintain accurate time, so add an offset
  DEBUG("WakeUp time", fmtTime.isotz(now + sleepTime));
  // Преобразуем секунды в наносекунды
  return SLEEP_SEC(sleepTime);
};

void Application::shutdown() {
  uint64_t time_in_us = getDeepSleepTime();
  shutdown(time_in_us);
};

void Application::shutdown(uint64_t time_in_us){
	#ifdef SERIAL_DEBUG
	Serial.println("deepSleep: "  + String(uint32_t(time_in_us / 1000)) + " ms");
	Serial.flush();
	#endif
	//It will turn off the power of the entire
  // POWER_EN control and also turn off the blue LED light
  epd_poweroff_all();
  power.deepSleep(time_in_us);
}
