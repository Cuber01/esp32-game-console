
#ifndef ESP32_GAME_CONSOLE_SPIHANDLER_H
#define ESP32_GAME_CONSOLE_SPIHANDLER_H
#include <driver/spi_master.h>

class SPIHandler {
    spi_device_handle_t spiHandle = nullptr;

public:
    void Init();

    esp_err_t GpioWrite(uint8_t pin, bool level);
    esp_err_t Transmit(spi_transaction_t *transaction, uint8_t chipSelectPin);

    esp_err_t QueueTransmit(spi_transaction_t *transaction) const;

    esp_err_t GetTransactionResult(spi_transaction_t **transactionPointer) const;

    esp_err_t TransmitWithManualCS(spi_transaction_t *transaction) const;
};

#endif //ESP32_GAME_CONSOLE_SPIHANDLER_H
