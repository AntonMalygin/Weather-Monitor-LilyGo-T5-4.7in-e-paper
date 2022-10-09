// Hello BASIC PRINT :-)
#pragma once

#include <Arduino.h>
#include <rom/rtc.h>
#include "Button2m.h"

void print_build(const String &title);

void print_chip_id(void);

void print_wakeup_reason(esp_sleep_wakeup_cause_t wakeup_reason);

void print_reset_reason(RESET_REASON reason);

void print_reset_reason_verbose(RESET_REASON reason, const String &title = "");

void print_esp_error(esp_err_t code, const String &function);

void print_button_event(Button2m &btn, Button2m::Event_t event);

void print_button_mask(uint16_t mask);

void print_rect(int x, int y, int w, int h);



