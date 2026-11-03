#ifndef ESP32_GAME_CONSOLE_COMMAND_BUFFER_H
#define ESP32_GAME_CONSOLE_COMMAND_BUFFER_H

#include <cstdint>
#include <string>
#include <algorithm>
#include <cstring>

#include "Colors.h"

enum class OpCode : uint8_t {
    RECT,
    TEXT
};

#pragma pack(push, 1)

struct CmdRectHeader {
    uint16_t x, y;
    uint16_t w, h;
    uint16_t color;
};

struct CmdTextHeader {
    uint16_t x, y;
    uint16_t textColor;
    uint16_t backgroundColor;
    uint16_t textLen; // Length of character data immediately following this struct
};

#pragma pack(pop)

class CommandBuffer {
private:
    static constexpr size_t BUFFER_CAPACITY = 4096;
    uint8_t buffer[BUFFER_CAPACITY] = {};
    size_t writeOffset = 0;


public:
    uint16_t TextColor = WHITE;
    uint16_t TextBackgroundColor = BLACK;

    void Clear() { writeOffset = 0; }
    const uint8_t* GetRawData() const { return buffer; }
    size_t GetSize() const { return writeOffset; }

    void WriteRaw(const void* data, size_t size) {
        assert(writeOffset + size <= BUFFER_CAPACITY);

        std::memcpy(&buffer[writeOffset], data, size);
        writeOffset += size;
    }

    template <typename T>
    void PushFixedCommand(OpCode op, const T& params) {
        assert(writeOffset + sizeof(OpCode) + sizeof(T) <= BUFFER_CAPACITY);

        buffer[writeOffset] = static_cast<uint8_t>(op);
        writeOffset++;

        std::memcpy(&buffer[writeOffset], &params, sizeof(T));
        writeOffset += sizeof(T);
    }

    void PushTextCommand(uint16_t x, uint16_t y, const char* text, uint16_t textLen) {
        OpCode op = OpCode::TEXT;
        CmdTextHeader header = { x, y, TextColor, TextBackgroundColor, textLen };
        assert(writeOffset + sizeof(OpCode) + sizeof(CmdTextHeader) + textLen <= BUFFER_CAPACITY);

        buffer[writeOffset++] = static_cast<uint8_t>(op);
        WriteRaw(&header, sizeof(CmdTextHeader));
        WriteRaw(text, textLen);
    }

};

#endif //ESP32_GAME_CONSOLE_COMMAND_BUFFER_H
