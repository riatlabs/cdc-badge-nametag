#ifndef DISPLAY_H
#define DISPLAY_H

#include "badge.h"
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <GxEPD2_BW.h>
#include <epd/GxEPD2_290_T94_V2.h>

#define DISPLAY_BACKGROUND_LIGHT_LEVEL 5  // 0-1023

// Display hardware initialization
void display_init();
void display_show();

// External display instance (defined in display.cpp)
extern GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display;

#endif // DISPLAY_H
