#include <Arduino.h>

#include "graphics.h"
#include "screen_driver/Colors.h"
#include "screen_driver/ST7789.h"
#include "screen_driver/Core.h"


ST7789 gfx = ST7789();
void renderFrame() {
    gfx.FillScreen(BLUE);
    gfx.DrawRect(0,0,8,8,GREEN);

    gfx.DrawText(0, 8, "Haaaaaa");
    gfx.DrawRect(0,8,8,8,GREEN);

    gfx.DrawRect(0,16,8,8,GREEN);

    gfx.DrawRect(8,0,8,8,GREEN);

    gfx.DrawRect(8,8,8,8,GREEN);

    gfx.DrawRect(0,8,8,8,GREEN);

    gfx.DrawRect(16,0,8,8,GREEN);

    gfx.DrawRect(16,16,8,8,GREEN);
}

void setup() {
    Serial.begin(9600);
    Serial.print(F("Hello! ST77xx TFT Test"));
    gfx.Init(renderFrame);
}

void loop() {
    gfx.Update();
}

