#include <Arduino.h>

#include "screen_driver/ST7789Core.h"

//Adafruit_ST7789 tft = Adafruit_ST7789(SCREEN_CS, SCREEN_DC, SCREEN_RST);
auto* driver = new ST7789Core();
void setup() {
    Serial.begin(9600);
    Serial.print(F("Hello! ST77xx TFT Test"));
    driver->Init();
    //gameInit();
}

void loop() {
    // if (IsRunning()) {
    //     gameLoop();
    // }
}

