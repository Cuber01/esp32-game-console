#ifndef ESP32_GAME_CONSOLE_SPIHANDLER_H
#define ESP32_GAME_CONSOLE_SPIHANDLER_H

#include <cstdint>
#include "driver/spi_master.h"

class SPIHandler {
    spi_device_handle_t spiHandle = nullptr;

    void Init();
    void GpioWrite(uint8_t pin, bool level);
    void Transmit(uint8_t *data, size_t length);
    void Receive(uint8_t *data, size_t length);
};


#endif //ESP32_GAME_CONSOLE_SPIHANDLER_H
