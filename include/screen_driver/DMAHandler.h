#ifndef GAME_CONSOLE_SPI_HANDLER_H
#define GAME_CONSOLE_SPI_HANDLER_H

#include "CommandBuffer.h"
#include "SPIHandler.h"
#include "driver/spi_master.h"

class DMAHandler {
    SPIHandler* spi = nullptr;
    CommandBuffer* userCmds = nullptr;

    static constexpr uint8_t BufferAmount = 2;
    spi_transaction_t trans[BufferAmount] = {};
    uint16_t* buffers[BufferAmount] = {{}, {}};

    void (*renderToBuffer)(uint16_t *buffers) = nullptr;

    typedef struct RectOverlap {
        uint16_t start;
        uint16_t end;
    } RectOverlap;

public:
    void RasterizeChunk(uint16_t *chunkBuf, int chunkIdx, const CommandBuffer &cmdBuffer);

    RectOverlap GetRectChunkOverlap(uint16_t chunkStart, uint16_t chunkEnd, uint16_t rectY, uint16_t rectH);

    bool RectChunkOverlap(int chunkStart, int chunkEnd, int rectY, int rectH);

    void Init(SPIHandler *spiHandler, CommandBuffer *userCmds);

    void RenderFrame();

    void StartLoop();
    void EndLoop(spi_transaction_t *readBuffers[]) const;
    spi_transaction_t *Loop(uint8_t nextChunkId);


};


#endif // GAME_CONSOLE_SPI_HANDLER_H
