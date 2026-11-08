#include <Arduino.h>

#include "apples.h"
#include "game.h"
#include "map.h"
#include "screen_driver/Colors.h"
#include "screen_driver/ST7789.h"
#include "screen_driver/Core.h"


ST7789 gfx = ST7789();
void renderFrame() {
    gfx.DrawText(0, 0, "APPLES: ");
    gfx.DrawText(72, 0, String(ApplesEaten).c_str());

    gfx.DrawRect(0, MAP_BLOCK_SIZE, SCREEN_WIDTH, SCREEN_HEIGHT-MAP_BLOCK_SIZE, WHITE);
    for (int y = 1; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            if (mapGet(x,y) == APPLE_SYMBOL) {
                gfx.DrawRect(x*MAP_BLOCK_SIZE, y*MAP_BLOCK_SIZE, MAP_BLOCK_SIZE, MAP_BLOCK_SIZE, RED);
            } else if (mapGet(x,y) == SNAKE_SYMBOL) {
                gfx.DrawRect(x*MAP_BLOCK_SIZE, y*MAP_BLOCK_SIZE, MAP_BLOCK_SIZE, MAP_BLOCK_SIZE, GREEN);
            }
        }
    }
}

void setup() {
    Serial.begin(9600);
    Serial.print(F("Hello! ST77xx TFT Test"));
    gfx.Init(renderFrame);
    gameInit();
}

void loop() {
    if (IsRunning()) {
        gameLoop();
        gfx.Update();
    }
}

