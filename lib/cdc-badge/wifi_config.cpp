#include "wifi_config.h"

// Default configurations
badge_wifi_config_t default_wifi_config = {
    .ssid = "39C3-Open",
    .password = "",  // Open network
    .auto_connect = true
};

c3nav_config_t default_c3nav_config = {
    .api_url = "https://39c3.c3nav.de/api/v2/positioning/locate/",  // Fixed: v2 API
    .enabled = true,
    .scan_interval_ms = 30000  // Scan every 30 seconds
};

static char last_error[128] = {0};
static char current_ssid[33] = {0};
static char current_ip[16] = {0};
static bool flight_mode = false;

bool wifi_init(badge_wifi_config_t *config) {
    if (flight_mode) {
        Serial.println("wifi_init() - SKIPPED (flight mode)");
        return true;
    }
    
    Serial.println("wifi_init()");
    
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    
    if (config && config->auto_connect) {
        return wifi_connect(config->ssid, config->password);
    }
    
    Serial.println("  WiFi initialized (not connected)");
    return true;
}

bool wifi_connect(const char* ssid, const char* password) {
    if (flight_mode) {
        Serial.println("WiFi connect blocked - flight mode active");
        return false;
    }
    
    if (!ssid) return false;
    
    Serial.print("Connecting to WiFi: ");
    Serial.println(ssid);
    
    WiFi.begin(ssid, password);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    Serial.println();
    
    if (WiFi.status() == WL_CONNECTED) {
        strncpy(current_ssid, ssid, sizeof(current_ssid) - 1);
        IPAddress ip = WiFi.localIP();
        snprintf(current_ip, sizeof(current_ip), "%d.%d.%d.%d", 
                 ip[0], ip[1], ip[2], ip[3]);
        
        Serial.print("  WiFi connected! IP: ");
        Serial.println(current_ip);
        return true;
    } else {
        Serial.println("  WiFi connection failed");
        snprintf(last_error, sizeof(last_error), "Failed to connect to %s", ssid);
        return false;
    }
}

bool wifi_is_connected() {
    return WiFi.status() == WL_CONNECTED && !flight_mode;
}

void wifi_disconnect() {
    WiFi.disconnect();
    current_ssid[0] = '\0';
    current_ip[0] = '\0';
    Serial.println("WiFi disconnected");
}

void wifi_set_flight_mode(bool enabled) {
    flight_mode = enabled;
    if (enabled) {
        Serial.println("Flight mode ENABLED");
        if (wifi_is_connected()) {
            wifi_disconnect();
            WiFi.mode(WIFI_OFF);
        }
    } else {
        Serial.println("Flight mode DISABLED");
        WiFi.mode(WIFI_STA);
    }
}

bool wifi_get_flight_mode() {
    return flight_mode;
}

const char* wifi_get_ssid() {
    return current_ssid;
}

const char* wifi_get_ip() {
    return current_ip;
}

int wifi_scan_for_c3nav(wifi_scan_result_t* results, int max_results) {
    if (!results || max_results <= 0) return 0;
    if (flight_mode) {
        Serial.println("WiFi scan blocked - flight mode active");
        return 0;
    }
    
    Serial.println("Scanning WiFi networks for c3nav...");
    
    int n = WiFi.scanNetworks();
    if (n == 0) {
        Serial.println("  No networks found");
        return 0;
    }
    
    Serial.print("  Found ");
    Serial.print(n);
    Serial.println(" networks");
    
    int count = 0;
    for (int i = 0; i < n && count < max_results; i++) {
        // Get BSSID (MAC address)
        uint8_t* bssid = WiFi.BSSID(i);
        snprintf(results[count].bssid, sizeof(results[count].bssid),
                 "%02x:%02x:%02x:%02x:%02x:%02x",
                 bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);
        
        // Get SSID
        String ssid = WiFi.SSID(i);
        strncpy(results[count].ssid, ssid.c_str(), sizeof(results[count].ssid) - 1);
        results[count].ssid[sizeof(results[count].ssid) - 1] = '\0';
        
        // Get RSSI (signal strength)
        results[count].rssi = WiFi.RSSI(i);
        
        // Get frequency (channel to frequency conversion)
        int channel = WiFi.channel(i);
        if (channel <= 14) {
            // 2.4 GHz: Channel 1 = 2412 MHz, each channel = 5 MHz
            results[count].frequency = (2407 + channel * 5) * 1000;  // kHz
        } else {
            // 5 GHz approximation
            results[count].frequency = 5000000;  // Default 5 GHz
        }
        
        count++;
    }
    
    WiFi.scanDelete();
    
    Serial.print("  Collected ");
    Serial.print(count);
    Serial.println(" scan results");
    
    return count;
}

