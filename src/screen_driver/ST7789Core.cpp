#include "cstdint"
#include "screen_driver/ST7789Core.h"

#include <HardwareSerial.h>

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

ColorFormats ST7789Core::ReadColorFormat() {
    uint8_t readBuffer[1] = {0};

    spi.GpioWrite(SCREEN_CS, false);
    ReadCommand(RDDCOLMOD, readBuffer, 1, 1);
    spi.GpioWrite(SCREEN_CS, true);

    uint8_t highNibble = (*readBuffer >> 4);
    uint8_t lowNibble =  ((*readBuffer) & 0x0F);

    ColorFormats rv = {};
    rv.RGBInterfaceFormat = static_cast<DisplayPixelFormat>(highNibble);
    rv.ControlInterfaceFormat = static_cast<DisplayPixelFormat>(lowNibble);

    return rv;
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

esp_err_t ST7789Core::WriteCommand(const uint8_t cmd, uint8_t* paramsBuffer, const size_t paramBytes) {
    // TODO I think this can be declared as a single transaction somehow
    esp_err_t err = ESP_OK;

    // Send command
    spi.GpioWrite(SCREEN_DC, false);

    spi_transaction_t commandTrans = {};
    commandTrans.flags = SPI_TRANS_USE_TXDATA; // Use internal buffer instead of pointer to stack
    commandTrans.tx_data[0] = cmd;
    commandTrans.length = 8;

    err = spi.Transmit(&commandTrans);
    if (err != ESP_OK) {
        return err;
    }

    if (paramsBuffer != nullptr && paramBytes > 0) {
        spi.GpioWrite(SCREEN_DC, true); // Send Parameters

        spi_transaction_t paramsTrans = {};
        paramsTrans.tx_buffer = paramsBuffer;
        paramsTrans.length = paramBytes * 8;

        err = spi.Transmit(&paramsTrans);
        if (err != ESP_OK) {
            return err;
        }
    }

    return ESP_OK;
}

esp_err_t ST7789Core::WritePixelData(uint16_t color, uint16_t amount) {
    assert(amount > 0);
    uint8_t writeBuffer[amount * 2];
    // TODO buffer the two bytes set here?
    for (uint16_t i = 0; i < amount*2; i++) {
        if (i % 2 == 0) {
            writeBuffer[i] = static_cast<uint8_t>(color >> 8);
        } else {
            writeBuffer[i] = static_cast<uint8_t>(color & 0x00FF);
        }
    }

    esp_err_t err = WriteCommand(RAMWR, writeBuffer,amount*2);
    return err;
}


esp_err_t ST7789Core::SetColumnsAddress(uint8_t x1, uint8_t x2) {
    assert(x1 <= x2 && x2 <= SCREEN_WIDTH-1);
    spi.GpioWrite(SCREEN_CS, false);
    // 1st and 3rd parameter should be empty because SCREEN WIDTH is small     // TODO support bigger?
    uint8_t writeBuffer[4] = {
        0,
        x1,
        0,
        x2
    };
    esp_err_t e = WriteCommand(COLMOD, writeBuffer, 4);
    spi.GpioWrite(SCREEN_CS, true);
    return e;
}

esp_err_t ST7789Core::SetRowsAddress(uint16_t y1, uint16_t y2) {
    assert(y1 <= y2 && y2 <= SCREEN_HEIGHT-1);

    spi.GpioWrite(SCREEN_CS, false);
    uint8_t writeBuffer[4] = {
        static_cast<uint8_t>(y1 >> 8),
        static_cast<uint8_t>(y1 & 0x00FF),
        static_cast<uint8_t>(y2 >> 8),
        static_cast<uint8_t>(y2 & 0x00FF)
    };
    esp_err_t e = WriteCommand(COLMOD, writeBuffer, 4);
    spi.GpioWrite(SCREEN_CS, true);
    return e;
}

esp_err_t ST7789Core::SetColorFormat(ColorFormats* config) {
    spi.GpioWrite(SCREEN_CS, false);
    uint8_t writeBuffer = (config->RGBInterfaceFormat << 4) | config->ControlInterfaceFormat;
    esp_err_t e = WriteCommand(COLMOD, &writeBuffer, 1);
    spi.GpioWrite(SCREEN_CS, true);
    ESP_ERROR_CHECK(e);
    return e;
}