#ifndef GAME_CONSOLE_SPI_HANDLER_H
#define GAME_CONSOLE_SPI_HANDLER_H

#include <cstdint>
#include "driver/spi_master.h"

class SPIHandler {
    spi_device_handle_t spiHandle = nullptr;

public:
    void Init();
    esp_err_t GpioWrite(uint8_t pin, bool level);
    esp_err_t Transmit(spi_transaction_t* transaction);

};


#endif // GAME_CONSOLE_SPI_HANDLER_H
