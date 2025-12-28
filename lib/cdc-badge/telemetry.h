#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <Arduino.h>

// Charge status enumeration derived from BQ25895 REG0A bits 4-3
typedef enum {
    CHARGE_STATUS_NOT_CHARGING = 0,
    CHARGE_STATUS_PRECHARGE = 1,
    CHARGE_STATUS_FASTCHARGE = 2,
    CHARGE_STATUS_DONE = 3
} charge_status_t;

// Display mode enumeration
typedef enum {
    DISPLAY_MODE_NAMETAG = 0,
    DISPLAY_MODE_TELEMETRY = 1,
    DISPLAY_MODE_TROPIC_DEMO = 2
} display_mode_t;

// Comprehensive telemetry data structure
typedef struct {
    float battery_voltage;
    uint8_t battery_percentage;
    charge_status_t charge_status;
    bool usb_connected;
    uint16_t charge_current_ma;
    float system_voltage;
    uint32_t uptime_seconds;
    uint32_t free_heap_bytes;
    float temperature_celsius;
    bool tropic01_active;
    display_mode_t display_mode;
    uint32_t last_update_millis;
} telemetry_data_t;

// Initialize telemetry subsystem
void telemetry_init();

// Update all telemetry readings
void telemetry_update(telemetry_data_t *data);

// Get battery voltage from BQ25895 ADC registers
float telemetry_get_battery_voltage();

// Get charge status from BQ25895
charge_status_t telemetry_get_charge_status();

// Get USB connection status
bool telemetry_get_usb_connected();

// Get charge current in mA
uint16_t telemetry_get_charge_current();

// Get system voltage
float telemetry_get_system_voltage();

// Convert battery voltage to percentage (using lookup table)
uint8_t telemetry_voltage_to_percentage(float voltage);

// Format uptime as string (HH:MM:SS)
void telemetry_format_uptime(uint32_t seconds, char *buffer, size_t buflen);

#endif // TELEMETRY_H
