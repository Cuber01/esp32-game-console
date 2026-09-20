#include <Arduino.h>
#include "game.h"

void setup() {
    Serial.begin(9600);
    Serial.print(F("Hello! ST77xx TFT Test"));
    gameInit();
}

void loop() {
    if (IsRunning()) {
        gameLoop();
    }
}

