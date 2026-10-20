#ifndef GAME_CONSOLE_ST7789_H
#define GAME_CONSOLE_ST7789_H
#include <cstdint>

#include "Colors.h"
#include "CommandBuffer.h"
#include "Core.h"
#include "DMAHandler.h"

class ST7789 {
private:
    DMAHandler* dma = new DMAHandler();
    Core* core = new Core();
    SPIHandler* spi = new SPIHandler();
    CommandBuffer* userCmds = new CommandBuffer();

    uint16_t* currentFrameBuffer = nullptr;
    uint16_t textColor = WHITE;
    uint16_t textBackgroundColor = BLACK;
    void (*userRender)() = nullptr;

public:
    void Init(void (*renderFunction)());

    void Update();

    void SetFrame(uint16_t *frameBuffer);

    void FillScreen(std::uint16_t color) const;
    void DrawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) const;

    void DrawText(uint16_t x, uint16_t y, const char *text);

    void SetTextColor(uint16_t color);

    void SetTextBackgroundColor(uint16_t color);
};

#endif // GAME_CONSOLE_ST7789_H
