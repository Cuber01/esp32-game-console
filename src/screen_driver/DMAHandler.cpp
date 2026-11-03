#include "screen_driver/DMAHandler.h"
#include <driver/spi_master.h>
#include <driver/gpio.h>
#include "graphics.h"
#include <screen_driver/CommandBuffer.h>

#include "screen_driver/Commands.h"
#include "screen_driver/Core.h"
#include "screen_driver/Font.h"

void DMAHandler::RasterizeChunk(uint16_t* chunkBuf, int chunkIdx, const CommandBuffer& cmdBuffer) {
    memset(chunkBuf, 0, CHUNK_WIDTH * CHUNK_HEIGHT * sizeof(uint16_t));

    int chunkStartY = chunkIdx * CHUNK_HEIGHT;
    int chunkEndY = chunkStartY + CHUNK_HEIGHT - 1;

    const uint8_t* ptr = cmdBuffer.GetRawData();
    const uint8_t* end = ptr + cmdBuffer.GetSize();

    while (ptr < end) {
        OpCode op = static_cast<OpCode>(*(ptr++));

        switch (op) {
            case OpCode::TEXT: {
                auto header = reinterpret_cast<const CmdTextHeader*>(ptr);
                ptr += sizeof(CmdTextHeader);

                RectOverlap overlap = GetRectChunkOverlap(chunkStartY, chunkEndY, header->y, CHAR_H);

                // Is overlap
                if (overlap.end != 0) {
                    const char* txt = reinterpret_cast<const char*>(ptr);
                    
                    for (int i = 0; i < header->textLen; i++) {
                        char* bitmap = Font[static_cast<int>(txt[i])];

                        for (int y = overlap.start; y <= overlap.end; y++) {
                            int localY = y - chunkStartY;
                            for (int localX = 0; localX < CHAR_W; localX++) {
                                uint8_t set = (bitmap[localX] >> localY) & 1;

                                chunkBuf[localY * CHUNK_WIDTH +
                                         header->x + localX +
                                         i*CHAR_W + CHAR_SPACE] =
                                         (set ? __builtin_bswap16(header->textColor)
                                              : __builtin_bswap16(header->backgroundColor));
                            }
                        }
                    }

                    ptr += header->textLen;
                }


            }

            case OpCode::RECT: {
                auto header = reinterpret_cast<const CmdRectHeader*>(ptr);
                ptr += sizeof(CmdRectHeader);

                RectOverlap overlap = GetRectChunkOverlap(chunkStartY, chunkEndY, header->y, header->h);
                if (overlap.end != 0) // Is overlap
                {
                    for (int y = overlap.start; y <= overlap.end; y++) {
                        int localY = y - chunkStartY;
                        for (int x = header->x; x < header->x + header->w; x++) {
                            chunkBuf[localY * CHUNK_WIDTH + x] = __builtin_bswap16(header->color);
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
    RectOverlap overlap = {0, 0};
    bool doesOverlap = false;

    // Starts inside of chunk
    if (rectY >= chunkStart && rectY < chunkEnd) {
        overlap.start = rectY;
        doesOverlap = true;
    // Starts above chunk and overlaps
    } else if (rectY < chunkStart && rectY + rectH > chunkStart) {
        overlap.start = chunkStart;
        doesOverlap = true;
    }

    if (doesOverlap) {
        if (rectY + rectH >= chunkEnd) {
            overlap.end = chunkEnd;
        }
        else {
            overlap.end = rectY + rectH;
        }
    }
    return overlap;
}

void DMAHandler::Init(SPIHandler* spiHandler, CommandBuffer* cmdBuffer, Core* driverCore) {
    this->spi = spiHandler;
    this->userCmds = cmdBuffer;
    this->core = driverCore;

    // Setup buffers
    for (int i = 0; i < BufferAmount; i++) {
        buffers[i] = static_cast<uint16_t*>(
            heap_caps_malloc(CHUNK_WIDTH*CHUNK_HEIGHT*sizeof(uint16_t), MALLOC_CAP_DMA)
        );

        trans[i].length = CHUNK_WIDTH * CHUNK_HEIGHT * sizeof(uint16_t) * 8;
        trans[i].flags = SPI_TRANS_CS_KEEP_ACTIVE;
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
    spi->AcquireBus();

    // core->SetColumnsAddress(0, SCREEN_WIDTH - 1);
    // core->SetRowsAddress(0, SCREEN_HEIGHT - 1);

    spi->GpioWrite(SCREEN_DC, false);

    spi_transaction_t ramwrTrans = {};
    ramwrTrans.flags = SPI_TRANS_USE_TXDATA | SPI_TRANS_CS_KEEP_ACTIVE;
    ramwrTrans.tx_data[0] = RAMWR;
    ramwrTrans.length = 8;

    spi->TransmitWithManualCS(&ramwrTrans);
    spi->GpioWrite(SCREEN_DC, true);

    for (int i = 0; i < BufferAmount; i++) {
        trans[i].flags = SPI_TRANS_CS_KEEP_ACTIVE; // Restore flag
        RasterizeChunk(buffers[i], i, *userCmds);
        volatile const esp_err_t err = spi->QueueTransmit(&trans[i]);
        ESP_ERROR_CHECK(err);
    }
}

spi_transaction_t* DMAHandler::Loop(uint8_t nextChunkId) {
    spi_transaction_t* completedTrans = nullptr;
    esp_err_t ret = spi->GetTransactionResult(&completedTrans);

    if (ret == ESP_OK) {
        int freedIdx = static_cast<int>(reinterpret_cast<uintptr_t>(completedTrans->user));

        if (nextChunkId == CHUNK_AMOUNT-1) {
            trans[freedIdx].flags = 0; // Clear KEEP CS ACTIVE flag for the last buffer
        }

        RasterizeChunk(buffers[freedIdx], nextChunkId, *userCmds);
        volatile const esp_err_t err = spi->QueueTransmit(&trans[freedIdx]);
        ESP_ERROR_CHECK(err);
    }
    return completedTrans;
}

void DMAHandler::EndLoop(spi_transaction_t* readBuffers[BufferAmount]) const {
    spi->ReleaseBus();

    volatile esp_err_t err = ESP_OK;
    for (int i = 0; i < BufferAmount; i++) {
        if (readBuffers == nullptr) {
            err = spi->GetTransactionResult(nullptr);
        } else {
            err = spi->GetTransactionResult(&(readBuffers[i]));
        }
        ESP_ERROR_CHECK(err);
    }
}



