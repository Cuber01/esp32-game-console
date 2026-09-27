#include <Arduino.h>

#include "screen_driver/Colors.h"
#include "screen_driver/ST7789Core.h"

//Adafruit_ST7789 tft = Adafruit_ST7789(SCREEN_CS, SCREEN_DC, SCREEN_RST);
auto* driver = new ST7789Core();
void setup() {
    Serial.begin(9600);
    Serial.print(F("Hello! ST77xx TFT Test"));
    driver->Init();

    ESP_ERROR_CHECK(driver->SetColumnsAddress(10,99));
    ESP_ERROR_CHECK(driver->SetRowsAddress(10,99));
    ESP_ERROR_CHECK(driver->WritePixelData(CYAN, 810));
}

void loop() {
    // if (IsRunning()) {
    //     gameLoop();
    // }
}

