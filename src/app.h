#ifndef APP_H
#define APP_H
#include <Arduino.h>

// All state variables of app might be tracked here.
// Then they can be processed in a loop and display could be updated.
typedef struct app_state_t {
  bool is_initialized;
} app_state_t;

void app_setup(app_state_t *state);

void app_loop(app_state_t *state);

#endif // APP_H