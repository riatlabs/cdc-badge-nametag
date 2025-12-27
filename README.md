# CDC badge Firmware

Read more about Critical Decentralisation Cluster [here](https://decentral.community/).
Minimalistic firmware for CDC badge with e-paper, display, keyboard and TROPIC01 secure element.

# How to update display with my name?

* Install Visual Studio Code IDE
* Open it and install platform.io extension
* Restart Visual Studio Code IDE
* Clone this repo
* On the left side of Visual Studio Code, there is platformio icon. Click on it, then open content of this repo there

go to `lib/cdc-badge/display.h` and change following macros:
```
#define DISPLAY_LINE_1 "Your name"
#define DISPLAY_LINE_2 "your project"
#define DISPLAY_LINE_3 "your web"

```

Rebuild and flash.


### Main Loop
```cpp
setup() → badge_init() → app_setup()
   ↓
loop() → app_loop() → read buttons
```

## Hardware Components

| Component          | Interface | Address/Pin | Purpose              |
|--------------------|-----------|-------------|----------------------|
| BQ25895            | I2C0      | 0x6A        | Battery charging     |
| TCA9535 Expander   | I2C1      | 0x20        | Keyboard matrix      |
| GxEPD2 Display     | SPI       | CS=41       | E-paper display      |
| TROPIC01           | SPI       | CS=10       | Secure element       |
| LED                | GPIO      | 0           | Status indicator     |

### Pin Map
```
I2C0: SDA=17, SCL=18  (Charging)
I2C1: SDA=47, SCL=48  (Expander)
SPI:  SCLK=12, MISO=11, MOSI=13
EPD:  CS=41, DC=45, RST=46, BUSY=42, LED=8
TR01: CS=10
BTN:  EXP_IRQ=1, FLASH=0
```

## Dependencies

- [libtropic-arduino](https://github.com/tropicsquare/libtropic-arduino) - TROPIC01 SDK
- [TCA9555](https://github.com/robtillaart/TCA9555) - I/O expander driver
- [GxEPD2](https://github.com/ZinggJM/GxEPD2) - E-paper display driver
- [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library) - Graphics library

### App Initialization Flow
1. **Serial Port** → Debug output (115200 baud)
2. **I2C Buses** → Two buses (charging IC, pin expander)
3. **Power Management** → BQ25895 battery charging IC
4. **SPI Bus** → Display and secure element communication
5. **Pin Expander** → TCA9535 for keyboard input
6. **TROPIC01** → Secure element initialization + session
7. **Display** → E-paper with white text on black background
