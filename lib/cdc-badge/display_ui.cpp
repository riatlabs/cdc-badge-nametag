#include "display_ui.h"
#include "badge.h"
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>

// Battery icons (16x16, stored in PROGMEM)
const uint8_t PROGMEM icon_battery_empty[] = {
    0x7F, 0xF8, 0xFF, 0xFC, 0xC0, 0x06, 0xC0, 0x07, 
    0xC0, 0x07, 0xC0, 0x07, 0xC0, 0x07, 0xC0, 0x07,
    0xC0, 0x07, 0xC0, 0x07, 0xC0, 0x07, 0xC0, 0x07,
    0xC0, 0x06, 0xFF, 0xFC, 0x7F, 0xF8, 0x00, 0x00
};

const uint8_t PROGMEM icon_battery_low[] = {
    0x7F, 0xF8, 0xFF, 0xFC, 0xC0, 0x06, 0xC0, 0x07, 
    0xC0, 0x07, 0xC0, 0x07, 0xC0, 0x07, 0xC0, 0x07,
    0xC0, 0x07, 0xC0, 0x07, 0xC3, 0xC7, 0xC3, 0xC7,
    0xC3, 0xC6, 0xFF, 0xFC, 0x7F, 0xF8, 0x00, 0x00
};

const uint8_t PROGMEM icon_battery_medium[] = {
    0x7F, 0xF8, 0xFF, 0xFC, 0xC0, 0x06, 0xC0, 0x07, 
    0xC0, 0x07, 0xC7, 0x07, 0xC7, 0x07, 0xC7, 0x07,
    0xC7, 0x07, 0xC7, 0x07, 0xC7, 0xC7, 0xC7, 0xC7,
    0xC7, 0xC6, 0xFF, 0xFC, 0x7F, 0xF8, 0x00, 0x00
};

const uint8_t PROGMEM icon_battery_full[] = {
    0x7F, 0xF8, 0xFF, 0xFC, 0xDF, 0xF6, 0xDF, 0xF7, 
    0xDF, 0xF7, 0xDF, 0xF7, 0xDF, 0xF7, 0xDF, 0xF7,
    0xDF, 0xF7, 0xDF, 0xF7, 0xDF, 0xF7, 0xDF, 0xF7,
    0xDF, 0xF6, 0xFF, 0xFC, 0x7F, 0xF8, 0x00, 0x00
};

const uint8_t PROGMEM icon_battery_charging[] = {
    0x7F, 0xF8, 0xFF, 0xFC, 0xC3, 0x06, 0xC7, 0x87, 
    0xC7, 0x87, 0xC0, 0xF7, 0xC0, 0xF7, 0xC1, 0xE7,
    0xC3, 0xC7, 0xC7, 0x87, 0xCF, 0x07, 0xCF, 0x07,
    0xDE, 0x06, 0xFF, 0xFC, 0x7F, 0xF8, 0x00, 0x00
};

const uint8_t PROGMEM icon_usb[] = {
    0x01, 0x80, 0x01, 0x80, 0x03, 0xC0, 0x06, 0x60,
    0x0C, 0x30, 0x0C, 0x30, 0x0C, 0x30, 0x06, 0x60,
    0x03, 0xC0, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80,
    0x01, 0x80, 0x03, 0xC0, 0x03, 0xC0, 0x00, 0x00
};

const uint8_t PROGMEM icon_temperature[] = {
    0x01, 0x80, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40,
    0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40,
    0x06, 0x60, 0x0F, 0xF0, 0x0F, 0xF0, 0x0F, 0xF0,
    0x0F, 0xF0, 0x06, 0x60, 0x01, 0x80, 0x00, 0x00
};

const uint8_t PROGMEM icon_memory[] = {
    0xFF, 0xFF, 0x80, 0x01, 0x9F, 0xF9, 0x90, 0x09,
    0x9F, 0xF9, 0x90, 0x09, 0x9F, 0xF9, 0x90, 0x09,
    0x9F, 0xF9, 0x90, 0x09, 0x9F, 0xF9, 0x90, 0x09,
    0x9F, 0xF9, 0x80, 0x01, 0xFF, 0xFF, 0x00, 0x00
};

