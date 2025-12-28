#include "badge.h"
#include "i2c_bus.h"
#include <Arduino.h>
#include <TCA9555.h> // Include the library

// Volatile flag to track interrupt occurrence
static volatile bool expanderInterruptOccurred = false;

// Interrupt handler
void IRAM_ATTR expanderInterruptHandler() { expanderInterruptOccurred = true; }

static TCA9535 TCA(EXPANDER_ADDR, &I2C0_Bus);

bool pin_expander_init(void) {
  Serial.println("pin_expander_init()");
  // I2C is initialized in i2c_bus_init() - don't call begin() here

  // expander
  if (!TCA.begin()) {
    Serial.println("  ERROR: Failed to initialize TCA9535");
    return false;
  }

  TCA.pinMode16(0xFFFF); // Set all pins as inputs

  // Read initial state to clear any pending interrupt
  TCA.read16();

  // Configure expander interrupt pin as input with pullup
  pinMode(EXP_IRQ_PIN, INPUT_PULLUP);

  // Attach interrupt: GPIO pin, ISR function, FALLING edge (high to low)
  attachInterrupt(digitalPinToInterrupt(EXP_IRQ_PIN), expanderInterruptHandler,
                  FALLING);

  Serial.println("  Pin expander initialized");

  return true;
}

// Function to check and handle keyboard interrupt
char pin_expander_process_irq() {
  char returnedchar = 'x';
  if (expanderInterruptOccurred) {
    uint16_t pressed = TCA.read16();

    switch (pressed) {
    case 0b111111111110:
      returnedchar = '0';
      break;
    case 0b111111111101:
      returnedchar = '1';
      break;
    case 0b111111111011:
      returnedchar = '2';
      break;
    case 0b111111110111:
      returnedchar = '3';
      break;
    case 0b111111101111:
      returnedchar = '4';
      break;
    case 0b111111011111:
      returnedchar = '5';
      break;
    case 0b111110111111:
      returnedchar = '6';
      break;
    case 0b111101111111:
      returnedchar = '7';
      break;
    case 0b111011111111:
      returnedchar = '8';
      break;
    case 0b110111111111:
      returnedchar = '9';
      break;
    case 0b011111111111:
      returnedchar = 'N';
      break;
    case 0b101111111111:
      returnedchar = 'Y';
      break;
    default:
      returnedchar = 'x';
      break;
    }
    // Serial.println(state->keyboard, BIN);
  }

  expanderInterruptOccurred = false;
  return returnedchar;
}
