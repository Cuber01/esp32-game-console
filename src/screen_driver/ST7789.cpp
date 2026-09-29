#include "screen_driver/ST7789.h"

#include <esp32-hal.h>

#include "graphics.h"
#include "screen_driver/Colors.h"
#include "screen_driver/Font.h"

void ST7789::Init(void (*renderFunction)(uint16_t* buffers)) {
    spi->Init();
    dma->Init(renderFunction, spi);
    core->Init(spi);

    dma->Start();
}

void ST7789::Update() {
    dma->Loop();
}

void ST7789::FillScreen(uint16_t color) const {
    DrawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, color);
}

void ST7789::DrawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) const {
    for (int i = y; i < y+height; i++) {
        for (int j = x; j < x+width; j++) {
            currentFrameBuffer[j+i*width] = color;
        }
    }
}

void ST7789::SetTextColor(uint16_t color) {
    textColor = color;
}

void ST7789::SetTextBackgroundColor(uint16_t color) {
    textBackgroundColor = color;
}

void ST7789::SetFrame(uint16_t* frameBuffer) {
    currentFrameBuffer = frameBuffer;
}

// void ST7789::DrawText(uint16_t x, uint16_t y, const char* text) {
//     uint16_t currentX = x;
//     driver->SetColumnsAddress(x, (CHAR_WIDTH+CHAR_SPACING)*strlen(text));
//     driver->SetRowsAddress(y, CHAR_HEIGHT);
//
//     driver->WritePixelData(textBackgroundColor, 1, false);
//     for (int i = 0; text[i] != '\0'; i++) {
//         uint8_t index = GET_CHAR_INDEX(text[i]);
//         for (int j = 0;)
//
//         currentX += CHAR_WIDTH + CHAR_SPACING;
//     }
// }