const uint8_t PROGMEM icon_secure[] = {
    0x03, 0xC0, 0x04, 0x20, 0x08, 0x10, 0x08, 0x10,
    0x08, 0x10, 0x08, 0x10, 0x3F, 0xFC, 0x20, 0x04,
    0x20, 0x04, 0x27, 0xE4, 0x24, 0x24, 0x24, 0x24,
    0x27, 0xE4, 0x20, 0x04, 0x3F, 0xFC, 0x00, 0x00
};

const uint8_t PROGMEM icon_clock[] = {
    0x07, 0xE0, 0x18, 0x18, 0x20, 0x04, 0x47, 0xE2,
    0x48, 0x12, 0x90, 0x09, 0x90, 0x09, 0x90, 0x09,
    0x90, 0x09, 0x48, 0x12, 0x47, 0xE2, 0x20, 0x04,
    0x18, 0x18, 0x07, 0xE0, 0x00, 0x00, 0x00, 0x00
};

void ui_init() {
    // UI initialization if needed
}

void ui_draw_icon(GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> &display,
                  const uint8_t *icon, int16_t x, int16_t y) {
    display.drawBitmap(x, y, icon, ICON_WIDTH, ICON_HEIGHT, GxEPD_WHITE);
}

const uint8_t* ui_get_battery_icon(uint8_t percentage, charge_status_t status) {
    if (status == CHARGE_STATUS_FASTCHARGE || status == CHARGE_STATUS_PRECHARGE) {
        return icon_battery_charging;
    }
    
    if (percentage >= 75) return icon_battery_full;
    if (percentage >= 40) return icon_battery_medium;
    if (percentage >= 15) return icon_battery_low;
    return icon_battery_empty;
}

void ui_print_right(GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> &display,
                    const char* text, int16_t x, int16_t y) {
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    display.setCursor(x - w, y);
    display.print(text);
}

void ui_print_center(GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> &display,
                     const char* text, int16_t x, int16_t y) {
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    display.setCursor(x - w/2, y);
    display.print(text);
}

void ui_draw_nametag(GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> &display) {
    display.fillScreen(GxEPD_BLACK);
    display.setTextColor(GxEPD_WHITE);
    
    // Draw logo
    int16_t logo_x = 20, logo_y = 15;
    for (int i = 0; i < 3; i++) {
        int16_t radius = (i + 1) * 7;
        display.drawCircle(logo_x, logo_y + 20, radius, GxEPD_WHITE);
        display.fillRect(logo_x - radius - 1, logo_y + 20 - radius, radius + 1, radius * 2, GxEPD_BLACK);
    }
    display.fillCircle(logo_x, logo_y + 20, 3, GxEPD_WHITE);
    
    // Event title
    display.setFont(&FreeSansBold9pt7b);
    display.setCursor(70, 30);
    display.print("39C3");
    
    // Tagline
    display.setFont(&FreeSans9pt7b);
    display.setCursor(70, 50);
    display.print("Don't trust, verify!");
    
    // Organization
    display.setCursor(20, 70);
    display.print("decentral.community");
}

