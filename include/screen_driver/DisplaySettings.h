#ifndef GAME_CONSOLE_DISPLAY_SETTINGS_H
#define GAME_CONSOLE_DISPLAY_SETTINGS_H

enum DisplaySettings {
    DISPLAY_16_BIT_PIXEL=0b101,
    DISPLAY_18_BIT_PIXEL=0b110,
    UNSET=0b000
};

// Reminder: On ESP32 in unions bits are assembled from LSB to MSB (right to left)
union DisplayConfig {
    struct {
        DisplaySettings ControlInterfaceFormat: 4; // For SPI
        DisplaySettings RGBInterfaceFormat : 4;    // For parallel
    } bits;

    uint8_t raw;
};

union MadctlConfig {
    struct {
        uint8_t reserved : 2; // Unused
        bool mh          : 1; // Horizontal refresh order (0 left-right, 1 right-left)
        bool rgb         : 1; // RGB/BGR Order (0 RGB, 1 BGR)
        bool ml          : 1; // Vertical Refresh Order (0 top-bottom, 1 bottom-top)
        bool mv          : 1; // Row/Column Exchange (0 "portrait" coords h>w, 1 "landscape" coords w>h)
        bool mx          : 1; // Column Address Order (1 decrement, 0 increment)
        bool my          : 1; // Row Address Order (1 decrement, 0 increment)
    } bits;

    uint8_t raw;
};

#endif
