
#include "Print.h"

#include <Arduino.h>
#include <rom/rtc.h>
#include "Button2m.h"


void print_build(const String &title) {
  if (not title.isEmpty())
    Serial.println("-=-=-=-=-=-=-=-= " + title + " =-=-=-=-=-=-=-=-");
  // Serial.println(String(__FILE__) + "\nStarting...");
  Serial.print(F("compiled: "));
  Serial.print(__DATE__);
  Serial.print(F(" "));
  Serial.println(__TIME__);
}

void print_chip_id(void) {
  uint32_t chipId = 0;
  for (int i = 0; i < 17; i = i + 8) {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  Serial.printf("ESP32 Chip model = %s Rev %d\n", ESP.getChipModel(), ESP.getChipRevision());
  Serial.printf("This chip has %d cores\n", ESP.getChipCores());
  Serial.print("Chip ID: ");
  Serial.println(chipId);
}

/*
Method to print the reason by which ESP32
has been awaken from sleep
*/
void print_wakeup_reason(esp_sleep_wakeup_cause_t wakeup_reason) {
  //esp_sleep_wakeup_cause_t wakeup_reason;

  //wakeup_reason = esp_sleep_get_wakeup_cause();
  Serial.print("Wakeup caused 0x0");
  Serial.print(wakeup_reason, HEX);
  Serial.print(" ");
  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0: Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1: Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER: Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD: Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP: Serial.println("Wakeup caused by ULP program"); break;
    case ESP_SLEEP_WAKEUP_GPIO: Serial.println("Wakeup caused by GPIO "); break;  // (light sleep only)
    case ESP_SLEEP_WAKEUP_UART: Serial.println("Wakeup caused by UART "); break;  // (light sleep only)
    default: Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
  }
}

/*
  Serial.println("CPU0 reset reason:");
  print_reset_reason(rtc_get_reset_reason(0));
  verbose_print_reset_reason(rtc_get_reset_reason(0));

  Serial.println("CPU1 reset reason:");
  print_reset_reason(rtc_get_reset_reason(1));
  verbose_print_reset_reason(rtc_get_reset_reason(1));
*/

void print_reset_reason(RESET_REASON reason) {
  switch (reason) {
    case 1: Serial.println("POWERON_RESET"); break;            // <1,  Vbat power on reset
    case 3: Serial.println("SW_RESET"); break;                 // <3,  Software reset digital core
    case 4: Serial.println("OWDT_RESET"); break;               // <4,  Legacy watch dog reset digital core
    case 5: Serial.println("DEEPSLEEP_RESET"); break;          // <5,  Deep Sleep reset digital core
    case 6: Serial.println("SDIO_RESET"); break;               // <6,  Reset by SLC module, reset digital core
    case 7: Serial.println("TG0WDT_SYS_RESET"); break;         // <7,  Timer Group0 Watch dog reset digital core
    case 8: Serial.println("TG1WDT_SYS_RESET"); break;         // <8,  Timer Group1 Watch dog reset digital core
    case 9: Serial.println("RTCWDT_SYS_RESET"); break;         // <9,  RTC Watch dog Reset digital core
    case 10: Serial.println("INTRUSION_RESET"); break;         // <10, Instrusion tested to reset CPU
    case 11: Serial.println("TGWDT_CPU_RESET"); break;         // <11, Time Group reset CPU
    case 12: Serial.println("SW_CPU_RESET"); break;            // <12, Software reset CPU
    case 13: Serial.println("RTCWDT_CPU_RESET"); break;        // <13, RTC Watch dog Reset CPU
    case 14: Serial.println("EXT_CPU_RESET"); break;           // <14, for APP CPU, reseted by PRO CPU
    case 15: Serial.println("RTCWDT_BROWN_OUT_RESET"); break;  // <15, Reset when the vdd voltage is not stable
    case 16: Serial.println("RTCWDT_RTC_RESET"); break;        // <16, RTC Watch dog reset digital core and rtc module
    default: Serial.println("NO_MEAN");
  }
}

void print_reset_reason_verbose(RESET_REASON reason, const String &title) {
  Serial.print(title);
  Serial.print(" 0x0");
  Serial.print(reason, HEX);
  Serial.print(" ");
  switch (reason) {
    case POWERON_RESET: Serial.println("Vbat power on reset"); break;
    case SW_RESET: Serial.println("Software reset digital core"); break;
    case OWDT_RESET: Serial.println("Legacy watch dog reset digital core"); break;
    case DEEPSLEEP_RESET: Serial.println("Deep Sleep reset digital core"); break;
    case SDIO_RESET: Serial.println("Reset by SLC module, reset digital core"); break;
    case TG0WDT_SYS_RESET: Serial.println("Timer Group0 Watch dog reset digital core"); break;
    case TG1WDT_SYS_RESET: Serial.println("Timer Group1 Watch dog reset digital core"); break;
    case RTCWDT_SYS_RESET: Serial.println("RTC Watch dog Reset digital core"); break;
    case INTRUSION_RESET: Serial.println("Instrusion tested to reset CPU"); break;
    case TGWDT_CPU_RESET: Serial.println("Time Group reset CPU"); break;
    case SW_CPU_RESET: Serial.println("Software reset CPU"); break;
    case RTCWDT_CPU_RESET: Serial.println("RTC Watch dog Reset CPU"); break;
    case EXT_CPU_RESET: Serial.println("for APP CPU, reseted by PRO CPU"); break;
    case RTCWDT_BROWN_OUT_RESET: Serial.println("Reset when the vdd voltage is not stable"); break;
    case RTCWDT_RTC_RESET: Serial.println("RTC Watch dog reset digital core and rtc module"); break;
    default: Serial.println("NO_MEAN");
  }
}

void print_esp_error(esp_err_t code, const String &function) {
  if (code == 0)
    return;
  Serial.print(function + " Error: 0x");
  Serial.print(code, HEX);
  Serial.println();
}

const String EventName[9] = {
  "Pressed",
  "Released",
  "Change",
  "Tap",
  "Click",
  "Long",
  "Double",
  "Triple",
  "LongPress"
};

void print_button_event(Button2m &btn, Button2m::Event_t event) {
  Serial.print("btn");
  Serial.print(btn.Tag);
  Serial.print(": [");
  Serial.print(btn.readPin());
  Serial.print(" ");
  Serial.print(btn.isPressed(false));
  Serial.print(" ");
  Serial.print(btn.isPressed());
  Serial.print("]");
  Serial.print(": " + EventName[event]);
  switch ((int)event) {
    case Button2m::EventReleased:
    case Button2m::EventLong:
    case Button2m::EventDouble:
    case Button2m::EventTriple:
      unsigned int time = btn.getPressedTime();
      Serial.print(" (");
      Serial.print(time);
      Serial.print(" ms)");

      Serial.print(" # ");
      Serial.print(btn.getClickCount());
      break;
  }

  Serial.println();
}

void print_button_mask(uint16_t mask) {
  Serial.print("btn pressed: 0x0");
  Serial.print(mask, HEX);
  Serial.print(" 0000");
  Serial.print(mask, BIN);
  Serial.println("b");
}

void print_rect(int x, int y, int w, int h) {
  Serial.printf("%d %d %d %d\n", x, y, w, h);
}

/*
#ifdef __cplusplus
extern "C" {
#endif

uint8_t temprature_sens_read();

#ifdef __cplusplus    
}
#endif

void printCoreTemperature(){ 
  uint8_t temprature = temprature_sens_read();
  Serial.printf("Core temprature: %f °C (raw %d)\n" , (temprature - 32) / 1.8, temprature);
} 
*/  