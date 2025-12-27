#ifndef SPI_BUS_H
#define SPI_BUS_H

#include <SPI.h>

// Declare the SPI bus instance as extern (defined elsewhere)
extern SPIClass SPI_Bus;

// Initialize SPI bus
// Returns true on success, false on failure
bool spi_bus_init();

#endif // SPI_BUS_H
