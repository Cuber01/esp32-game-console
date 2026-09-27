#include "cstdint"
#include "screen_driver/ST7789Core.h"

#include <HardwareSerial.h>

#include "graphics.h"
#include "driver/spi_master.h"
#include "screen_driver/Commands.h"
#include "screen_driver/DisplayPixelFormat.h"
#include "screen_driver/Colors.h"

void ST7789Core::Init(void) {
    spi.Init();

    spi.GpioWrite(SCREEN_CS, false);

    ESP_ERROR_CHECK(transmitCommand(SWRESET));
    delay(120);
    ESP_ERROR_CHECK(SetSleep(false));
    delay(120);

    ColorFormats newFormats = {
        .RGBInterfaceFormat = UNSET,
        .ControlInterfaceFormat = DISPLAY_16_BIT_PIXEL
    };
    ESP_ERROR_CHECK(SetColorFormat(&newFormats));
    delay(100);
    ColorFormats formats = ReadColorFormat();
    Serial.print(formats.RGBInterfaceFormat);
    Serial.print(formats.ControlInterfaceFormat);
    delay(200);

    uint8_t buf[1] = {0x00};
    WriteCommand(MADCTL, buf, 1);

    ESP_ERROR_CHECK(SetColumnsAddress(0,SCREEN_WIDTH-1));
    ESP_ERROR_CHECK(SetRowsAddress(0,SCREEN_HEIGHT-1));


    transmitCommand(INVON);
    transmitCommand(NORON);
    ESP_ERROR_CHECK(TurnDisplay(true));
    delay(200);

    ESP_ERROR_CHECK(WritePixelData(BLUE, 76800));


    delay(200);

    spi.GpioWrite(SCREEN_CS, true);
}

uint32_t ST7789Core::ReadDisplayID(void) {
    uint8_t readBuffer[3] = {0};

    ReadCommand(RDDID, readBuffer, 3, 1);

    uint8_t id1 = readBuffer[0];
    uint8_t id2 = readBuffer[1];
    uint8_t id3 = readBuffer[2];

    return (static_cast<uint32_t>(id1) << 16)
           |(static_cast<uint32_t>(id2) << 8)
           |static_cast<uint32_t>(id3);
}

ColorFormats ST7789Core::ReadColorFormat() {
    uint8_t readBuffer[1] = {0};

    ESP_ERROR_CHECK(ReadCommand(RDDCOLMOD, readBuffer, 1, 1));

    uint8_t highNibble = (*readBuffer >> 4);
    uint8_t lowNibble =  ((*readBuffer) & 0x0F);

    ColorFormats rv = {};
    rv.RGBInterfaceFormat = static_cast<DisplayPixelFormat>(highNibble);
    rv.ControlInterfaceFormat = static_cast<DisplayPixelFormat>(lowNibble);

    return rv;
}

esp_err_t ST7789Core::ReadCommand(uint8_t cmd, uint8_t* receiveBuffer,
                                 size_t rxInformationBytes, size_t rxDummyBytes) {
    spi.GpioWrite(SCREEN_DC, false);

    spi_transaction_ext_t t = {};
    t.base.flags = SPI_TRANS_VARIABLE_CMD;
    t.base.cmd = cmd;
    t.command_bits = 8;
    t.dummy_bits = rxDummyBytes * 8;
    t.base.rxlength = rxInformationBytes * 8;
    t.base.rx_buffer = receiveBuffer;

    return spi.Transmit(reinterpret_cast<spi_transaction_t *>(&t));
}

esp_err_t ST7789Core::WriteCommand(const Commands cmd, uint8_t* paramsBuffer, const size_t paramBytes) {
    assert(paramsBuffer != nullptr && paramBytes > 0);

    spi_transaction_t t = {};

    transmitCommand(cmd);
    transmitParameters(paramsBuffer, paramBytes);

    return ESP_OK;
}

// Writes pixel data in 2 byte format!!!
// [8bit data] + [8bit data]
esp_err_t ST7789Core::WritePixelData(uint16_t color, int32_t amount) {
    assert(amount > 0);
    transmitCommand(RAMWR);

    uint8_t firstByte = static_cast<uint8_t>(color >> 8);
    uint8_t secondByte = static_cast<uint8_t>(color & 0x00FF);
    while (amount > 0) {
        uint32_t bytesToSend = WriteBufferSize > amount*2 ? amount*2 : WriteBufferSize;

        for (uint32_t i = 0; i < bytesToSend; i++) {
            if (i % 2 == 0) {
                pxWriteBuffer[i] = firstByte;
            } else {
                pxWriteBuffer[i] = secondByte;
            }
        }

        const esp_err_t err = transmitParameters(pxWriteBuffer, bytesToSend);
        if (err != ESP_OK) {
            return err;
        }
        amount -= WriteBufferSize;
    }

    return ESP_OK;
}

