#ifndef BQ25895_H
#define BQ25895_H

// Initialize power management (BQ25895)
// Returns true on success, false on failure
bool power_management_init();

// Disable charging on BQ25895
// If disconnect_battery is true, also turns off BATFET (disconnects battery - will lose power!)
// Returns true on success, false on failure
bool power_management_disable(bool disconnect_battery = false);

// Disconnect battery (system stays powered through USB only)
// Returns true on success, false on failure
bool power_management_disconnect_battery();

// Reconnect battery (re-enable BATFET)
// Returns true on success, false on failure
bool power_management_reconnect_battery();

// Software shutdown via QON emulation (BATFET_DLY)
// This emulates pulling QON low - delays then disconnects battery
// Returns true on success, false on failure
bool power_management_shutdown();

#endif