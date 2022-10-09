// Weather-Monitor (https://gitflic.ru/project/gembeider/weather-monitor)
// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#include <Arduino.h>         // In-built
#include <Wire.h>            // In-built
#include <time.h>            // In-built
#include "epd_driver.h"      // github.com/Xinyuan-LilyGO/LilyGo-EPD47

#define SERIAL_DEBUG true
// uncomment the following to disable serial debug statements
// #define SERIAL_DEBUG false
#include <SerialDebug.h>     // github.com/rlogiacco/MicroDebug
// Set the following if you wish to override the default
#undef  SERIAL_DEBUG_SEPARATOR
#define SERIAL_DEBUG_SEPARATOR ": "

#include "Config.h"
#include "Rect.h"
#include "Page.h"
#include "Power.h"
#include "Display.h"
#include "Application.h"
#include "Internet.h"
#include "Format.h"
#include "Print.h"

#include "Demo/PageDemoIcons.h"

void setup() {

#ifdef SERIAL_DEBUG
  uint32_t timer0 = millis();
  Serial.begin(115200);
  while (!Serial)
    ;
#endif

  //Increment boot number and print it every reboot
  ++bootCount;
  // Восстановим смещение веремени часового пояса из памяти RTC
  fmtTime.Offset = timeZoneOffset;

#ifdef SERIAL_DEBUG
  print_build(AppTitle);
  print_chip_id();
  DEBUG("Boot number", bootCount);
  print_wakeup_reason(esp_sleep_get_wakeup_cause());
  print_reset_reason_verbose(rtc_get_reset_reason(0), "CPU0 reset reason:");
  // print_reset_reason_verbose(rtc_get_reset_reason(1), "CPU1 reset reason:");
  DEBUG("Battery Voltage", power.getBatteryVoltage());
  DEBUG("Battery Level",   power.getBatteryLevel());
  printLocalTimeISOtz();
#endif

  application.begin();

  #ifdef SERIAL_DEBUG
  // Печать маски нажатых кнопок без подавления дребезга
  print_button_mask(application.getDownButtons(false));
  // Маска может пригодиться, например, для релизации входа в инженерное меню
  // Например, включением или сбросом при нажатой кнопке 'A'
  #endif

  // Сюда в последствии перенесется основной код
  application.startUp();
  // Страница демонстрации иконок
  application.addPage(new PageDemoIcons(), "DemoIcons");

  if (bootCount == 1) {
    // Отображаем страницу Logo (About)
    application.drawPage(idPageLogo);
  }

  // Если включение питания или пробуждение по таймеру
  if ((bootCount == 1) | (power.isWakeupByTimer())) {
    // Загружаем и отображаем погоду
    if (application.downloagWeather()){
      application.drawPage(idPageWeather);
    } else {
      DEBUG("Failure dowload Weather Data");
      // Ошибка получения данных
      // Запланируем следующую попытку через заданное время
      application.shutdown(OnFailureRepeatDelay * 1000LL);
      return; // Выход из глубокого сна только через reset (кнопкой или таймером)
    }
  }

  DEBUG("Setup done " + String(millis() - timer0) + "ms");

  // Если было пробуждение по таймеру, ничего дополнительно делать не надо,
  // просто уходим в глубокий сон до следующего сеанса связи
  if (power.isWakeupByTimer()) {
    DEBUG("WakeupByTimer. All Done. Shutdown");
    // Погружение в глубокий сон до следующего сеанса загрузки погоды
    application.shutdown();
    return; // Выход из глубокого сна только через reset (кнопкой или таймером)
  }

  // Сброс для начала остчета времени неактивности пользовалеля
  application.resetUserInactiveTime();

} // setup done


// В этот цикл попадаем только если было пробуждение по Кнопкам или Сбросу
void loop() {

  // Обрабатываем нажатие на кнопок
  application.update();
  // Проверяем активность пользователя
  if (application.checkUserActivity()) {
    // Пользователь что-то нажимает, для экономии энегрии временно перейденм в
    // короткий легкий сон LightSleepTimeout
    // Пока не получается сделать пробуждение по нажатию кнопок :-(
    application.sleep();
  } else {
    // Пользователь не нажимал кнопки в течении заданного таймаута, поэтому
    // погружение в глубокий сон до следующего сеанса загрузки погоды
    DEBUG("Inactivity Timeout", application.getUserInactiveTime());
    application.shutdown();
    return; // Выход из глубокого сна только через reset (кнопкой или таймером)
  };

}
