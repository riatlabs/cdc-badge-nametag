#ifndef BADGE_H
#define BADGE_H

#include "display.h"
#include "pin_expander.h"
#include "power_management.h"
#include "serial_port.h"
#include "tropic01.h"

#define DBG_SERIAL_BAUDRATE 115200

#define LED_PIN 0
// I2C0 (Charging IC)
#define I2C0_SDA_PIN 17
#define I2C0_SCL_PIN 18
// I2C1 (Expander)
#define I2C1_SDA_PIN 47
#define I2C1_SCL_PIN 48
// IO Expander Interrupt
#define EXP_IRQ_PIN 1
// Flash/BOOT Button (Power Off Trigger)
#define FLASH_BTN_PIN 0
// EPD Backlight
#define EPD_LED_PIN 8
// EPD Control Pins
#define EPD_CS_PIN 41
#define EPD_DC_PIN 45
#define EPD_RST_PIN 46
#define EPD_BUSY_PIN 42

// addresses for i2c devices
#define BQ25895_ADDR 0x6A
#define EXPANDER_ADDR 0x20

// Pins for the BQ25895
#define CHG_DSEL_PIN 21 // CHG_DSEL
#define CHG_IRQ_PIN 39  // CHG_IRQ

// SPI BUS PINS
#define SPI_SCLK_PIN 12
#define SPI_MISO_PIN 11
#define SPI_MOSI_PIN 13

// TROPIC SQUARE TROPIC-01 TR-01
#define TR01_CS_PIN 10

// Initialize badge hardware (I2C, TCA9535, interrupts)
void badge_init();

#endif // BADGE_H