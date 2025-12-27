#ifndef DISPLAY_H
#define DISPLAY_H

#include "badge.h"
#include <Fonts/FreeMonoBold9pt7b.h>
#include <GxEPD2_BW.h>
#include <epd/GxEPD2_290_T94_V2.h>

#define DISPLAY_BACKGROUND_LIGHT_LEVEL 5 // 0-1023

#define DISPLAY_LINE_1 "CDC badge 39c3"
#define DISPLAY_LINE_2 "ESP32S3, E-INK and"
#define DISPLAY_LINE_3 "TROPIC01"

void display_init();
void display_show();

// Other functions used for test and inspiration:
//void test_graphics_checkerboard(void);
//void test_EPD_light(void);
//void test_EPD_pwm(void);
//void test_gpio_led_output(void);

#endif // DISPLAY_H