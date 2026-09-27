#ifndef GAME_CONSOLE_DISPLAY_SETTINGS_H
#define GAME_CONSOLE_DISPLAY_SETTINGS_H

enum DisplaySettings {
    DISPLAY_16_BIT_PIXEL=0b101,
    DISPLAY_18_BIT_PIXEL=0b110,
    UNSET=0b000
};

struct ColorFormats {
    DisplaySettings RGBInterfaceFormat; // This one is for the parallel thingy
    DisplaySettings ControlInterfaceFormat; // We use this one (via SPI)
};

#endif
