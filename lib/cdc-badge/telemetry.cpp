#include "telemetry.h"
#include "badge.h"
#include "i2c_bus.h"
#include <esp_system.h>

// External reference to BQ25895 register access functions
extern uint8_t read_bq_reg(uint8_t reg_addr);

// Battery voltage lookup table for LiPo (typical discharge curve)
// Maps voltage to percentage in 0.1V increments from 3.0V to 4.2V
static const uint8_t voltage_to_percent[] = {
    0,   // 3.0V
    5,   // 3.1V
    10,  // 3.2V
    15,  // 3.3V
    20,  // 3.4V
    30,  // 3.5V
    40,  // 3.6V
    50,  // 3.7V
    60,  // 3.8V
    75,  // 3.9V
    85,  // 4.0V
    90,  // 4.1V
    100  // 4.2V
};

void telemetry_init() {
    // Telemetry initialization (if needed)
    // Currently all initialization handled by badge_init()
}

float telemetry_get_battery_voltage() {
    // Read battery voltage from BQ25895 ADC
    // REG0E: BATV[6:0] high bits, REG0F: BATV low bit
    uint8_t reg0e = read_bq_reg(0x0E);
    uint8_t reg0f = read_bq_reg(0x0F);
    
    // BATV = (REG0E[6:0] << 1) | REG0F[7]
    // Resolution: 20mV, offset: 2304mV
    uint16_t adc_value = ((reg0e & 0x7F) << 1) | ((reg0f >> 7) & 0x01);
    float voltage = 2.304f + (adc_value * 0.02f);
    
    return voltage;
}

charge_status_t telemetry_get_charge_status() {
    // Read charge status from REG0A bits 4-3
    uint8_t reg0a = read_bq_reg(0x0A);
    uint8_t status = (reg0a >> 3) & 0x03;
    
    return (charge_status_t)status;
}

bool telemetry_get_usb_connected() {
    // Read VBUS status from REG0B bit 7
    uint8_t reg0b = read_bq_reg(0x0B);
    return (reg0b & 0x80) != 0;
}

uint16_t telemetry_get_charge_current() {
    // Read charge current from REG13
    // ICHGR ADC: Resolution 50mA, no offset
    uint8_t reg13 = read_bq_reg(0x13);
    return (reg13 & 0x7F) * 50;
}

float telemetry_get_system_voltage() {
    // Read system voltage from REG10
    // VSYS ADC: Resolution 20mV, offset 2304mV
    uint8_t reg10 = read_bq_reg(0x10);
    return 2.304f + ((reg10 & 0x7F) * 0.02f);
}

uint8_t telemetry_voltage_to_percentage(float voltage) {
    // Clamp voltage to valid range
    if (voltage < 3.0f) return 0;
    if (voltage >= 4.2f) return 100;
    
    // Calculate index into lookup table
    float index_float = (voltage - 3.0f) / 0.1f;
    uint8_t index = (uint8_t)index_float;
    
    if (index >= sizeof(voltage_to_percent) - 1) {
        return voltage_to_percent[sizeof(voltage_to_percent) - 1];
    }
    
    // Linear interpolation between table values
    float fraction = index_float - index;
    uint8_t lower = voltage_to_percent[index];
    uint8_t upper = voltage_to_percent[index + 1];
    
    return lower + (uint8_t)((upper - lower) * fraction);
}

void telemetry_format_uptime(uint32_t seconds, char *buffer, size_t buflen) {
    uint32_t hours = seconds / 3600;
    uint32_t minutes = (seconds % 3600) / 60;
    uint32_t secs = seconds % 60;
    
    snprintf(buffer, buflen, "%02lu:%02lu:%02lu", hours, minutes, secs);
}

void telemetry_update(telemetry_data_t *data) {
    if (!data) return;
    
    // Update timestamp
    data->last_update_millis = millis();
    
    // Update power metrics
    data->battery_voltage = telemetry_get_battery_voltage();
    data->battery_percentage = telemetry_voltage_to_percentage(data->battery_voltage);
    data->charge_status = telemetry_get_charge_status();
    data->usb_connected = telemetry_get_usb_connected();
    data->charge_current_ma = telemetry_get_charge_current();
    data->system_voltage = telemetry_get_system_voltage();
    
    // Update system metrics
    data->uptime_seconds = millis() / 1000;
    data->free_heap_bytes = ESP.getFreeHeap();
    data->temperature_celsius = temperatureRead();
    
    // TROPIC01 status maintained by application layer
}
