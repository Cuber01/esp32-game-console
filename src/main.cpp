#include <Arduino.h>
#include "game.h"
#include "screen_driver/ST7789Core.h"

void setup() {
    Serial.begin(9600);
    Serial.print(F("Hello! ST77xx TFT Test"));

    delay(100);
    auto* driver = new ST7789Core();
    driver->Init();
    delay(10);
    ColorFormats formats = driver->ReadColorFormat();
    Serial.print(formats.RGBInterfaceFormat);
    Serial.print(formats.ControlInterfaceFormat);
    delay(1);
    ColorFormats newFormats = {
        .RGBInterfaceFormat = UNSET,
        .ControlInterfaceFormat = DISPLAY_16_BIT_PIXEL
    };
    driver->SetColorFormat(&newFormats);
    delay(1);
    formats = driver->ReadColorFormat();
    Serial.print(formats.RGBInterfaceFormat);
    Serial.print(formats.ControlInterfaceFormat);

    //gameInit();
}

void loop() {
    // if (IsRunning()) {
    //     gameLoop();
    // }
}

