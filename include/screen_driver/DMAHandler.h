#ifndef GAME_CONSOLE_SPI_HANDLER_H
#define GAME_CONSOLE_SPI_HANDLER_H

#include "SPIHandler.h"
#include "driver/spi_master.h"

class DMAHandler {
    SPIHandler* spi = nullptr;
    static constexpr uint8_t BufferAmount = 2;
    spi_transaction_t trans[BufferAmount] = {};
    uint16_t* buffers[BufferAmount] = {nullptr, nullptr};

    void (*renderToBuffer)(uint16_t *buffers) = nullptr;

public:
    void Init(void (*renderFunction)(uint16_t *buffers), SPIHandler *spi);
    void Start();
    void End(spi_transaction_t *readBuffers[]) const;
    spi_transaction_t *Loop();


};


#endif // GAME_CONSOLE_SPI_HANDLER_H
