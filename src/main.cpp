#include <Arduino.h>
#include "app.h"
#include "badge.h"

// Global TROPIC01 instance (needed for app access)
extern Tropic01 tropic01;

app_state_t app_state;

void setup() {
    // Initialize all hardware resources
    badge_init();

    // Initialize application state
    app_setup(&app_state);
}

void loop() {
    // Loop through all inputs and process application outputs
    app_loop(&app_state);
}
