#ifndef TEST_COMMON_H
#define TEST_COMMON_H
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <unity.h>

#define LED_PIN 0

// --- PINS FROM SCHEMATICS ---
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

TwoWire I2C0_Bus = TwoWire(0);

// Screen
#include <Fonts/FreeMonoBold9pt7b.h>
#include <GxEPD2_BW.h>
#include <epd/GxEPD2_290_T94_V2.h>
GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display(GxEPD2_290_T94_V2(EPD_CS_PIN, EPD_DC_PIN, EPD_RST_PIN,
                                                                                  EPD_BUSY_PIN));

void setup_clean(void) {

    // reset screen
    pinMode(18, OUTPUT);
    digitalWrite(18, HIGH);

    pinMode(EPD_LED_PIN, OUTPUT);
    digitalWrite(EPD_LED_PIN, HIGH);

    SPI.end();
    SPI.begin(SPI_SCLK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN, EPD_CS_PIN);

    display.init(115200, true, 20, false);
}

void setup_usb_serial() {
    Serial.begin(115200);
    unsigned long start = millis();
    while (!Serial && (millis() - start) < 3000) {
        delay(10);
    }
    delay(500);
    Serial.println("\n--- Serial available ---");
}

// executed before each test
void setUp(void) {
    static int test_counter = 0;
    test_counter++;
    Serial.print("\n====== TEST #");
    Serial.println(test_counter);
}

// executed after each test
void tearDown(void) {}

// general i2c setup
void bq25895_init() {
    I2C0_Bus.begin(I2C0_SDA_PIN, I2C0_SCL_PIN);
    pinMode(CHG_DSEL_PIN, INPUT);
    // pinMode(CHG_IRQ_PIN, INPUT_PULLUP); // confirm IRQ open drain?
    // pinMode(FLASH_BTN_PIN, INPUT_PULLDOWN);
}

// write a single register
bool write_bq_reg(uint8_t reg_addr, uint8_t data) {
    I2C0_Bus.beginTransmission(BQ25895_ADDR);
    I2C0_Bus.write(reg_addr);
    I2C0_Bus.write(data);
    return I2C0_Bus.endTransmission(true) == 0;
}

// read a single register
uint8_t read_bq_reg(uint8_t reg_addr) {
    I2C0_Bus.beginTransmission(BQ25895_ADDR);
    I2C0_Bus.write(reg_addr);

    if (I2C0_Bus.endTransmission(false) != 0) {
        return 0xFF;
    }
    if (I2C0_Bus.requestFrom(BQ25895_ADDR, 1) != 1) {
        return 0xFE;
    }
    return I2C0_Bus.read();
}

#endif
