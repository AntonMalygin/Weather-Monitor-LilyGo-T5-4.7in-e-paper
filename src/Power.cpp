// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#include "Power.h"
#include <rom/rtc.h>
#include <esp_sleep.h>
#include "esp_adc_cal.h"        // In-built
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

Power power;

Power::Power() {};
Power::~Power() {};

void Power::deepSleep(uint64_t time_in_us) {
  if (_wakeupPin != 0xFF) {
    while (digitalRead(_wakeupPin) == _wakeupLevel) {
      delay(10);
    }
    esp_sleep_enable_ext0_wakeup((gpio_num_t)_wakeupPin, LOW);
  }
  if (time_in_us > 0) {
    #ifdef DEBUG_DEEP_SLEEP
    Serial.println("deepSleep: "  + String(uint32_t(time_in_us / 1000)) + " ms");
    Serial.flush();
    #endif
    esp_deep_sleep(time_in_us);
  } else {
    #ifdef DEBUG_DEEP_SLEEP
    Serial.println("deepSleep");
    Serial.flush();
    #endif
    esp_deep_sleep_start();
  }
}

void Power::lightSleep(uint64_t time_in_us) {
  if (_wakeupPin != 0xFF) {
    while (digitalRead(_wakeupPin) == _wakeupLevel) {
      delay(10);
    }
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_AUTO);
    uint64_t ext_wakeup_pin_mask = 1ULL << _wakeupPin;
    esp_sleep_enable_ext1_wakeup(ext_wakeup_pin_mask, ESP_EXT1_WAKEUP_ALL_LOW);
  }
  if (time_in_us > 0) {
    esp_sleep_enable_timer_wakeup(time_in_us);

  } else {
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_TIMER);
  }
  #ifdef DEBUG_LIGH_SLEEP
  Serial.flush();
  #endif
  esp_light_sleep_start();
}

void Power::setWakeupPin(uint8_t pin) {
  _wakeupPin = pin;
}

bool Power::isWakeupByTimer() {
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  return (wakeup_reason == ESP_SLEEP_WAKEUP_TIMER);
}

float  Power::getBatteryVoltage() {
  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
    #ifdef SERIAL_DEBUG
    Serial.printf("eFuse Vref:%u mV", adc_chars.vref);
    #endif
    _vref = adc_chars.vref;
  }
  float voltage = 0;
  int   count = 64;
  for (int i = 0; i < count; ++i){
    voltage += analogRead(36);
  }
  voltage = voltage / (float)count;
  voltage = voltage / 4096.0 * 6.566 * (_vref / 1000.0);
  return voltage;

}
uint8_t Power::getBatteryLevel() {
  return(getBatteryLevel(getBatteryVoltage()));
}
uint8_t Power::getBatteryLevel(float voltage) {
  float percentage = 2836.9625 * pow(voltage, 4) - 43987.4889 * pow(voltage, 3) + 255233.8134 * pow(voltage, 2) - 656689.7123 * voltage + 632041.7303;
  if (voltage >= 4.20) percentage = 100;
  if (voltage <= 3.20) percentage = 0;  // orig 3.5
  return round(percentage);
}
