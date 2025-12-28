#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// WiFi configuration structure
typedef struct {
    const char* ssid;
    const char* password;
    bool auto_connect;
} badge_wifi_config_t;  // Renamed to avoid ESP32 conflict

// c3nav configuration
typedef struct {
    const char* api_url;
    bool enabled;
    uint32_t scan_interval_ms;
} c3nav_config_t;

// c3nav location result
typedef struct {
    bool valid;
    float x;
    float y;
    float z;
    uint8_t level;
    const char* space_name;
    char space_name_buf[64];  // Buffer for space name storage
    float precision;
} c3nav_location_t;

// WiFi scan result for c3nav
typedef struct {
    char bssid[18];      // MAC address string
    char ssid[33];       // SSID string
    int8_t rssi;         // Signal strength
    uint32_t frequency;  // Frequency in kHz
} wifi_scan_result_t;

// Initialize WiFi module
bool wifi_init(badge_wifi_config_t *config);

// Connect to WiFi network
bool wifi_connect(const char* ssid, const char* password);

// Check WiFi connection status
bool wifi_is_connected();

// Disconnect from WiFi
void wifi_disconnect();

// Flight mode control
void wifi_set_flight_mode(bool enabled);
bool wifi_get_flight_mode();

// Get current SSID
const char* wifi_get_ssid();

// Get IP address as string
const char* wifi_get_ip();

// Scan for WiFi networks for c3nav positioning
int wifi_scan_for_c3nav(wifi_scan_result_t* results, int max_results);

// c3nav positioning functions
bool c3nav_init(c3nav_config_t *config);
bool c3nav_locate(wifi_scan_result_t* scan_results, int num_results, c3nav_location_t* location);
const char* c3nav_get_last_error();

// Default configurations
extern badge_wifi_config_t default_wifi_config;
extern c3nav_config_t default_c3nav_config;

#endif // WIFI_CONFIG_H
