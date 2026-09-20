#include <Arduino.h>
#include "game.h"

void setup() {
    Serial.begin(9600);
    Serial.print(F("Hello! ST77xx TFT Test"));
    gameInit();

    // SPI speed defaults to SPI_DEFAULT_FREQ defined in the library, you can override it here
    // Note that speed allowable depends on chip and quality of wiring, if you go too fast, you
    // may end up with a black screen some times, or all the time.

}

void loop() {
    if (IsRunning()) {
        gameLoop();
    }
}

