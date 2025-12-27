#include "i2c_bus.h"
#include "badge.h"

// Define the I2C bus instances only once
TwoWire I2C0_Bus = TwoWire(0);
TwoWire I2C1_Bus = TwoWire(1);

bool i2c_bus_init() {
    bool success = true;
    Serial.println("i2c_bus_init()");

    // Initialize I2C0 bus
    if (!I2C0_Bus.begin(I2C0_SDA_PIN, I2C0_SCL_PIN)) {
        Serial.println("ERROR: Failed to initialize I2C0_Bus");
        success = false;
    }
    
    // Initialize I2C1 bus
    if (!I2C1_Bus.begin(I2C1_SDA_PIN, I2C1_SCL_PIN)) {
        Serial.println("ERROR: Failed to initialize I2C1_Bus");
        success = false;
    }
    
    return success;
}
