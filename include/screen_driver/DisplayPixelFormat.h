#ifndef GAME_CONSOLE_DISPLAY_PIXEL_FORMAT_H
#define GAME_CONSOLE_DISPLAY_PIXEL_FORMAT_H

enum DisplayPixelFormat {
    DISPLAY_16_BIT_PIXEL=0b101,
    DISPLAY_18_BIT_PIXEL=0b110,
    UNSET=0b000
};

struct ColorFormats {
    DisplayPixelFormat RGBInterfaceFormat; // This one is for the parallel thingy
    DisplayPixelFormat ControlInterfaceFormat; // We use this one (via SPI)
};

#endif
