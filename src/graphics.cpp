
#include "graphics.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#include "game.h"
#include "map.h"

// OPTION 1 (recommended) is to use the HARDWARE SPI pins, which are unique
// to each board and not reassignable. For Arduino Uno: MOSI = pin 11 and
// SCLK = pin 13. This is the fastest mode of operation and is required if
// using the breakout board's microSD card.

Adafruit_ST7789 tft = Adafruit_ST7789(SCREEN_CS, SCREEN_DC, SCREEN_RST);

void init() {
    tft.init(SCREEN_WIDTH, SCREEN_HEIGHT);
    tft.fillScreen(0);
    //tft.setSPISpeed(40000000);
}

void appleCounterDraw() {
    tft.setCursor(0, 0);
    tft.setTextColor(ST77XX_YELLOW, ST77XX_BLACK);
    tft.setTextWrap(false);
    tft.print("apples: ");

    // Clear last number
    int16_t lastX = tft.getCursorX();
    int16_t lastY = tft.getCursorY();
    tft.setTextColor(ST77XX_MAGENTA, ST77XX_BLACK);
    tft.print("      ");

    tft.setCursor(lastX, lastY);
    tft.setTextColor(ST77XX_YELLOW, ST77XX_BLACK);
    tft.print(applesEaten);
    //ESP.getFreeHeap()
}

void mapDraw() {
    for (int y = 1; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            if (mapGet(x,y) == APPLE_SYMBOL) {
                tft.fillRect(x*MAP_BLOCK_SIZE, y*MAP_BLOCK_SIZE, MAP_BLOCK_SIZE, MAP_BLOCK_SIZE, ST77XX_RED);
            } else if (mapGet(x,y) == SNAKE_SYMBOL) {
                tft.fillRect(x*MAP_BLOCK_SIZE, y*MAP_BLOCK_SIZE, MAP_BLOCK_SIZE, MAP_BLOCK_SIZE, ST77XX_GREEN);
            } else if (mapGet(x,y) == GROUND_SYMBOL) {
                tft.fillRect(x*MAP_BLOCK_SIZE, y*MAP_BLOCK_SIZE, MAP_BLOCK_SIZE, MAP_BLOCK_SIZE, ST77XX_WHITE);
            }
        }
    }
}

void gameDraw()
{
    appleCounterDraw();
    mapDraw();
}

void graphicsDemo() {
    static int X = 0;
    static int Y = 0;

    tft.fillRect(X*MAP_BLOCK_SIZE,Y*MAP_BLOCK_SIZE,MAP_BLOCK_SIZE, MAP_BLOCK_SIZE, ST77XX_WHITE);
    if (X >= MAP_WIDTH) {
        X = 0;
        Y += 1;
    } else {
        X += 1;
    }

    tft.fillRect(MAP_WIDTH*8 -8, MAP_HEIGHT*8 -8, MAP_BLOCK_SIZE, MAP_BLOCK_SIZE, ST77XX_CYAN);
    tft.fillRect(MAP_WIDTH*8 -8, 0, MAP_BLOCK_SIZE, MAP_BLOCK_SIZE, ST77XX_YELLOW);
    tft.fillRect(0, MAP_HEIGHT*8 -8, MAP_BLOCK_SIZE, MAP_BLOCK_SIZE, ST77XX_MAGENTA);
    tft.fillRect(0, 0, MAP_BLOCK_SIZE, MAP_BLOCK_SIZE, ST77XX_ORANGE);
}