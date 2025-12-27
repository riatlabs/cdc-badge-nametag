#include "app.h"
#include "badge.h"
#include <Arduino.h>

void app_setup(app_state_t *state) {
  state->is_initialized = true;
}

void app_loop(app_state_t *state) {

  char pressed = pin_expander_process_irq();
  if (pressed != 'x') {
    Serial.print("Button pressed: ");
    Serial.println(pressed);
  }

  // Once we have some menu, we can update it here
  // update_menu();
}