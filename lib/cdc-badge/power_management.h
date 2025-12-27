#ifndef BQ25895_H
#define BQ25895_H

// Initialize power management (BQ25895)
// Returns true on success, false on failure
bool power_management_init();

// Disconnect battery (system stays powered through USB only)
// Returns true on success, false on failure
bool power_management_disconnect_battery();

// Process power button interrupt (GPIO0/FLASH_BTN_PIN)
// Returns true if button was pressed, false otherwise
bool powerdown_process_irq();

#endif