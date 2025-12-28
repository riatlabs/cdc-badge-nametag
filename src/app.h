#ifndef APP_H
#define APP_H

#include <Arduino.h>
#include <LibtropicArduino.h>
#include "telemetry.h"
#include "tropic_demo.h"

// Application state structure
typedef struct app_state_t {
    bool is_initialized;
    display_mode_t display_mode;
    telemetry_data_t telemetry;
    tropic_demo_data_t tropic_demo;
    uint32_t last_telemetry_update;
    uint32_t last_display_update;
    uint32_t last_page_rotation;
    bool display_needs_update;
    uint8_t demo_step;
    uint8_t telemetry_page;
    bool auto_rotate_pages;
} app_state_t;

// Update intervals in milliseconds
#define TELEMETRY_UPDATE_INTERVAL 10000
#define DISPLAY_UPDATE_INTERVAL 5000
#define PAGE_ROTATION_INTERVAL 10000

// External TROPIC01 instance
extern Tropic01 tropic01;

void app_setup(app_state_t *state);
void app_loop(app_state_t *state);
void app_switch_mode(app_state_t *state, display_mode_t mode);
void app_next_page(app_state_t *state);
void app_prev_page(app_state_t *state);
void app_update_display(app_state_t *state);
void app_handle_number_button(app_state_t *state, char button);
void app_handle_yn_button(app_state_t *state, char button);

#endif // APP_H
