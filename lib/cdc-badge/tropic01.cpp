#include <Arduino.h>
#include "spi_bus.h"
#include <LibtropicArduino.h>
#include "psa/crypto.h"
#include "spi_bus.h"
#include "badge.h"

// Pairing Key macros for establishing a Secure Channel Session with TROPIC01
#define PAIRING_KEY_PRIV sh0priv_prod0
#define PAIRING_KEY_PUB sh0pub_prod0
#define PAIRING_KEY_SLOT TR01_PAIRING_KEY_SLOT_INDEX_0

// Global TROPIC01 instance (accessible from app)
Tropic01 tropic01(TR01_CS_PIN, esp_random(), SPI_Bus);

lt_ret_t returnVal;
psa_status_t mbedtlsInitStatus;

void errorHandler(void) {
  Serial.println("Starting cleanup...");
  tropic01.end();
  mbedtls_psa_crypto_free();
  Serial.println("Cleanup finished, entering infinite loop...");
  while (true);
}

bool tropic01_init() {
  Serial.println("tropic01_init()");
  
  Serial.println("  Initializing MbedTLS PSA Crypto...");
  psa_status_t mbedtlsInitStatus = psa_crypto_init();
  if (mbedtlsInitStatus != PSA_SUCCESS) {
    Serial.print("  ERROR: MbedTLS's PSA Crypto initialization failed, psa_status_t=");
    Serial.println(mbedtlsInitStatus);
    return false;
  }

  Serial.println("  Initializing Tropic01 resources...");
  returnVal = tropic01.begin();

  if (returnVal != LT_OK) {
    Serial.print("  ERROR: Tropic01.begin() failed, returnVal=");
    Serial.println(returnVal);
    return false;
  }

  return true;
}

bool tropic01_secure_session_start() {
  Serial.println("tropic01_secure_session_start()");

  Serial.println("  Starting Secure Channel ...");
  returnVal = tropic01.secureSessionStart(PAIRING_KEY_PRIV, PAIRING_KEY_PUB,
                                          PAIRING_KEY_SLOT);
  if (returnVal != LT_OK) {
    Serial.print("  ERROR: Tropic01.secureSessionStart() failed, returnVal=");
    Serial.println(returnVal);
    return false;
  }

  return true;
}
