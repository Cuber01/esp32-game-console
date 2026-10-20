#ifndef ESP32_GAME_CONSOLE_COMMAND_BUFFER_H
#define ESP32_GAME_CONSOLE_COMMAND_BUFFER_H

#include <cstdint>
#include <string>
#include <algorithm>
#include <cstring>

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
    uint16_t color;
    uint16_t textLen; // Length of character data immediately following this struct
};

#pragma pack(pop)

class CommandBuffer {
private:
    static constexpr size_t BUFFER_CAPACITY = 4096;
    uint8_t buffer[BUFFER_CAPACITY] = {};
    size_t writeOffset = 0;

public:
    void Clear() { writeOffset = 0; }
    const uint8_t* GetRawData() const { return buffer; }
    size_t GetSize() const { return writeOffset; }

    bool WriteRaw(const void* data, size_t size) {
        if (writeOffset + size > BUFFER_CAPACITY) return false;

        std::memcpy(&buffer[writeOffset], data, size);
        writeOffset += size;
        return true;
    }

    template <typename T>
    bool PushFixedCommand(OpCode op, const T& params) {
        if (writeOffset + sizeof(OpCode) + sizeof(T) > BUFFER_CAPACITY) return false;

        buffer[writeOffset] = static_cast<uint8_t>(op);
        writeOffset++;

        std::memcpy(&buffer[writeOffset], &params, sizeof(T));
        writeOffset += sizeof(T);
        return true;
    }

    bool DrawText(uint16_t x, uint16_t y, const char* text, uint16_t textLen, uint16_t color) {
        OpCode op = OpCode::TEXT;
        CmdTextHeader header = { x, y, color, textLen };

        size_t totalSize = sizeof(OpCode) + sizeof(CmdTextHeader) + textLen;
        if (writeOffset + totalSize > BUFFER_CAPACITY) return false;

        buffer[writeOffset++] = static_cast<uint8_t>(op);
        WriteRaw(&header, sizeof(CmdTextHeader));
        WriteRaw(text, textLen); // Stream raw bytes without needing a null terminator
        return true;
    }

    bool DrawText(uint16_t x, uint16_t y, const char* text, uint16_t color) {
        return DrawText(x, y, text, static_cast<uint16_t>(strlen(text)), color);
    }

};

#endif //ESP32_GAME_CONSOLE_COMMAND_BUFFER_H