bool c3nav_init(c3nav_config_t *config) {
    if (!config) return false;
    
    Serial.println("c3nav_init()");
    Serial.print("  API URL: ");
    Serial.println(config->api_url);
    
    return true;
}

bool c3nav_locate(wifi_scan_result_t* scan_results, int num_results, c3nav_location_t* location) {
    if (!scan_results || num_results <= 0 || !location) {
        snprintf(last_error, sizeof(last_error), "Invalid parameters");
        return false;
    }
    
    if (!wifi_is_connected()) {
        snprintf(last_error, sizeof(last_error), "WiFi not connected");
        return false;
    }
    
    Serial.println("c3nav_locate()");
    
    HTTPClient http;
    http.begin(default_c3nav_config.api_url);
    http.addHeader("Content-Type", "application/json");
    
    // Build JSON request
    // Format: {"wifi_peers": [{"bssid": "aa:bb:cc:dd:ee:ff", "rssi": -50}, ...], "ibeacon_peers": []}
    DynamicJsonDocument doc(4096);
    JsonArray wifi_peers = doc.createNestedArray("wifi_peers");
    
    for (int i = 0; i < num_results; i++) {
        JsonObject peer = wifi_peers.createNestedObject();
        peer["bssid"] = scan_results[i].bssid;
        peer["rssi"] = scan_results[i].rssi;
        // Optional fields that c3nav can use
        if (strlen(scan_results[i].ssid) > 0) {
            peer["ssid"] = scan_results[i].ssid;
        }
    }
    
    // Empty ibeacon array (we don't have iBeacon scanning on ESP32 in this version)
    doc.createNestedArray("ibeacon_peers");
    
    String json_str;
    serializeJson(doc, json_str);
    
    Serial.print("  Request size: ");
    Serial.print(json_str.length());
    Serial.println(" bytes");
    
    // Send POST request
    int httpCode = http.POST(json_str);
    
    if (httpCode > 0) {
        Serial.print("  HTTP response: ");
        Serial.println(httpCode);
        
        if (httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            
            // Parse response
            DynamicJsonDocument response(2048);
            DeserializationError error = deserializeJson(response, payload);
            
            if (error) {
                snprintf(last_error, sizeof(last_error), "JSON parse error: %s", error.c_str());
                http.end();
                return false;
            }
            
            // Extract location data
            JsonObject loc = response["location"];
            if (loc.isNull()) {
                snprintf(last_error, sizeof(last_error), "No location returned");
                location->valid = false;
                http.end();
                return false;
            }
            
            location->valid = true;
            location->x = loc["x"] | 0.0f;
            location->y = loc["y"] | 0.0f;
            location->z = loc["z"] | 0.0f;
            location->level = loc["level"] | 0;
            
            // Space name (optional)
            const char* space = loc["space"];
            if (space) {
                strncpy(location->space_name_buf, space, sizeof(location->space_name_buf) - 1);
                location->space_name_buf[sizeof(location->space_name_buf) - 1] = '\0';
                location->space_name = location->space_name_buf;
            } else {
                location->space_name = "";
            }
            
            // Precision
            location->precision = response["precision"] | 0.0f;
            
            Serial.println("  Location found:");
            Serial.print("    Coordinates: (");
            Serial.print(location->x);
            Serial.print(", ");
            Serial.print(location->y);
            Serial.print(", ");
            Serial.print(location->z);
            Serial.println(")");
            Serial.print("    Precision: ");
            Serial.print(location->precision);
            Serial.println("m");
            
            http.end();
            return true;
        } else {
            snprintf(last_error, sizeof(last_error), "HTTP error %d", httpCode);
        }
    } else {
        snprintf(last_error, sizeof(last_error), "HTTP request failed: %s", http.errorToString(httpCode).c_str());
    }
    
    http.end();
    return false;
}

const char* c3nav_get_last_error() {
    return last_error;
}
