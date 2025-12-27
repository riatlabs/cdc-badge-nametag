#include <Arduino.h>

#include "spi_bus.h"
#include "badge.h"
#include "display.h"

GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display(GxEPD2_290_T94_V2(EPD_CS_PIN, EPD_DC_PIN, EPD_RST_PIN,
                                                                                  EPD_BUSY_PIN));

void display_init() {

    Serial.println("display_init()");

    const int PWM_CHANNEL = 0;     // LEDC-channel 0
    const int PWM_FREQ = 10000;    // 5 kHz freq
    const int PWM_RESOLUTION = 10; // 10 bit

    ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);

    ledcAttachPin(EPD_LED_PIN, PWM_CHANNEL);

    // Keep LED off during display update
    ledcWrite(PWM_CHANNEL, DISPLAY_BACKGROUND_LIGHT_LEVEL);

    // SPI is already initialized in spi_bus_init(), just use it
    display.init(115200, true, 20, false, SPI_Bus, SPISettings(4000000, MSBFIRST, SPI_MODE0));

    display.setRotation(1);
    display.setFont(&FreeMonoBold9pt7b);
    display.setTextColor(GxEPD_WHITE);

    display.setFullWindow();
    display.firstPage();
    do {
        display.fillScreen(GxEPD_BLACK);  // Black background
        display.setTextColor(GxEPD_WHITE);  // White text

        display.setCursor(10, 30);
        display.print(DISPLAY_LINE_1);

        display.setCursor(10, 60);
        display.print(DISPLAY_LINE_2);

        display.setCursor(10, 90);
        display.print(DISPLAY_LINE_3);

    } while (display.nextPage());

    Serial.println("[SUCCESS] Screen updated");
}

void display_show() {
    const int PWM_CHANNEL = 0;     // LEDC-channel 0
    const int PWM_FREQ = 10000;    // 5 kHz freq
    const int PWM_RESOLUTION = 10; // 10 bit

    ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);

    ledcAttachPin(EPD_LED_PIN, PWM_CHANNEL);

    // Keep LED off during display update
    ledcWrite(PWM_CHANNEL, DISPLAY_BACKGROUND_LIGHT_LEVEL);
}

/*
void test_gpio_led_output(void) {
    Serial.println("[TEST] 5 second LED test");
    pinMode(LED_PIN, OUTPUT);

    digitalWrite(LED_PIN, LOW);
    Serial.println("-> LED should be ON");
    delay(2000);

    digitalWrite(LED_PIN, HIGH);
    Serial.println("-> LED should be OFF");
    delay(2000);
}

void test_EPD_pwm(void) {
    Serial.println("[TEST] EPD background light (PWM Ramp Test)");

    const int PWM_CHANNEL = 0;     // LEDC-channel 0
    const int PWM_FREQ = 10000;    // 5 kHz freq
    const int PWM_RESOLUTION = 10; // 10 bit

    ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);

    ledcAttachPin(EPD_LED_PIN, PWM_CHANNEL);
    //Serial.println("PWM-Controller configured for EPD LED");

    Serial.println("-> ramp 0% to 100%.");
    for (int dutyCycle = 0; dutyCycle <= 1023; dutyCycle += 32) {
        ledcWrite(PWM_CHANNEL, dutyCycle);
        delay(1000);
    }

    Serial.println("-> starting ramp 100% to 0% (fast)");
    for (int dutyCycle = 1023; dutyCycle >= 0; dutyCycle -= 32) {
        ledcWrite(PWM_CHANNEL, dutyCycle);
        delay(500);
    }

    Serial.println("-> starting ramp 100% to 0% (slow)");
    for (int dutyCycle = 1023; dutyCycle >= 0; dutyCycle -= 32) {
        ledcWrite(PWM_CHANNEL, dutyCycle);
        delay(50);
    }
    Serial.println("-> 10x quick ramps");
    for (int i = 0; i < 500; i++) {

        for (int dutyCycle = 1023; dutyCycle >= 0; dutyCycle -= 32) {
            ledcWrite(PWM_CHANNEL, dutyCycle);
            delay(10);
        }
    }

    ledcWrite(PWM_CHANNEL, 0);

    Serial.println("VISUAL CHECK: did the screen dim? (and flicker?)");
    //TEST_ASSERT_TRUE(true);
}

void test_graphics_checkerboard(void) {
    Serial.println("[TEST] Graphics: Checkerboard");
    pinMode(18, OUTPUT);
    digitalWrite(18, HIGH);

    pinMode(EPD_LED_PIN, OUTPUT);
    digitalWrite(EPD_LED_PIN, HIGH);

    // SPI is already initialized, just use it
    display.init(1000000, true, 20, false);

    display.setRotation(1);
    display.setFont(&FreeMonoBold9pt7b);
    display.setTextColor(GxEPD_BLACK);

    display.setFullWindow();
    display.firstPage();
    do {
        display.fillScreen(GxEPD_WHITE);

        int boxSize = (rand() % 19) + (rand() % 16);
        for (int y = 0; y < display.height(); y += boxSize) {
            for (int x = 0; x < display.width(); x += boxSize) {
                if ((x / boxSize + y / boxSize) % 2 == 0) {
                    display.fillRect(x, y, boxSize, boxSize, GxEPD_BLACK);
                }
            }
        }

        display.fillRect(10, 10, 200, 60, GxEPD_WHITE);
        display.drawRect(10, 10, 200, 60, GxEPD_BLACK);
        display.setCursor(20, 30);
        display.print("Inspected by C.T.");

    } while (display.nextPage());
    delay(4000);

    display.powerOff();
}
*/
