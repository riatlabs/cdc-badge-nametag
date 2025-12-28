#include <Arduino.h>
#include "spi_bus.h"
#include "badge.h"
#include "display.h"

// Global display instance (accessible from app)
GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display(
    GxEPD2_290_T94_V2(EPD_CS_PIN, EPD_DC_PIN, EPD_RST_PIN, EPD_BUSY_PIN));

void display_init() {
    Serial.println("display_init()");

    const int PWM_CHANNEL = 0;
    const int PWM_FREQ = 10000;
    const int PWM_RESOLUTION = 10;

    ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(EPD_LED_PIN, PWM_CHANNEL);
    ledcWrite(PWM_CHANNEL, DISPLAY_BACKGROUND_LIGHT_LEVEL);

    // SPI already initialized in spi_bus_init()
    display.init(115200, true, 20, false, SPI_Bus, SPISettings(4000000, MSBFIRST, SPI_MODE0));
    display.setRotation(1);
    display.setTextColor(GxEPD_WHITE);

    Serial.println("Display hardware initialized");
}

void display_show() {
    const int PWM_CHANNEL = 0;
    const int PWM_FREQ = 10000;
    const int PWM_RESOLUTION = 10;

    ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(EPD_LED_PIN, PWM_CHANNEL);
    ledcWrite(PWM_CHANNEL, DISPLAY_BACKGROUND_LIGHT_LEVEL);
}
