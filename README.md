# CDC Badge Firmware Update Package

## What's Included

This package contains all modified files for the CDC badge firmware with:
- ✅ 4-page telemetry display with headers (POWER, SYSTEM, OVERVIEW, C3NAV)
- ✅ Grid layout (no text overflow)
- ✅ Button 3/6 navigation
- ✅ Auto-rotating pages (10s interval)
- ✅ Fixed TROPIC01 key generation
- ✅ WiFi connectivity
- ✅ c3nav indoor positioning

## Quick Install

```bash
# 1. Unzip in your project root
cd cdc-badge-nametag
unzip cdc-badge-firmware-update.zip

# 2. Copy files to your project
cp -r cdc-badge-update/src/* src/
cp -r cdc-badge-update/lib/cdc-badge/* lib/cdc-badge/

# 3. Update platformio.ini
# Add this line to lib_deps:
#   bblanchon/ArduinoJson@^7.2.1

# 4. Build and flash
pio run -t upload
```

## Files Changed

### src/ (3 files)
- `app.h` - Added page navigation, WiFi tracking
- `app.cpp` - 4-page rotation, button 3/6 support
- `main.cpp` - No changes from working version

### lib/cdc-badge/ (8 files)
- `display_ui.h` - 4-page telemetry declarations
- `display_ui.cpp` - Headers, grid layout, c3nav page
- `telemetry.h` - Added c3nav location fields
- `telemetry.cpp` - Battery/system telemetry
- `tropic_demo.h` - Crypto demo interface
- `tropic_demo.cpp` - Ed25519 sign/verify
- `wifi_config.h` - NEW: WiFi and c3nav API
- `wifi_config.cpp` - NEW: WiFi/HTTP implementation

## Documentation

- **INSTALL.md** - Quick installation steps
- **BUTTON_MANUAL.md** - Button reference card
- **V2_IMPROVEMENTS.md** - Display improvements explained
- **WIFI_C3NAV_GUIDE.md** - Complete WiFi/c3nav guide
- **platformio_additions.ini** - PlatformIO changes needed

## What Changed

### Display
- Added page headers (POWER, SYSTEM, OVERVIEW, C3NAV)
- Page indicators (1/4, 2/4, 3/4, 4/4)
- Grid layout with better spacing
- 4th page for c3nav positioning

### Navigation
- Button 3: Next page
- Button 6: Previous page
- Auto-rotation every 10 seconds

### WiFi (Optional)
- Auto-connects to 39C3-Open
- Scans WiFi for c3nav positioning
- Displays coordinates on page 4/4
- Configurable in wifi_config.cpp

### TROPIC01
- Fixed key generation (erases before generating)
- Works reliably every time

## Button Map

```
0 → Nametag
1 → Telemetry (4 pages, auto-rotating)
2 → TROPIC01 Demo
3 → Next page
6 → Previous page
Y → Toggle nametag/telemetry (or demo action)
N → Verify signature
```

## Configuration

### WiFi Network
Edit `lib/cdc-badge/wifi_config.cpp`:
```cpp
wifi_config_t default_wifi_config = {
    .ssid = "39C3-Open",        // Change SSID here
    .password = "",              // Add password if needed
    .auto_connect = true         // Set false to disable
};
```

### c3nav API
```cpp
c3nav_config_t default_c3nav_config = {
    .api_url = "https://39c3.c3nav.de/api/positioning/locate/",
    .enabled = true,             // Set false to disable
    .scan_interval_ms = 30000    // 30 seconds between scans
};
```

## Dependencies

Add to `platformio.ini`:
```ini
lib_deps = 
    https://github.com/tropicsquare/libtropic-arduino.git
    robtillaart/TCA9555@^0.4.3
    zinggjm/GxEPD2 @ ^1.6.5
    adafruit/Adafruit GFX Library @ ^1.12.4
    bblanchon/ArduinoJson@^7.2.1  # <-- ADD THIS LINE
```

## Telemetry Pages

### Page 1: POWER (1/4)
```
POWER                       1/4
────────────────────────────────
[🔋] 4.12V          85%
Fast                450mA
[USB] USB           3.85V
```

### Page 2: SYSTEM (2/4)
```
SYSTEM                      2/4
────────────────────────────────
[🌡] 38.5C          [💾] 142K
[🕐] 02:34:15
[🔒] Active
```

### Page 3: OVERVIEW (3/4)
```
OVERVIEW                    3/4
────────────────────────────────
[🔋] 4.12V 85%
[🌡] 38.5C          [💾] 142K
[🕐] 02:34:15
[USB] USB           [🔒] SE:OK
SYS:3.85V           450mA
```

### Page 4: C3NAV (4/4)
```
C3NAV                       4/4
────────────────────────────────
WiFi: Connected
X: 123.4m
Y: 678.9m
Level: 0
Precision: 5.2m
```

## Troubleshooting

### Display not updating
- Check serial monitor: `pio device monitor -b 115200`
- Should see "Display updated" message
- Verify files copied correctly

### WiFi not connecting
- Open network: No password needed
- Check SSID is exactly "39C3-Open" (case-sensitive)
- See WIFI_C3NAV_GUIDE.md for details

### c3nav shows "Unknown"
- WiFi must be connected first
- Needs 3+ access points in range
- Scan interval is 30 seconds
- Check API URL matches event

### TROPIC01 key gen fails
- This is fixed in this update
- Explicitly erases key before generating
- Should work reliably now

## Support

Read the detailed guides:
1. **INSTALL.md** - Installation steps
2. **V2_IMPROVEMENTS.md** - What changed and why
3. **WIFI_C3NAV_GUIDE.md** - WiFi troubleshooting and config
4. **BUTTON_MANUAL.md** - Quick button reference

## Version

CDC Badge Firmware Update - December 2025
- Telemetry V2 with 4 pages
- WiFi & c3nav integration
- TROPIC01 fixes
- Grid layout improvements

Enjoy your upgraded CDC badge! 🎉
