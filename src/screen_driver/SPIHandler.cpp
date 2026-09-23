#include "screen_driver/SPIHandler.h"
#include <driver/spi_master.h>
#include <driver/gpio.h>
#include "graphics.h"


void SPIHandler::Init() {
    esp_err_t err = ESP_OK;

    // Set config for data command pin
    gpio_config_t ioConfig = {
        .pin_bit_mask = (1ULL << SCREEN_DC),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    err = gpio_config(&ioConfig);
    assert(err == ESP_OK);

    // Configure SPI Bus (MOSI used for bidirectional SDA)
    spi_bus_config_t busConfig = {
        .mosi_io_num = SCREEN_SDA, // both Input/Output
        .miso_io_num = -1,         // Not used since we use SDA as mosi+miso
        .sclk_io_num = SCREEN_SCL,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4096,
    };
    // SPI2_HOST is available for user in ESP32. SPI1_HOST isn't
    err = spi_bus_initialize(SPI2_HOST, &busConfig, SPI_DMA_CH_AUTO);
    assert(err == ESP_OK);

    // Configure Device with HALF-DUPLEX
    spi_device_interface_config_t deviceConfig = {
        .mode = 0,                          // SPI Mode
        .clock_speed_hz = 2 * 1000 * 1000, // 6.6 MHz
        .spics_io_num = SCREEN_CS,
        .flags = SPI_DEVICE_HALFDUPLEX | SPI_DEVICE_3WIRE,      // Crucial for 1-wire bidirectional read
        .queue_size = 7,
    };

    err = spi_bus_add_device(SPI2_HOST, &deviceConfig, &spiHandle);
    assert(err == ESP_OK);
}

esp_err_t SPIHandler::GpioWrite(uint8_t pin, const bool level) {
    return gpio_set_level(static_cast<gpio_num_t>(pin), level ? 1 : 0);
}



esp_err_t SPIHandler::Transmit(spi_transaction_t* transaction) {
    return spi_device_transmit(spiHandle, transaction);
}

