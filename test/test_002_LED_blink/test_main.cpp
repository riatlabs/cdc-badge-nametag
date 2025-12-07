#include <Arduino.h>
#include <unity.h>
#include "../test_common.h"

#ifdef TEST_LED_PIN

void test_pin_mode_output(void) {
    pinMode(TEST_LED_PIN, OUTPUT);
    TEST_ASSERT_TRUE(true);
}

void test_led_state_high(void) {
    digitalWrite(TEST_LED_PIN, HIGH);
    delay(10);
    TEST_ASSERT_EQUAL(HIGH, digitalRead(TEST_LED_PIN));
}

void test_led_state_low(void) {
    digitalWrite(TEST_LED_PIN, LOW);
    delay(10);
    TEST_ASSERT_EQUAL(LOW, digitalRead(TEST_LED_PIN));
}

#endif

void setup() {
    setup_usb_serial();
    UNITY_BEGIN();
    #ifndef TEST_LED_PIN
	TEST_IGNORE_MESSAGE("SKIPPED: TEST_LED_PIN not defined");
    #else
        RUN_TEST(test_pin_mode_output);

        for (int i = 0; i < 5; i++) {
            RUN_TEST(test_led_state_high);
            delay(500);
            RUN_TEST(test_led_state_low);
            delay(500);
        }

    #endif
    UNITY_END();
}

void loop() {}
