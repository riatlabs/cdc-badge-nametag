#include "app.h"
#include "badge.h"
#include "display_ui.h"
#include "display.h"
#include <Arduino.h>

// External display reference
extern GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display;

void app_setup(app_state_t *state) {
    if (!state) return;
    
    memset(state, 0, sizeof(app_state_t));
    
    state->display_mode = DISPLAY_MODE_NAMETAG;
    state->is_initialized = true;
    state->display_needs_update = true;
    state->telemetry_page = 0;
    state->auto_rotate_pages = true;
    
    // Initialize subsystems
    telemetry_init();
    tropic_demo_init(&state->tropic_demo);
    ui_init();
    
    // Initial telemetry reading
    telemetry_update(&state->telemetry);
    state->telemetry.tropic01_active = true;
    state->telemetry.display_mode = state->display_mode;
    
    Serial.println("App initialized");
}

void app_switch_mode(app_state_t *state, display_mode_t mode) {
    if (!state || state->display_mode == mode) return;
    
    state->display_mode = mode;
    state->telemetry.display_mode = mode;
    state->display_needs_update = true;
    state->telemetry_page = 0;  // Reset to first page
    
    Serial.print("Switched to mode: ");
    Serial.println(mode);
}

void app_next_page(app_state_t *state) {
    if (!state || state->display_mode != DISPLAY_MODE_TELEMETRY) return;
    
    state->telemetry_page++;
    state->display_needs_update = true;
    state->last_page_rotation = millis();
    
    Serial.print("Next page: ");
    Serial.println(state->telemetry_page);
}

void app_prev_page(app_state_t *state) {
    if (!state || state->display_mode != DISPLAY_MODE_TELEMETRY) return;
    
    if (state->telemetry_page > 0) {
        state->telemetry_page--;
    } else {
        state->telemetry_page = 1;  // Wrap to last page
    }
    state->display_needs_update = true;
    state->last_page_rotation = millis();
    
    Serial.print("Prev page: ");
    Serial.println(state->telemetry_page);
}

void app_update_display(app_state_t *state) {
    if (!state || !state->display_needs_update) return;
    
    uint32_t now = millis();
    
    // Rate limiting
    if (now - state->last_display_update < DISPLAY_UPDATE_INTERVAL) {
        return;
    }
    
    display.setPartialWindow(0, 0, display.width(), display.height());
    display.firstPage();
    
    do {
        switch (state->display_mode) {
            case DISPLAY_MODE_NAMETAG:
                ui_draw_nametag(display);
                break;
                
            case DISPLAY_MODE_TELEMETRY:
                ui_draw_telemetry(display, &state->telemetry, state->telemetry_page);
                break;
                
            case DISPLAY_MODE_TROPIC_DEMO:
                ui_draw_tropic_demo(display, tropic_demo_get_status(&state->tropic_demo));
                break;
        }
    } while (display.nextPage());
    
    state->display_needs_update = false;
    state->last_display_update = now;
    
    Serial.println("Display updated");
}

void app_handle_number_button(app_state_t *state, char button) {
    if (!state) return;
    
    Serial.print("Number button: ");
    Serial.println(button);
    
    switch (button) {
        case '0':
            app_switch_mode(state, DISPLAY_MODE_NAMETAG);
            break;
        case '1':
            app_switch_mode(state, DISPLAY_MODE_TELEMETRY);
            break;
        case '2':
            app_switch_mode(state, DISPLAY_MODE_TROPIC_DEMO);
            break;
        case '3':
            app_next_page(state);
            break;
        case '6':
            app_prev_page(state);
            break;
        default:
            break;
    }
}

void app_handle_yn_button(app_state_t *state, char button) {
    if (!state) return;
    
    Serial.print("Y/N button: ");
    Serial.println(button);
    
    if (state->display_mode == DISPLAY_MODE_TROPIC_DEMO) {
        if (button == 'Y') {
            switch (state->demo_step) {
                case 0:
                    // Erase key first to ensure clean state
                    tropic01.eccKeyErase(TROPIC_DEMO_KEY_SLOT);
                    delay(100);
                    
                    if (tropic_demo_generate_key(&state->tropic_demo, tropic01)) {
                        state->demo_step = 1;
                        state->display_needs_update = true;
                    }
                    break;
                case 1:
                    if (tropic_demo_sign_message(&state->tropic_demo, tropic01, "39C3 CDC Badge Demo")) {
                        state->demo_step = 2;
                        state->display_needs_update = true;
                    }
                    break;
                case 2:
                    if (tropic_demo_erase_key(&state->tropic_demo, tropic01)) {
                        state->demo_step = 0;
                        state->display_needs_update = true;
                    }
                    break;
            }
        } else if (button == 'N') {
            if (state->demo_step == 2) {
                tropic_demo_verify_signature(&state->tropic_demo);
                state->display_needs_update = true;
            }
        }
    } else {
        if (button == 'Y') {
            if (state->display_mode == DISPLAY_MODE_NAMETAG) {
                app_switch_mode(state, DISPLAY_MODE_TELEMETRY);
            } else {
                app_switch_mode(state, DISPLAY_MODE_NAMETAG);
            }
        }
    }
}

void app_loop(app_state_t *state) {
    if (!state) return;
    
    uint32_t now = millis();
    
    // Update telemetry at regular intervals
    if (now - state->last_telemetry_update >= TELEMETRY_UPDATE_INTERVAL) {
        telemetry_update(&state->telemetry);
        state->last_telemetry_update = now;
        
        if (state->display_mode == DISPLAY_MODE_TELEMETRY) {
            state->display_needs_update = true;
        }
    }
    
    // Auto-rotate pages in telemetry mode
    if (state->display_mode == DISPLAY_MODE_TELEMETRY && 
        state->auto_rotate_pages &&
        now - state->last_page_rotation >= PAGE_ROTATION_INTERVAL) {
        
        state->telemetry_page++;
        state->display_needs_update = true;
        state->last_page_rotation = now;
    }
    
    // Process keyboard input
    char pressed = pin_expander_process_irq();
    if (pressed != 'x') {
        if (pressed >= '0' && pressed <= '9') {
            app_handle_number_button(state, pressed);
        } else if (pressed == 'Y' || pressed == 'N') {
            app_handle_yn_button(state, pressed);
        }
    }
    
    // Handle power button
    powerdown_process_irq();
    
    // Update display if needed
    app_update_display(state);
}
