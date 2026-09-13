
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
}

void mapDraw(void)
{
    tft.fillScreen(ST77XX_BLACK);

    tft.setCursor(0, 0);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextWrap(false);
    tft.print("apples: ");
    tft.print(applesEaten);

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            //tft.fillRect(0,0,MAP_HEIGHT*MAP_BLOCK_SIZE, MAP_WIDTH*MAP_BLOCK_SIZE, ST77XX_WHITE);
            if (mapGet(x,y) == APPLE_SYMBOL) {
                 tft.fillRect(x*MAP_BLOCK_SIZE, y*MAP_BLOCK_SIZE, MAP_BLOCK_SIZE, MAP_BLOCK_SIZE, ST77XX_RED);
            } else if (mapGet(x,y) == SNAKE_SYMBOL) {
                 tft.fillRect(x*MAP_BLOCK_SIZE, y*MAP_BLOCK_SIZE, MAP_BLOCK_SIZE, MAP_BLOCK_SIZE, ST77XX_GREEN);
            } else if (mapGet(x,y) == GROUND_SYMBOL) {
                 tft.fillRect(x*MAP_BLOCK_SIZE, y*MAP_BLOCK_SIZE, MAP_BLOCK_SIZE, MAP_BLOCK_SIZE, ST77XX_WHITE);
            }
        };
    };
}