/**
 * @file hello_world.ino
 * @brief Libtropic Hello World example using the C++ wrapper.
 * @copyright Copyright (c) 2020-2025 Tropic Square s.r.o.
 *
 * @license For the license see file LICENSE.txt file in the root directory of
 * this source tree.
 */

/***************************************************************************
 *
 * Hello World TROPIC01 Example
 *
 * This example shows how to:
 * 1. Establish a Secure Channel Session with TROPIC01.
 * 2. Execute the Ping command to send some message to TROPIC01.
 *
 * The Ping command can be used to verify that the Secure Channel Session
 * was successfully established.
 *
 * In the background, the LibtropicArduino.h library uses Libtropic,
 * a C-based Software Development Kit (SDK) developed by Tropic Square,
 * used to interface the TROPIC01 secure element.
 *
 * Because some cryptographic functionality is needed on the host side,
 * MbedTLS library is included below. Libtropic uses it to e.g. decrypt
 * incomming packets from TROPIC01.
 *
 * For more information, refer to the following links:
 * 1. Tropic Square: https://tropicsquare.com/
 * 2. TROPIC01: https://tropicsquare.com/tropic01
 * 3. TROPIC01 resources: https://github.com/tropicsquare/tropic01
 * 4. Libtropic: https://tropicsquare.github.io/libtropic
 *
 ***************************************************************************/

// Arduino libraries.
#include <Arduino.h>

#include "app.h"
#include "badge.h"

app_state_t app_state;
void setup() {

    // Initialize all hardware resources
    badge_init();

    // Initialize application state
    app_setup(&app_state);
}

void loop() {

    // Loop through the all inputs and process the aplication outputs
    app_loop(&app_state);
}