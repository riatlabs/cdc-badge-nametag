#ifndef I2C_BUS_H
#define I2C_BUS_H

#include <Wire.h>

// Declare the I2C bus instances as extern (defined elsewhere)
extern TwoWire I2C0_Bus; // For BQ25895 charging IC and for TCA9535 expander (if needed)
extern TwoWire I2C1_Bus; // Unused currently, goes to some extension headers

// Initialize both I2C buses
// Returns true on success, false on failure
bool i2c_bus_init();

#endif // I2C_BUS_H
