#ifndef GAME_CONSOLE_ST7789_H
#define GAME_CONSOLE_ST7789_H
#include <cstdint>

#include "Colors.h"
#include "ST7789Core.h"

class ST7789 {
private:
    ST7789Core* driver = new ST7789Core();

    uint16_t textColor = WHITE;
    uint16_t textBackgroundColor = BLACK;
public:
    void Init();

    void FillScreen(std::uint16_t color) const;
    void DrawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) const;

    void DrawText(uint16_t x, uint16_t y, const char *text);

    void SetTextColor(uint16_t color);

    void SetTextBackgroundColor(uint16_t color);
};

#endif // GAME_CONSOLE_ST7789_H
