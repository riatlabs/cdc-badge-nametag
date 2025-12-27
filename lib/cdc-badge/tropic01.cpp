#include <Arduino.h>
#include "spi_bus.h"
// LibtropicArduino library.
#include <LibtropicArduino.h>
// MbedTLS's PSA Crypto library.
#include "psa/crypto.h"
#include "spi_bus.h"
#include "badge.h"

// Pairing Key macros for establishing a Secure Channel Session with TROPIC01.
// Using the default Pairing Key slot 0 of Production TROPIC01 chips.
#define PAIRING_KEY_PRIV sh0priv_prod0
#define PAIRING_KEY_PUB sh0pub_prod0
#define PAIRING_KEY_SLOT TR01_PAIRING_KEY_SLOT_INDEX_0

// Constructor: csPin, rngSeed (use ESP32 hardware RNG), spiInstance
Tropic01 tropic01(TR01_CS_PIN, esp_random(), SPI_Bus);
lt_ret_t returnVal;
psa_status_t mbedtlsInitStatus;

// Used when some error occurs
void errorHandler(void) {
  Serial.println("Starting cleanup...");
  tropic01.end(); // Aborts all communication with TROPIC01 and frees resources.
  mbedtls_psa_crypto_free(); // Frees MbedTLS's PSA Crypto resources.

  Serial.println("Cleanup finished, entering infinite loop...");
  while (true)
    ;
}

bool tropic01_init() {
  Serial.println("tropic01_init()");
  // Init MbedTLS's PSA Crypto.
  Serial.println("  Initializing MbedTLS PSA Crypto...");
  psa_status_t mbedtlsInitStatus = psa_crypto_init();
  if (mbedtlsInitStatus != PSA_SUCCESS) {
    Serial.print("  ERROR: MbedTLS's PSA Crypto initialization failed, psa_status_t=");
    Serial.println(mbedtlsInitStatus);
    return false;
  }

  // Init Tropic01 resources (SPI is already initialized in spi_bus_init())
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

  // Start Secure Channel Session with TROPIC01.
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
