#include <Arduino.h>

#include "graphics.h"
#include "screen_driver/Colors.h"
#include "screen_driver/ST7789.h"
#include "screen_driver/ST7789Core.h"


ST7789 gfx = ST7789();
void setup() {
    Serial.begin(9600);
    Serial.print(F("Hello! ST77xx TFT Test"));
    gfx.Init();
    gfx.FillScreen(BLUE);

    gfx.DrawText(100,100, "HELLO WORLD!");
}

void loop() {
    //gfx.FillScreen(esp_random());

    // delay(50);

    // if (IsRunning()) {
    //     gameLoop();
    // }
}

