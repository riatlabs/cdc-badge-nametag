#include "badge.h"
#include "i2c_bus.h"
#include <Arduino.h>

// Write a single register
bool write_bq_reg(uint8_t reg_addr, uint8_t data) {
  I2C0_Bus.beginTransmission(BQ25895_ADDR);
  I2C0_Bus.write(reg_addr);
  I2C0_Bus.write(data);
  return I2C0_Bus.endTransmission(true) == 0;
}

// Read a single register
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

bool power_management_init() {

  Serial.println("power_management_init()");

  // power
  pinMode(CHG_DSEL_PIN, INPUT);
  // pinMode(CHG_IRQ_PIN, INPUT_PULLUP); // confirm IRQ open drain?
  // pinMode(FLASH_BTN_PIN, INPUT_PULLDOWN);

  // charging IC BQ25895 is at 0x6A
  byte error;

  // confirm part number
  uint8_t val = read_bq_reg(0x14);
  uint8_t part_number = (val >> 3) & 0x07;
  Serial.printf("  REG14 Raw: 0x%02X -> Part Number: %d\r\n", val, part_number);

  if (part_number != 7) {
    Serial.println(
        "[WARNING] Chip ID does not match standard BQ25895 (Expected: 7)");
    return false; // Chip not detected
  }

  Serial.println("  Setting defaults on BQ25895");
  uint8_t readback;

  // Disable ILIM pin on REG00[6]
  uint8_t current_val = read_bq_reg(0x00);
  uint8_t new_val = current_val & ~(1 << 6);
  if (current_val != new_val) {
    if (!write_bq_reg(0x00, new_val)) {
      Serial.println("ERROR: Failed to write REG00");
      return false;
    }
    Serial.printf("  REG00: ILIM disabled. Old: 0x%02X -> New: 0x%02X\r\n",
                  current_val, new_val);
  } else {
    Serial.println("  REG00: ILIM was already disabled.");
  }

  // Disable OTG on REG03[5]
  current_val = read_bq_reg(0x03);
  new_val = current_val & ~(1 << 5);
  if (current_val != new_val) {
    if (!write_bq_reg(0x03, new_val)) {
      Serial.println("  ERROR: Failed to write REG03");
      return false;
    }
    Serial.printf("  REG03: OTG disabled. Old: 0x%02X -> New: 0x%02X\r\n",
                  current_val, new_val);
  } else {
    Serial.println("  REG03: OTG was already disabled.");
  }

  // Serial.println("Power management initialized successfully");
  return true;
}

bool power_management_disconnect_battery() {
  Serial.println("power_management_disconnect_battery()");
  Serial.println("  WARNING: Ensure USB is connected before calling this!");

  // Disconnect battery by setting BATFET_DIS (REG09[5] = 1)
  // System will only run from USB power after this
  uint8_t current_val = read_bq_reg(0x09);
  uint8_t new_val = current_val | (1 << 5);

  if (!write_bq_reg(0x09, new_val)) {
    Serial.println("  ERROR: Failed to set BATFET_DIS");
    return false;
  }

  Serial.printf("  REG09: Battery disconnected. Old: 0x%02X -> New: 0x%02X\r\n",
                current_val, new_val);
  Serial.println("  System now powered by USB only");

  return true;
}

bool power_management_reconnect_battery() {
  Serial.println("power_management_reconnect_battery()");

  // Reconnect battery by clearing BATFET_DIS (REG09[5] = 0)
  uint8_t current_val = read_bq_reg(0x09);
  uint8_t new_val = current_val & ~(1 << 5);

  if (!write_bq_reg(0x09, new_val)) {
    Serial.println("  ERROR: Failed to clear BATFET_DIS");
    return false;
  }

  Serial.printf("  REG09: Battery reconnected. Old: 0x%02X -> New: 0x%02X\r\n",
                current_val, new_val);
  Serial.println("  Battery is now connected");

  return true;
}

bool power_management_shutdown() {
  Serial.println("power_management_shutdown()");
  Serial.println("  Initiating software shutdown via BATFET_DLY...");
  
  // Trigger QON-like shutdown by setting BATFET_DLY (REG09[3] = 1)
  // This will delay and then turn off BATFET, emulating QON pin being pulled low
  uint8_t current_val = read_bq_reg(0x09);
  uint8_t new_val = current_val | (1 << 3);
  
  if (!write_bq_reg(0x09, new_val)) {
    Serial.println("  ERROR: Failed to set BATFET_DLY");
    return false;
  }
  
  Serial.printf("  REG09: Shutdown initiated. Old: 0x%02X -> New: 0x%02X\r\n",
                current_val, new_val);
  Serial.println("  System will shutdown in 10 seconds...");
  
  return true;
}