#include "spi_bus.h"
#include "badge.h"

// Define the SPI bus instance HERE (only once)
SPIClass SPI_Bus = SPIClass(HSPI);

bool spi_bus_init() {
    Serial.println("spi_bus_init()");
    SPI_Bus.begin(SPI_SCLK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN, -1); // -1 for CS (managed per device)
    return true; // SPI.begin() doesn't return error status, assume success
}