void ui_draw_telemetry_power(GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> &display,
                             telemetry_data_t *data) {
    if (!data) return;
    
    display.fillScreen(GxEPD_BLACK);
    display.setTextColor(GxEPD_WHITE);
    
    char buffer[32];
    
    // Header
    display.setFont(&FreeSansBold9pt7b);
    display.setCursor(10, 15);
    display.print("POWER");
    
    // Page indicator
    display.setFont(&FreeSans9pt7b);
    ui_print_right(display, "1/3", 286, 15);
    
    display.setFont(&FreeSans9pt7b);
    
    int16_t col1_x = 10, col2_x = 160;
    int16_t row_height = 30;
    int16_t y = 45;
    
    // Row 1: Battery voltage | Battery %
    const uint8_t *bat_icon = ui_get_battery_icon(data->battery_percentage, data->charge_status);
    ui_draw_icon(display, bat_icon, col1_x, y - 12);
    snprintf(buffer, sizeof(buffer), "%.2fV", data->battery_voltage);
    display.setCursor(col1_x + 20, y);
    display.print(buffer);
    
    snprintf(buffer, sizeof(buffer), "%d%%", data->battery_percentage);
    display.setCursor(col2_x, y);
    display.print(buffer);
    y += row_height;
    
    // Row 2: Charge status | Current
    const char* status_str = "";
    switch (data->charge_status) {
        case CHARGE_STATUS_NOT_CHARGING: status_str = "---"; break;
        case CHARGE_STATUS_PRECHARGE: status_str = "Pre"; break;
        case CHARGE_STATUS_FASTCHARGE: status_str = "Fast"; break;
        case CHARGE_STATUS_DONE: status_str = "Done"; break;
    }
    display.setCursor(col1_x, y);
    display.print(status_str);
    
    snprintf(buffer, sizeof(buffer), "%umA", data->charge_current_ma);
    display.setCursor(col2_x, y);
    display.print(buffer);
    y += row_height;
    
    // Row 3: USB | System voltage
    if (data->usb_connected) {
        ui_draw_icon(display, icon_usb, col1_x, y - 12);
        display.setCursor(col1_x + 20, y);
        display.print("USB");
    }
    
    snprintf(buffer, sizeof(buffer), "%.2fV", data->system_voltage);
    display.setCursor(col2_x, y);
    display.print(buffer);
}

void ui_draw_telemetry_system(GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> &display,
                              telemetry_data_t *data) {
    if (!data) return;
    
    display.fillScreen(GxEPD_BLACK);
    display.setTextColor(GxEPD_WHITE);
    
    char buffer[32];
    
    // Header
    display.setFont(&FreeSansBold9pt7b);
    display.setCursor(10, 15);
    display.print("SYSTEM");
    
    // Page indicator
    display.setFont(&FreeSans9pt7b);
    ui_print_right(display, "2/3", 286, 15);
    
    display.setFont(&FreeSans9pt7b);
    
    int16_t col1_x = 10, col2_x = 160;
    int16_t row_height = 30;
    int16_t y = 45;
    
    // Row 1: Temperature | Heap
    ui_draw_icon(display, icon_temperature, col1_x, y - 12);
    snprintf(buffer, sizeof(buffer), "%.1fC", data->temperature_celsius);
    display.setCursor(col1_x + 20, y);
    display.print(buffer);
    
    ui_draw_icon(display, icon_memory, col2_x, y - 12);
    snprintf(buffer, sizeof(buffer), "%luK", data->free_heap_bytes / 1024);
    display.setCursor(col2_x + 20, y);
    display.print(buffer);
    y += row_height;
    
    // Row 2: Uptime
    ui_draw_icon(display, icon_clock, col1_x, y - 12);
    telemetry_format_uptime(data->uptime_seconds, buffer, sizeof(buffer));
    display.setCursor(col1_x + 20, y);
    display.print(buffer);
    y += row_height;
    
    // Row 3: TROPIC01 status
    ui_draw_icon(display, icon_secure, col1_x, y - 12);
    display.setCursor(col1_x + 20, y);
    display.print(data->tropic01_active ? "Active" : "Inactive");
}

