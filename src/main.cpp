#include <Arduino.h>

#include "graphics.h"
#include "screen_driver/Colors.h"
#include "screen_driver/ST7789.h"
#include "screen_driver/Core.h"


ST7789 gfx = ST7789();

void renderFrame(uint16_t* buffer) {
    gfx.SetFrame(buffer);
    gfx.FillScreen(BLUE);
    gfx.DrawRect(10, 10, 100, 100, RED);
}

void setup() {
    Serial.begin(9600);
    Serial.print(F("Hello! ST77xx TFT Test"));
    gfx.Init(renderFrame);
}


void loop() {
    gfx.Update();
}

