#ifndef DISPLAY_H
#define DISPLAY_H

#include "badge.h"
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <GxEPD2_BW.h>
#include <epd/GxEPD2_290_T94_V2.h>

#define OSDBOX_W 240
#define OSDBOX_H 60
#define DISPLAY_BACKGROUND_LIGHT_LEVEL 5 // 0-1023

#define DISPLAY_LINE_1 "39C3"
#define DISPLAY_LINE_1A "CDC Badge Nametag"
#define DISPLAY_LINE_2 ""
#define DISPLAY_LINE_3 "ESP32s3, E-Ink and  "
#define DISPLAY_LINE_4 "TROPIC01 Secure Element"
#define DISPLAY_LINE_5 ""

void display_init();
void display_show();
void home_display();
void brightness_osd(int brightness);

// Other functions used for test and inspiration:
// void test_graphics_checkerboard(void);
// void test_EPD_light(void);
// void test_EPD_pwm(void);
// void test_gpio_led_output(void);

#endif // DISPLAY_H