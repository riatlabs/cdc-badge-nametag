# CDC Badge QC Firmware
Quality Control firmware for the CDC electronic badge.
Sets sane defaults and tests chips and peripherals.

## Charging IC
[BQ25895](https://www.ti.com/product/BQ25895)
- [] Disable ILIM pin on REG00[6]
- [] Disable high voltage (HVDCP and MaxCharge) on REG02[2-3]
- [] Disable OTG on REG03[5]
- [] Set minimum system voltage to 3.3V on REG03[1-3]
- [] Set fast charge current limit to 512 mA on REG04[0-6]
- [] Process interrupts coming from INT pin
- [] Watch DSEL pin

## License and authorship
Copyright © 2025 RIAT Institute

Licensed under [MIT License](/LICENSE).
