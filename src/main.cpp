#include <Arduino.h>

#include "screen_driver/ST7789Core.h"

//Adafruit_ST7789 tft = Adafruit_ST7789(SCREEN_CS, SCREEN_DC, SCREEN_RST);
auto* driver = new ST7789Core();
void setup() {
    Serial.begin(9600);
    Serial.print(F("Hello! ST77xx TFT Test"));
    //tft.init(SCREEN_WIDTH, SCREEN_HEIGHT);

    delay(100);

    driver->Init();
    delay(100);

    delay(100);


    //ESP_ERROR_CHECK(driver->WritePixelData(GREEN, 1000));

    //gameInit();
}

void loop() {
    // if (IsRunning()) {
    //     gameLoop();
    // }

}

