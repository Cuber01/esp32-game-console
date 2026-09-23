#include <Arduino.h>
#include "game.h"
#include "screen_driver/Colors.h"
#include "screen_driver/ST7789Core.h"

auto* driver = new ST7789Core();
void setup() {
    Serial.begin(9600);
    Serial.print(F("Hello! ST77xx TFT Test"));

    delay(100);

    driver->Init();
    delay(10);
    ColorFormats newFormats = {
        .RGBInterfaceFormat = UNSET,
        .ControlInterfaceFormat = DISPLAY_16_BIT_PIXEL
    };
    driver->SetColorFormat(&newFormats);
    delay(1);
    ColorFormats formats = driver->ReadColorFormat();
    Serial.print(formats.RGBInterfaceFormat);
    Serial.print(formats.ControlInterfaceFormat);





    //gameInit();
}

void loop() {
    // if (IsRunning()) {
    //     gameLoop();
    // }
    ESP_ERROR_CHECK(driver->SetColumnsAddress(25,30));
    ESP_ERROR_CHECK(driver->SetRowsAddress(25,30));
    ESP_ERROR_CHECK(driver->WritePixelData(GREEN, 25));
}

