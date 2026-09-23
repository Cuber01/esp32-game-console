
#ifndef GAME_CONSOLE_ST7789_CORE_H
#define GAME_CONSOLE_ST7789_CORE_H
#include "DisplayPixelFormat.h"
#include "SPIHandler.h"


class ST7789Core {
    SPIHandler spi;

public:
    void Init();
    uint32_t ReadDisplayID();
    esp_err_t ReadCommand(uint8_t cmd, uint8_t *receiveBuffer, size_t rxInformationBytes, size_t rxDummyBytes);

    esp_err_t WriteCommand(uint8_t cmd, const uint8_t *paramsBuffer, size_t paramBytes);

    ColorFormats ReadColorFormat();

    esp_err_t SetColorFormat(ColorFormats *config);
};


#endif // GAME_CONSOLE_ST7789_CORE_H
