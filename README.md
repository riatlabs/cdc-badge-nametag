# CDC Badge Firmware

This repository contins minimalistic firmware for CDC badge with e-paper display, keyboard and TROPIC01 secure element.
Hw design of this badge is [here](github.com/riatlabs/cdc-badge).
Read more about Critical Decentralisation Cluster [here](https://decentral.community/).

---

## Table of Contents

- [Quick Start](#quick-start)
- [How to Personalize Your Badge](#how-to-personalize-your-badge)
- [Power Management](#power-management)
- [Hardware Architecture](#hardware-architecture)
- [Software Architecture](#software-architecture)
- [Dependencies](#dependencies)
- [FAQ](#faq)

---

## Quick Start

### Prerequisites

1. Install [Visual Studio Code](https://code.visualstudio.com/)
2. Install the [PlatformIO extension](https://platformio.org/install/ide?install=vscode)
3. Restart Visual Studio Code

### Build and Flash

1. Clone this repository
2. Open the project folder in VS Code
3. Click the PlatformIO icon in the left sidebar
4. Select your build environment and click "Upload"

---

## How to Personalize Your Badge

To customize the display with your information:

1. Open [`lib/cdc-badge/display.h`](lib/cdc-badge/display.h)
2. Modify the following macros:

```cpp
#define DISPLAY_LINE_1 "Your name"
#define DISPLAY_LINE_2 "your project"
#define DISPLAY_LINE_3 "your web"
```

3. Rebuild and flash the firmware

---

## Power Management

### Power Controls

- **Power ON**: Press and hold **SW12** for ~3 seconds
- **Power OFF**: Press the **FLASH button** (GPIO0)

---

## Hardware Architecture

### Component Overview

| Component          | Interface | Address/Pin | Description               |
|--------------------|-----------|-------------|---------------------------|
| BQ25895            | I2C0      | 0x6A        | Battery charging IC       |
| TCA9535 Expander   | I2C1      | 0x20        | Keyboard matrix I/O       |
| GxEPD2 Display     | SPI       | CS=41       | E-paper display           |
| TROPIC01           | SPI       | CS=10       | Secure element            |
| LED                | GPIO      | 8           | Status indicator          |

### Pin Configuration

```
I2C Buses:
  I2C0: SDA=17, SCL=18  → Power Management (BQ25895)
  I2C1: SDA=47, SCL=48  → Pin Expander (TCA9535)

SPI Bus:
  SCLK=12, MISO=11, MOSI=13

E-Paper Display:
  CS=41, DC=45, RST=46, BUSY=42, LED=8

TROPIC01 Secure Element:
  CS=10

Buttons & Interrupts:
  EXP_IRQ=1    → Pin expander interrupt
  FLASH=0      → Power button / Boot mode
```

---

## Software Architecture

### Main Loop Flow

```cpp
setup() → badge_init() → app_setup()
   ↓
loop() → app_loop() → read keyboard input
```

### Initialization Sequence

1. **Serial Port** → Debug output (115200 baud)
2. **I2C Buses** → Initialize I2C0 (charging) and I2C1 (expander)
3. **Power Management** → Configure BQ25895 battery charger
4. **SPI Bus** → Initialize communication for display and secure element
5. **Pin Expander** → Setup TCA9535 for keyboard matrix scanning
6. **TROPIC01** → Initialize secure element and establish session
7. **Display** → Configure e-paper with white text on black background

---

## Dependencies

This project uses the following libraries:

- **[libtropic-arduino](https://github.com/tropicsquare/libtropic-arduino)** - TROPIC01 secure element SDK
- **[TCA9555](https://github.com/robtillaart/TCA9555)** - I²C I/O expander driver for keyboard
- **[GxEPD2](https://github.com/ZinggJM/GxEPD2)** - E-paper display driver
- **[Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library)** - Graphics rendering library

All dependencies are automatically managed by PlatformIO.

---

## FAQ

For frequently asked questions and troubleshooting, see the [FAQ document](./FAQ.md).

---

## License

This project is part of the Critical Decentralisation Cluster initiative.
