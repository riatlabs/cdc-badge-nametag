#include "tropic_demo.h"
#include "psa/crypto.h"

void tropic_demo_init(tropic_demo_data_t *demo) {
    if (!demo) return;
    
    memset(demo, 0, sizeof(tropic_demo_data_t));
    demo->state = TROPIC_DEMO_IDLE;
    strcpy(demo->message, "39C3 CDC Badge");
    strcpy(demo->status_text, "Ready for demo");
}

bool tropic_demo_generate_key(tropic_demo_data_t *demo, Tropic01 &tropic01) {
    if (!demo) return false;
    
    demo->state = TROPIC_DEMO_KEYGEN;
    uint32_t start = millis();
    
    lt_ret_t ret = tropic01.eccKeyGenerate(TROPIC_DEMO_KEY_SLOT, TR01_CURVE_ED25519);
    if (ret != LT_OK) {
        snprintf(demo->status_text, sizeof(demo->status_text),
                 "Key gen failed: %d", ret);
        return false;
    }
    
    // Read public key
    lt_ecc_curve_type_t curve;
    lt_ecc_key_origin_t origin;
    ret = tropic01.eccKeyRead(TROPIC_DEMO_KEY_SLOT, demo->public_key, 
                              sizeof(demo->public_key), curve, origin);
    if (ret != LT_OK) {
        snprintf(demo->status_text, sizeof(demo->status_text),
                 "Key read failed: %d", ret);
        return false;
    }
    
    demo->operation_time_ms = millis() - start;
    
    snprintf(demo->status_text, sizeof(demo->status_text),
             "Ed25519 key generated in %lums", demo->operation_time_ms);
    
    return true;
}

bool tropic_demo_sign_message(tropic_demo_data_t *demo, Tropic01 &tropic01, const char *msg) {
    if (!demo || !msg) return false;
    
    demo->state = TROPIC_DEMO_SIGN;
    strncpy(demo->message, msg, sizeof(demo->message) - 1);
    
    uint32_t start = millis();
    
    lt_ret_t ret = tropic01.eddsaSign(TROPIC_DEMO_KEY_SLOT, 
                                      (const uint8_t*)demo->message,
                                      strlen(demo->message),
                                      demo->signature);
    if (ret != LT_OK) {
        snprintf(demo->status_text, sizeof(demo->status_text),
                 "Signing failed: %d", ret);
        return false;
    }
    
    demo->operation_time_ms = millis() - start;
    
    snprintf(demo->status_text, sizeof(demo->status_text),
             "Signed in %lums. Sig: %.4s...", 
             demo->operation_time_ms,
             demo->signature);
    
    return true;
}

bool tropic_demo_verify_signature(tropic_demo_data_t *demo) {
    if (!demo) return false;
    
    demo->state = TROPIC_DEMO_VERIFY;
    uint32_t start = millis();
    
    // PSA Crypto verification for Ed25519
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_id_t key_id = 0;
    psa_status_t status;
    
    // Set key attributes for Ed25519
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_VERIFY_MESSAGE);
    psa_set_key_algorithm(&attributes, PSA_ALG_PURE_EDDSA);
    psa_set_key_type(&attributes, PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_TWISTED_EDWARDS));
    psa_set_key_bits(&attributes, 255);
    
    // Import public key
    status = psa_import_key(&attributes, demo->public_key, 32, &key_id);
    if (status != PSA_SUCCESS) {
        psa_reset_key_attributes(&attributes);
        snprintf(demo->status_text, sizeof(demo->status_text),
                 "Key import failed: %d", status);
        return false;
    }
    
    // Verify signature
    status = psa_verify_message(key_id, PSA_ALG_PURE_EDDSA,
                                (const uint8_t*)demo->message, strlen(demo->message),
                                demo->signature, 64);
    
    demo->operation_time_ms = millis() - start;
    demo->signature_valid = (status == PSA_SUCCESS);
    
    if (demo->signature_valid) {
        snprintf(demo->status_text, sizeof(demo->status_text),
                 "Verified in %lums: VALID", demo->operation_time_ms);
        demo->state = TROPIC_DEMO_COMPLETE;
    } else {
        snprintf(demo->status_text, sizeof(demo->status_text),
                 "Verification failed: %d", status);
    }
    
    // Cleanup
    psa_destroy_key(key_id);
    psa_reset_key_attributes(&attributes);
    
    return demo->signature_valid;
}

bool tropic_demo_erase_key(tropic_demo_data_t *demo, Tropic01 &tropic01) {
    if (!demo) return false;
    
    lt_ret_t ret = tropic01.eccKeyErase(TROPIC_DEMO_KEY_SLOT);
    if (ret != LT_OK) {
        snprintf(demo->status_text, sizeof(demo->status_text),
                 "Key erase failed: %d", ret);
        return false;
    }
    
    // Reset demo state
    memset(demo->public_key, 0, sizeof(demo->public_key));
    memset(demo->signature, 0, sizeof(demo->signature));
    demo->state = TROPIC_DEMO_IDLE;
    strcpy(demo->status_text, "Key erased, ready");
    
    return true;
}

const char* tropic_demo_get_status(tropic_demo_data_t *demo) {
    return demo ? demo->status_text : "Demo not initialized";
}