void ui_draw_telemetry_overview(GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> &display,
                                telemetry_data_t *data) {
    if (!data) return;
    
    display.fillScreen(GxEPD_BLACK);
    display.setTextColor(GxEPD_WHITE);
    
    char buffer[32];
    
    // Header
    display.setFont(&FreeSansBold9pt7b);
    display.setCursor(10, 15);
    display.print("OVERVIEW");
    
    // Page indicator
    display.setFont(&FreeSans9pt7b);
    ui_print_right(display, "3/3", 286, 15);
    
    display.setFont(&FreeSans9pt7b);
    
    int16_t y = 40;
    int16_t line_height = 22;
    
    // Compact overview format
    // Battery
    const uint8_t *bat_icon = ui_get_battery_icon(data->battery_percentage, data->charge_status);
    ui_draw_icon(display, bat_icon, 10, y - 12);
    snprintf(buffer, sizeof(buffer), "%.2fV %d%%", data->battery_voltage, data->battery_percentage);
    display.setCursor(30, y);
    display.print(buffer);
    y += line_height;
    
    // Temperature + Memory
    ui_draw_icon(display, icon_temperature, 10, y - 12);
    snprintf(buffer, sizeof(buffer), "%.1fC", data->temperature_celsius);
    display.setCursor(30, y);
    display.print(buffer);
    
    ui_draw_icon(display, icon_memory, 150, y - 12);
    snprintf(buffer, sizeof(buffer), "%luK", data->free_heap_bytes / 1024);
    display.setCursor(170, y);
    display.print(buffer);
    y += line_height;
    
    // Uptime
    ui_draw_icon(display, icon_clock, 10, y - 12);
    telemetry_format_uptime(data->uptime_seconds, buffer, sizeof(buffer));
    display.setCursor(30, y);
    display.print(buffer);
    y += line_height;
    
    // USB + Secure element
    if (data->usb_connected) {
        ui_draw_icon(display, icon_usb, 10, y - 12);
        display.setCursor(30, y);
        display.print("USB");
    }
    
    ui_draw_icon(display, icon_secure, 150, y - 12);
    display.setCursor(170, y);
    display.print(data->tropic01_active ? "SE:OK" : "SE:--");
    y += line_height;
    
    // System voltage + Charge current
    snprintf(buffer, sizeof(buffer), "SYS:%.2fV", data->system_voltage);
    display.setCursor(10, y);
    display.print(buffer);
    
    if (data->charge_current_ma > 0) {
        snprintf(buffer, sizeof(buffer), "%umA", data->charge_current_ma);
        display.setCursor(150, y);
        display.print(buffer);
    }
}

void ui_draw_telemetry(GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> &display,
                       telemetry_data_t *data, uint8_t page) {
    switch (page % 3) {
        case 0:
            ui_draw_telemetry_power(display, data);
            break;
        case 1:
            ui_draw_telemetry_system(display, data);
            break;
        case 2:
            ui_draw_telemetry_overview(display, data);
            break;
    }
}

void ui_draw_tropic_demo(GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> &display,
                         const char *demo_text) {
    display.fillScreen(GxEPD_BLACK);
    display.setTextColor(GxEPD_WHITE);
    
    // Title
    display.setFont(&FreeSansBold9pt7b);
    display.setCursor(10, 15);
    display.print("TROPIC01");
    
    display.setFont(&FreeSans9pt7b);
    
    if (demo_text && strlen(demo_text) > 0) {
        int16_t y = 40;
        int16_t line_height = 18;
        char line_buffer[40];
        const char *ptr = demo_text;
        
        while (*ptr && y < 115) {
            int chars = 0;
            const char *space = ptr;
            const char *last_space = ptr;
            
            while (*space && chars < 35) {
                if (*space == ' ' || *space == '\n') {
                    last_space = space;
                }
                space++;
                chars++;
            }
            
            if (last_space != ptr && *last_space) {
                chars = last_space - ptr;
            }
            
            if (chars > 0) {
                strncpy(line_buffer, ptr, chars);
                line_buffer[chars] = '\0';
                
                display.setCursor(10, y);
                display.print(line_buffer);
                y += line_height;
                
                ptr += chars;
                if (*ptr == ' ' || *ptr == '\n') ptr++;
            } else {
                break;
            }
        }
    } else {
        display.setCursor(10, 50);
        display.print("Y: Generate key");
        display.setCursor(10, 70);
        display.print("Y: Sign message");
        display.setCursor(10, 90);
        display.print("N: Verify signature");
    }
}
