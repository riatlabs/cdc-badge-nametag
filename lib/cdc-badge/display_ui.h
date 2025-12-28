#ifndef DISPLAY_UI_H
#define DISPLAY_UI_H

#include <Arduino.h>
#include <GxEPD2_BW.h>
#include "telemetry.h"

// Icon dimensions
#define ICON_WIDTH 16
#define ICON_HEIGHT 16

// Battery icon states (16x16 pixels)
extern const uint8_t icon_battery_empty[];
extern const uint8_t icon_battery_low[];
extern const uint8_t icon_battery_medium[];
extern const uint8_t icon_battery_full[];
extern const uint8_t icon_battery_charging[];

// Status icons
extern const uint8_t icon_usb[];
extern const uint8_t icon_temperature[];
extern const uint8_t icon_memory[];
extern const uint8_t icon_secure[];
extern const uint8_t icon_clock[];

// Display function declarations
void ui_init();

// Render nametag mode
void ui_draw_nametag(GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> &display);

// Render telemetry with page rotation (0=power, 1=system, 2=overview)
void ui_draw_telemetry(GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> &display, 
                       telemetry_data_t *data, uint8_t page);

// Individual telemetry pages
void ui_draw_telemetry_power(GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> &display,
                             telemetry_data_t *data);
void ui_draw_telemetry_system(GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> &display,
                              telemetry_data_t *data);
void ui_draw_telemetry_overview(GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> &display,
                                telemetry_data_t *data);

// Render TROPIC01 demo
void ui_draw_tropic_demo(GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> &display, 
                         const char *demo_text);

// Helper functions for aligned text rendering
void ui_print_right(GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> &display, 
                    const char* text, int16_t x, int16_t y);

void ui_print_center(GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> &display,
                     const char* text, int16_t x, int16_t y);

// Icon rendering
void ui_draw_icon(GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> &display,
                  const uint8_t *icon, int16_t x, int16_t y);

// Get appropriate battery icon based on charge level and status
const uint8_t* ui_get_battery_icon(uint8_t percentage, charge_status_t status);

#endif // DISPLAY_UI_H
