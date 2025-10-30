# CDC Badge QC Firmware
Quality Control firmware for the CDC electronic badge.
Sets sane defaults and tests chips and peripherals.

## Charging IC
[BQ25895](https://www.ti.com/product/BQ25895)
- [  ] Disable ILIM pin on REG00[6]
- [  ] Disable OTG on REG03[5]
- [  ] Set minimum system voltage to 3.3V on REG03[1-3]
- [  ] Set fast charge current limit to 512 mA on REG04[0-6]
- [  ] Process interrupts coming from INT pin
- [  ] Watch DSEL pin
- [  ] Power off system by enabling shipping mode on REG09[5] upon a flash
  button (ESP32 IO0 pin) press.
- [  ] Read status and voltages from REG0B, REG0C, REG0E, REG0F, REG10, REG11,
  REG12 and REG13.

## IO Expander and Keypad
- [  ] Process interrupts.
- [  ] Check that all keypad buttons are working.

## Secure Element
- [  ] Establish secure channel and verify basic functionalities.

## E-Paper Display
- [  ] Draw something on the screen.
- [  ] Exercise the 3 refresh methods: full, fast and partial.

## E-Paper Display Backlight
- [  ] Use LED PWM and test different brightness levels.

## License and authorship
Copyright © 2025 RIAT Institute

Licensed under [MIT License](/LICENSE).