// Reads pixel data in 3-byte format!!!
// [6bit data + 00] + [6bit data + 00] + [6bit data + 00]
esp_err_t ST7789Core::ReadPixelData(uint8_t* receiveBuffer, int32_t amount) {
    assert(amount > 0);
    return ReadCommand(RAMRD, receiveBuffer, amount*3, 0);
}

esp_err_t ST7789Core::SetSleep(bool sleep) {
    return transmitCommand(sleep ? SLPIN : SLPOUT);
}

esp_err_t ST7789Core::TurnDisplay(bool on) {
    return transmitCommand(on ? DISPON : DISPOFF);
}

esp_err_t ST7789Core::SetColumnsAddress(uint8_t x1, uint8_t x2) {
    assert(x1 <= x2 && x2 <= SCREEN_WIDTH-1);
    // 1st and 3rd parameter should be empty because SCREEN WIDTH is small
    // TODO support bigger?
    uint8_t writeBuffer[4] = {
        0,
        x1,
        0,
        x2
    };
    esp_err_t e = WriteCommand(CASET, writeBuffer, 4);
    return e;
}

esp_err_t ST7789Core::SetRowsAddress(uint16_t y1, uint16_t y2) {
    assert(y1 <= y2 && y2 <= SCREEN_HEIGHT-1);

    uint8_t writeBuffer[4] = {
        static_cast<uint8_t>(y1 >> 8),
        static_cast<uint8_t>(y1 & 0x00FF),
        static_cast<uint8_t>(y2 >> 8),
        static_cast<uint8_t>(y2 & 0x00FF)
    };
    esp_err_t e = WriteCommand(RASET, writeBuffer, 4);
    return e;
}

esp_err_t ST7789Core::SetColorFormat(ColorFormats* config) {
    uint8_t writeBuffer = (config->RGBInterfaceFormat << 4) | config->ControlInterfaceFormat;
    esp_err_t e = WriteCommand(COLMOD, &writeBuffer, 1);
    ESP_ERROR_CHECK(e);
    return e;
}

esp_err_t ST7789Core::transmitCommand(Commands cmd) {
    spi.GpioWrite(SCREEN_DC, false);

    spi_transaction_t commandTrans = {};
    commandTrans.flags = SPI_TRANS_USE_TXDATA; // Use internal buffer instead of pointer to stack
    commandTrans.tx_data[0] = cmd;
    commandTrans.length = 8;

    esp_err_t err = spi.Transmit(&commandTrans);
    return err;
}

esp_err_t ST7789Core::transmitParameters(const uint8_t* paramsBuffer, const size_t paramBytes) {
    spi.GpioWrite(SCREEN_DC, true); // Send Parameters

    spi_transaction_t paramsTrans = {};

    //setupTxBuffer(&paramsTrans, paramsBuffer, paramBytes);
    if (paramBytes <= 4) {
        paramsTrans.flags = SPI_TRANS_USE_TXDATA;
        for (size_t i = 0; i < paramBytes; i++) {
            paramsTrans.tx_data[i] = paramsBuffer[i];
        }
    } else {
        paramsTrans.tx_buffer = paramsBuffer;
    }
    // ----

    paramsTrans.length = paramBytes * 8;

    esp_err_t err = spi.Transmit(&paramsTrans);
    return err;
}

// TODO for whatever reason this function clears out memory of paramsBuffer, but I only see it in debug, check if its a debugger bug
// void ST7789Core::setupTxBuffer(spi_transaction_t* trans, const uint8_t* paramsBuffer, const size_t paramBytes) {
//     // Use internal buffer for small parameter sets (<= 4 bytes) to avoid DMA alignment issues
//     Serial.println(*paramsBuffer);
//     if (paramBytes <= 4) {
//         trans->flags = SPI_TRANS_USE_TXDATA;
//         for (size_t i = 0; i < paramBytes; i++) {
//             trans->tx_data[i] = paramsBuffer[i];
//         }
//     } else {
//         trans->tx_buffer = paramsBuffer;
//     }
//     trans->length = paramBytes * 8;
// }

