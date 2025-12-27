#include <Arduino.h>

#include "spi_bus.h"
#include "badge.h"
#include "display.h"

GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display(GxEPD2_290_T94_V2(EPD_CS_PIN, EPD_DC_PIN, EPD_RST_PIN,
                                                                                  EPD_BUSY_PIN));

void drawLogo(int16_t x, int16_t y, int16_t size) {
    // Signal wave icon representing off-grid messaging
    int16_t waveWidth = size;
    int16_t waveHeight = size / 2;
    
    // Draw three signal arcs
    for (int i = 0; i < 3; i++) {
        int16_t radius = (i + 1) * (waveWidth / 6);
        display.drawCircle(x, y + waveHeight / 2, radius, GxEPD_WHITE);
        // Draw only right half by overlaying black on left
        display.fillRect(x - radius - 1, y + waveHeight / 2 - radius, 
                        radius + 1, radius * 2, GxEPD_BLACK);
    }
    
    // Central dot
    display.fillCircle(x, y + waveHeight / 2, 3, GxEPD_WHITE);
}

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
    display.setFont(&FreeSansBold9pt7b);
    display.setTextColor(GxEPD_WHITE);

    // Use partial window for smooth, flicker-free updates (ideal for menus)
    // This avoids the heavy blinking of full refresh mode
    display.setPartialWindow(0, 0, display.width(), display.height());
    
    display.firstPage();
    
    do {
        display.fillScreen(GxEPD_BLACK);  // Black background
        display.setTextColor(GxEPD_WHITE);  // White text
        // Draw logo
        drawLogo(20, 15, 40);
        
        // Title
        display.setFont(&FreeSansBold12pt7b);
        display.setCursor(70, 35);
        
        display.print(DISPLAY_LINE_1);


        display.setFont(&FreeSansBold12pt7b);
        display.setCursor(70, 55);
        
        display.print(DISPLAY_LINE_39C);
        // Subtitle
        display.setFont(&FreeSans9pt7b);
        display.setCursor(70, 90);
        display.print(DISPLAY_LINE_2);
        
        // Tagline
        display.setCursor(20, 110);
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
