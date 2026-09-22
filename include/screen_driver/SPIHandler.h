#ifndef ESP32_GAME_CONSOLE_SPIHANDLER_H
#define ESP32_GAME_CONSOLE_SPIHANDLER_H

#include <cstdint>
#include "driver/spi_master.h"

class SPIHandler {
    spi_device_handle_t spiHandle = nullptr;

public:
    void Init();
    esp_err_t GpioWrite(uint8_t pin, bool level);
    esp_err_t ReadCommand(uint8_t cmd, uint8_t *receiveBuffer, size_t rxInformationBytes, size_t rxDummyBytes);
    esp_err_t Transmit(uint8_t *data, size_t length);
    esp_err_t Receive(uint8_t *data, size_t length);
};


#endif //ESP32_GAME_CONSOLE_SPIHANDLER_H
