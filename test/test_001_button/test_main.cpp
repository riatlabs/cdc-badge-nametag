#include <Arduino.h>
#include <unity.h>
#include "../test_common.h"

#ifndef TEST_BUTTON_PIN
  #define TEST_BUTTON_PIN 0 
#endif
#define BUTTON_PIN TEST_BUTTON_PIN 

void wait_for_button_state(int target_state, const char* prompt) {
  Serial.println(prompt);
  while (digitalRead(BUTTON_PIN) != target_state) {
      delay(10); 
  }
}

void test_io0_full_cycle(void) {
  TEST_ASSERT_EQUAL_MESSAGE(HIGH, digitalRead(BUTTON_PIN), "ERROR: start state not HIGH (already pressed"); 
  wait_for_button_state(LOW, "\n>>> TEST: PLEASE PRESS AND HOLD BUTTON");
  Serial.println(">>> Registered button press");
  wait_for_button_state(HIGH, ">>> PLEASE RELEASE BUTTON <<<");
  Serial.println(">>> Registered button release");
}

void setup() {
  setup_usb_serial();
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  UNITY_BEGIN();
  RUN_TEST(test_io0_full_cycle);
  UNITY_END();
}

void loop() { }
