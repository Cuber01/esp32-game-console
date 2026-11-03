#include <Arduino.h>

#include "graphics.h"
#include "screen_driver/Colors.h"
#include "screen_driver/ST7789.h"
#include "screen_driver/Core.h"


ST7789 gfx = ST7789();
void renderFrame() {
    gfx.FillScreen(BLUE);
    gfx.DrawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RED);
}

void setup() {
    Serial.begin(9600);
    Serial.print(F("Hello! ST77xx TFT Test"));
    gfx.Init(renderFrame);
}

void loop() {
    gfx.Update();
}

