#include "serial_port.h"

void serial_port_init() {
    Serial.begin(115200);
    delay(700); // Give USB-CDC time to initialize
    
    // Wait for serial port to connect (max 5 seconds for native USB)
    while (!Serial && millis() < 5000)
        ;
    Serial.println();
    Serial.println("Serial port initialized");
    Serial.println();
}
