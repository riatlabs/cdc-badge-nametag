#ifndef TROPIC_DEMO_H
#define TROPIC_DEMO_H

#include <Arduino.h>
#include <LibtropicArduino.h>

// Demo state enumeration
typedef enum {
    TROPIC_DEMO_IDLE = 0,
    TROPIC_DEMO_KEYGEN,
    TROPIC_DEMO_SIGN,
    TROPIC_DEMO_VERIFY,
    TROPIC_DEMO_COMPLETE
} tropic_demo_state_t;

// Demo data structure
typedef struct {
    tropic_demo_state_t state;
    uint8_t public_key[64];  // Ed25519 public key
    uint8_t signature[64];   // Signature
    char message[128];       // Message to sign
    char status_text[256];   // Status message for display
    bool signature_valid;
    uint32_t operation_time_ms;
} tropic_demo_data_t;

// Demo slot for Ed25519 key
#define TROPIC_DEMO_KEY_SLOT TR01_ECC_SLOT_1

// Initialize TROPIC01 demo
void tropic_demo_init(tropic_demo_data_t *demo);

// Generate Ed25519 key pair
bool tropic_demo_generate_key(tropic_demo_data_t *demo, Tropic01 &tropic01);

// Sign message with Ed25519
bool tropic_demo_sign_message(tropic_demo_data_t *demo, Tropic01 &tropic01, const char *msg);

// Verify signature (host-side verification)
bool tropic_demo_verify_signature(tropic_demo_data_t *demo);

// Erase demo key
bool tropic_demo_erase_key(tropic_demo_data_t *demo, Tropic01 &tropic01);

// Get status text for display
const char* tropic_demo_get_status(tropic_demo_data_t *demo);

#endif // TROPIC_DEMO_H
