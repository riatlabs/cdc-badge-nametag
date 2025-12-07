#include <unity.h>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "../test_common.h"

#if !defined(TEST_RGB_LED_PIN) || !defined(TEST_BUTTON_PIN)
    #define PINS_DEFINED 0
#else
    #define PINS_DEFINED 1
    Adafruit_NeoPixel strip(1, TEST_RGB_LED_PIN, NEO_GRB + NEO_KHZ800);
#endif

void waitForButtonClick(const char* prompt) {
    Serial.print("\n>>> ");
    Serial.println(prompt);
    Serial.println(">>> Press button to continue");

    while (digitalRead(TEST_BUTTON_PIN) == HIGH) {
        delay(50);
    }

    delay(200);

    while (digitalRead(TEST_BUTTON_PIN) == LOW) {
        delay(50);
    }

    Serial.println(">>> Button pressed");
}

void test_rgb_led_color(void) {
    // check if led pin is set, otherwise skip test
    #ifndef TEST_RGB_LED_PIN 
        Serial.println("\n[SKIPPED] Test 'test_rgb_led_color': TEST_RGB_LED_PIN ist not set");
        UNITY_IGNORE(); 
    #endif

    Adafruit_NeoPixel strip(1, TEST_RGB_LED_PIN, NEO_GRB + NEO_KHZ800);
    strip.begin();

    strip.setBrightness(50);

    // test red
    strip.setPixelColor(0, strip.Color(255, 0, 0));
    strip.show();
    waitForButtonClick("Visual check: is the LED red?");

    // test green
    strip.setPixelColor(0, strip.Color(0, 255, 0));
    strip.show();
    waitForButtonClick("Visual check: is the LED green?");

    // test blue
    strip.setPixelColor(0, strip.Color(0, 0, 255));
    strip.show();
    waitForButtonClick("Visual check: is the LED blue?");

    // test white
    strip.setPixelColor(0, strip.Color(255, 255, 255));
    strip.show();
    waitForButtonClick("Visual check: is the LED white?");


    // turn off led
    strip.setPixelColor(0, strip.Color(0, 0, 0));
    strip.setBrightness(0);
    strip.show();
    TEST_PASS();
}

void setup() {
    setup_usb_serial();
    UNITY_BEGIN();
    RUN_TEST(test_rgb_led_color);
    UNITY_END(); 
}

void loop(){}
