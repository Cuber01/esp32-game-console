#include "screen_driver/ST7789.h"

#include <esp32-hal.h>

#include "graphics.h"

void ST7789::Init(void (*renderFunction)()) {
    spi->Init();
    dma->Init(spi, userCmds, core);
    core->Init(spi);
    userRender = renderFunction;
}

void ST7789::Update() {
    userCmds->Clear();
    userRender();
    dma->RenderFrame();
}

void ST7789::FillScreen(uint16_t color) const {
    DrawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, color);
}

void ST7789::DrawRect(const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height, const uint16_t color) const {
    userCmds->PushFixedCommand<CmdRectHeader>(OpCode::RECT, CmdRectHeader {.x = x, .y = y, .w = width, .h = height, .color = color});
}

void ST7789::SetTextColor(uint16_t color) {
    userCmds->TextColor = color;
}

void ST7789::SetTextBackgroundColor(uint16_t color) {
    userCmds->TextBackgroundColor = color;
}

void ST7789::DrawText(uint16_t x, uint16_t y, const char* text) {
    userCmds->PushTextCommand(x, y, text, static_cast<uint16_t>(strlen(text)));
}
