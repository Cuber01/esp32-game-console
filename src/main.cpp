#include <Arduino.h>
#include "game.h"
#include "screen_driver/ST7789Core.h"

void setup() {
    Serial.begin(9600);
    Serial.print(F("Hello! ST77xx TFT Test"));

    delay(100);
    auto* driver = new ST7789Core();
    driver->Init();
    delay(100);
    uint32_t scrrenID = driver->ReadDisplayID();
    Serial.print(scrrenID);

    //gameInit();
}

void loop() {
    // if (IsRunning()) {
    //     gameLoop();
    // }
}

