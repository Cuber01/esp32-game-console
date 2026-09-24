
#ifndef GAME_CONSOLE_ST7789_CORE_H
#define GAME_CONSOLE_ST7789_CORE_H
#include <array>

#include "Commands.h"
#include "DisplayPixelFormat.h"
#include "SPIHandler.h"


class ST7789Core {
    SPIHandler spi;
    static constexpr uint32_t WriteBufferSize = 128;
    uint8_t writeBuffer[WriteBufferSize] = {};

public:
    void Init();
    uint32_t ReadDisplayID();
    esp_err_t ReadCommand(uint8_t cmd, uint8_t *receiveBuffer, size_t rxInformationBytes, size_t rxDummyBytes);

    esp_err_t WriteCommand(Commands cmd, uint8_t *paramsBuffer, size_t paramBytes);

    esp_err_t WritePixelData(uint16_t color, int32_t amount);

    esp_err_t transmitCommand(Commands cmd);

    esp_err_t transmitParameters(const uint8_t *paramsBuffer, size_t paramBytes);

    void setupTxBuffer(spi_transaction_t *trans, const uint8_t *paramsBuffer, size_t paramBytes);

    esp_err_t SetSleep(bool awake);

    esp_err_t SoftwareReset();

    esp_err_t TurnDisplay(bool on);

    esp_err_t SetRowsAddress(uint16_t y1, uint16_t y2);

    esp_err_t SetColumnsAddress(uint8_t y1, uint8_t y2);

    ColorFormats ReadColorFormat();

    esp_err_t SetColorFormat(ColorFormats *config);
};


#endif // GAME_CONSOLE_ST7789_CORE_H
