#include "screen_driver/DMAHandler.h"
#include <driver/spi_master.h>
#include <driver/gpio.h>
#include "graphics.h"
#include <screen_driver/CommandBuffer.h>

#include "screen_driver/Commands.h"

void DMAHandler::RasterizeChunk(uint16_t* chunkBuf, int chunkIdx, const CommandBuffer& cmdBuffer) {
    memset(chunkBuf, 0, CHUNK_WIDTH * CHUNK_HEIGHT * sizeof(uint16_t));

    int chunkStartY = chunkIdx * CHUNK_HEIGHT;
    int chunkEndY = chunkStartY + CHUNK_HEIGHT - 1;

    const uint8_t* ptr = cmdBuffer.GetRawData();
    const uint8_t* end = ptr + cmdBuffer.GetSize();

    while (ptr < end) {
        OpCode op = static_cast<OpCode>(*ptr++);

        switch (op) {
            case OpCode::TEXT: {
                break;
            }

            case OpCode::RECT: {
                auto header = reinterpret_cast<const CmdRectHeader*>(ptr);
                ptr += sizeof(CmdRectHeader);

                RectOverlap overlap = GetRectChunkOverlap(chunkStartY, chunkEndY, header->y, header->h);
                if (overlap.end != 0) // Is valid
                {
                    for (int y = overlap.start; y < overlap.end; y++) {
                        for (int x = 0; x < CHUNK_WIDTH; x++) {
                            chunkBuf[y * CHUNK_WIDTH + x] = header->color;
                        }
                    }
                }

            }
            default:
                break;
        }
    }
}

DMAHandler::RectOverlap DMAHandler::GetRectChunkOverlap(uint16_t chunkStart, uint16_t chunkEnd, uint16_t rectY, uint16_t rectH) {
    auto* overlap = new RectOverlap();
    bool doesOverlap = false;

    // Starts inside of chunk
    if (rectY > chunkStart && rectY < chunkEnd) {
        overlap->start = rectY;
        doesOverlap = true;
    // Starts above chunk and overlaps
    } else if (rectY < chunkStart && rectY + rectH > chunkStart) {
        overlap->start = chunkStart;
        doesOverlap = true;
    }

    if (doesOverlap) {
        if (rectY + rectH >= chunkEnd) {
            overlap->end = chunkEnd;
        }
        else {
            overlap->end = rectY + rectH;
        }
    }
    return *overlap;
}

void DMAHandler::Init(SPIHandler* spiHandler, CommandBuffer* cmdBuffer) {
    this->spi = spiHandler;
    this->userCmds = cmdBuffer;

    // Setup buffers
    for (int i = 0; i < BufferAmount; i++) {
        buffers[i] = static_cast<uint16_t*>(
            heap_caps_malloc(CHUNK_WIDTH*CHUNK_HEIGHT*sizeof(uint16_t), MALLOC_CAP_DMA)
        );

        trans[i].length = CHUNK_WIDTH*CHUNK_HEIGHT*sizeof(uint16_t);
        trans[i].tx_buffer = buffers[i];
        trans[i].user = (void*)(uintptr_t)i;
    }

}

void DMAHandler::RenderFrame() {
    uint8_t chunksSent = BufferAmount;
    StartLoop(); // Fills all buffers and as such sends the amount of chunks equal to amount of buffer

    // Wait for a buffer to finish sending and send the rest of chunks
    while (chunksSent < CHUNK_AMOUNT) {
        Loop(chunksSent);
        chunksSent++;
    }
    EndLoop(nullptr);
}


void DMAHandler::StartLoop() {
    spi->GpioWrite(SCREEN_DC, false);
    spi->GpioWrite(SCREEN_CS, false);

    spi_transaction_t commandTrans = {};
    commandTrans.flags = SPI_TRANS_USE_TXDATA;
    commandTrans.tx_data[0] = RAMWR;
    commandTrans.length = 8;

    esp_err_t err = spi->Transmit(&commandTrans, SCREEN_CS);

    for (int i = 0; i < BufferAmount; i++) {
        RasterizeChunk(buffers[i], i, *userCmds);
        spi->QueueTransmit(&trans[i]);
    }

    spi->GpioWrite(SCREEN_DC, true);
}

spi_transaction_t* DMAHandler::Loop(uint8_t nextChunkId) {
    spi_transaction_t* completedTrans = nullptr;
    esp_err_t ret = spi->GetTransactionResult(&completedTrans);

    if (ret == ESP_OK) {
        int freedIdx = static_cast<int>(reinterpret_cast<uintptr_t>(completedTrans->user));
        RasterizeChunk(buffers[freedIdx], nextChunkId, *userCmds);
        spi->QueueTransmit(&trans[freedIdx]);
    }
    return completedTrans;
}

void DMAHandler::EndLoop(spi_transaction_t* readBuffers[BufferAmount]) const {
    for (int i = 0; i < BufferAmount; i++) {
        spi->GetTransactionResult(&(readBuffers[i]));
    }

    spi->GpioWrite(SCREEN_CS, true);
}



