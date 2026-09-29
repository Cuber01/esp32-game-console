#include "screen_driver/DMAHandler.h"
#include <driver/spi_master.h>
#include <driver/gpio.h>
#include "graphics.h"

void DMAHandler::Init(void (*renderFunction)(uint16_t* buffers), SPIHandler* spiHandler) {
    renderToBuffer = renderFunction;
    this->spi = spiHandler;

    // Setup buffers
    for (int i = 0; i < 2; i++) {
        buffers[i] = static_cast<uint16_t*>(
            heap_caps_malloc(SCREEN_WIDTH*SCREEN_HEIGHT*sizeof(uint16_t), MALLOC_CAP_DMA)
        );

        trans[i].length = SCREEN_WIDTH*SCREEN_HEIGHT * 16;
        trans[i].tx_buffer = buffers[i];
        trans[i].user = (void*)(uintptr_t)i;
    }

}

void DMAHandler::Start() {
    for (int i = 0; i < BufferAmount; i++) {
        renderToBuffer(buffers[i]);
        spi->QueueTransmit(&trans[i]);
    }
}

void DMAHandler::End(spi_transaction_t* readBuffers[BufferAmount]) const {
    for (int i = 0; i < BufferAmount; i++) {
        spi->GetTransactionResult(&(readBuffers[i]));
    }
}

spi_transaction_t* DMAHandler::Loop() {
    spi_transaction_t* completedTrans = nullptr;
    esp_err_t ret = spi->GetTransactionResult(&completedTrans);

    if (ret == ESP_OK) {
        int freedIdx = static_cast<int>(reinterpret_cast<uintptr_t>(completedTrans->user));

        renderToBuffer(buffers[freedIdx]);

        spi->QueueTransmit(&trans[freedIdx]);
    }
    return completedTrans;
}



