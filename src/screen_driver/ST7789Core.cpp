#include "cstdint"
#include "screen_driver/ST7789Core.h"

#include "graphics.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "screen_driver/Commands.h"
#include "screen_driver/DisplayPixelFormat.h"

void ST7789Core::Init(void) {
    spi.Init();
}

uint32_t ST7789Core::ReadDisplayID(void) {
    uint8_t readBuffer[3] = {0};
    spi.GpioWrite(SCREEN_CS, false);

    ReadCommand(RDDID, readBuffer, 3, 1);

    spi.GpioWrite(SCREEN_CS, true);

    uint8_t id1 = readBuffer[0];
    uint8_t id2 = readBuffer[1];
    uint8_t id3 = readBuffer[2];

    return (static_cast<uint32_t>(id1) << 16)
           |(static_cast<uint32_t>(id2) << 8)
           |static_cast<uint32_t>(id3);
}

esp_err_t ST7789Core::ReadCommand(uint8_t cmd, uint8_t* receiveBuffer,
                                 size_t rxInformationBytes, size_t rxDummyBytes) {
    // Pull DC LOW for Command Phase
    spi.GpioWrite(SCREEN_DC, false);

    spi_transaction_ext_t t = {};
    t.base.flags = SPI_TRANS_VARIABLE_CMD; // TODO NO NEED FOR THIS FLAG HERE, I THINK WE CAN SET THE DEFAULT IN CONFIG INSTEAD
    t.base.cmd = cmd;
    t.command_bits = 8;
    t.dummy_bits = rxDummyBytes * 8;
    t.base.rxlength = rxInformationBytes * 8;
    t.base.rx_buffer = receiveBuffer;

    return spi.Transmit(reinterpret_cast<spi_transaction_t *>(&t));
}

esp_err_t ST7789Core::WriteCommand(const uint8_t cmd, const uint8_t* paramsBuffer, const size_t paramBytes) {
    // TODO I think this can be declared as a single transaction somehow
    esp_err_t err = ESP_OK;
    spi.GpioWrite(SCREEN_DC, false); // Command

    spi_transaction_ext_t commandTrans = {};
    commandTrans.base.tx_buffer = &cmd;
    commandTrans.base.length = 8;

    err = spi.Transmit(reinterpret_cast<spi_transaction_t *>(&commandTrans));
    if (err != ESP_OK) {
        return err;
    }

    spi.GpioWrite(SCREEN_DC, true); // Params

    spi_transaction_ext_t paramsTrans = {};
    paramsTrans.base.tx_buffer = paramsBuffer;
    paramsTrans.base.length = paramBytes * 8;

    err = spi.Transmit(reinterpret_cast<spi_transaction_t *>(&paramsTrans));
    return err;
}

ColorFormats ST7789Core::ReadColorFormat() {
    uint8_t* receiveBuffer = nullptr;

    ReadCommand(RDDCOLMOD, receiveBuffer, 1, 1);

    uint8_t highNibble = (*receiveBuffer >> 4);
    uint8_t lowNibble =  ((*receiveBuffer) & 0x0F);

    ColorFormats rv = {};
    rv.RGBInterfaceFormat = static_cast<DisplayPixelFormat>(highNibble);
    rv.ColorInterfaceFormat = static_cast<DisplayPixelFormat>(lowNibble);

    return rv;
}

esp_err_t ST7789Core::SetColorFormat(ColorFormats* config) {
    spi.GpioWrite(SCREEN_CS, false);
    esp_err_t e = WriteCommand(COLMOD, reinterpret_cast<const uint8_t*>(config), 1);
    spi.GpioWrite(SCREEN_CS, true);
    assert(e == ESP_OK);
}