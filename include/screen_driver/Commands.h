#ifndef ESP32_GAME_CONSOLE_COMMANDS_H
#define ESP32_GAME_CONSOLE_COMMANDS_H

enum Commands {
    RDDCOLMOD=0x0C, // Read color mode
    COLMOD=0x3A, // Set color mode
    RDDID=0x04, // Read device idea
    CASET=0x2A, // Set address column
    RASET=0x2B, // Set address row
    RAMWR=0x2C, // Write to RAM
    RAMRD=0x2E, // Read from RAM
    SLPIN=0x10, // Turn on sleep mode
    SLPOUT=0x11, // Turn off sleep mode
    DISPON=0x28, // Turn on display
    DISPOFF=0x29, // Turn off display
    SWRESET=0x01, // Software reset
    MADCTL=0x36,
    INVON=0x21,
    NORON=0x13,
};

#endif
