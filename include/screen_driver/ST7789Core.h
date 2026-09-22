
#ifndef ESP32_GAME_CONSOLE_ST7789CORE_H
#define ESP32_GAME_CONSOLE_ST7789CORE_H
#include "SPIHandler.h"


class ST7789Core {
    SPIHandler spi;

public:
    void Init();
    uint32_t ReadDisplayID();
};


#endif
