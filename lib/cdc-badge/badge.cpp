#include "badge.h"
#include "i2c_bus.h"
#include "spi_bus.h"

void badge_init() {
    // Initialize serial port for debug output
    serial_port_init();

    // Initialize I2C buses first
    if (!i2c_bus_init()) {
        Serial.println("ERROR: I2C bus initialization failed!");
        // Continue anyway - some features may not work
    } else {
        Serial.println("  OK");
    }

    // bq25895 is used for power management
    if (!power_management_init()) {
        Serial.println("ERROR: Power management initialization failed!");
        // Continue anyway - charging features may not work
    } else {
        Serial.println("  OK");
    }

    // Initialize SPI bus
    if (!spi_bus_init()) {
        Serial.println("ERROR: SPI bus initialization failed!");
        // Continue anyway - SPI devices may not work
    } else {
        Serial.println("  OK");
    }

    // tca9535 is used as pin expander
    if (!pin_expander_init()) {
        Serial.println("ERROR: Pin expander initialization failed!");
        // Continue anyway - GPIO expander features may not work
    } else {
        Serial.println("  OK");
    }

    // Init display - put some initial screen there
    display_init();

    // TROPIC01 is used as secure element
    if (!tropic01_init()) {
        Serial.println("ERROR: TROPIC01 initialization failed!");
        // Continue anyway - secure element features will not work
    } else {
        Serial.println("  OK");
    }

    // Start secure session with TROPIC01
    if (!tropic01_secure_session_start()) {
        Serial.println("ERROR: TROPIC01 secure session start failed!");
        // Continue anyway - secure element features will not work
    } else {
        Serial.println("  OK");
    }
}
