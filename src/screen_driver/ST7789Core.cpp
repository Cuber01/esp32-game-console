#include "cstdint"
#include "screen_driver/ST7789Core.h"

#include "graphics.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

void ST7789Core::Init(void) {
    spi.Init();
}

uint32_t ST7789Core::ReadDisplayID(void) {
    // [0]=Dummy, rx_buf[1]=ID1, rx_buf[2]=ID2, rx_buf[3]=ID3
    uint8_t readBuffer[3] = {0};
    spi.GpioWrite(SCREEN_CS, false);

    spi.ReadCommand(0x04, readBuffer, 3, 1);

    spi.GpioWrite(SCREEN_CS, true);

    uint8_t id1 = readBuffer[0];
    uint8_t id2 = readBuffer[1];
    uint8_t id3 = readBuffer[2];

    return (static_cast<uint32_t>(id1) << 16)
           |(static_cast<uint32_t>(id2) << 8)
           |static_cast<uint32_t>(id3);
}