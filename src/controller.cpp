#include <stdio.h>
#include "controller.h"
#include "driver/gpio.h"
#include "driver/gpio.h"

void controllerInit()
{
    // Configure all input pins at once using gpio_config
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << INPUT_FORWARD)  |
                        (1ULL << INPUT_BACKWARD) |
                        (1ULL << INPUT_RIGHT)    |
                        (1ULL << INPUT_LEFT)     |
                        (1ULL << INPUT_PRESS),
        .mode         = GPIO_MODE_INPUT,
        .pull_up_en   = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);
}

InputKeys controllerGetInput()
{
    // gpio_get_level returns 0 (LOW) or 1 (HIGH)
    if (gpio_get_level(static_cast<gpio_num_t>(INPUT_FORWARD))  == 0) return INPUT_FORWARD;
    if (gpio_get_level(static_cast<gpio_num_t>(INPUT_BACKWARD)) == 0) return INPUT_BACKWARD;
    if (gpio_get_level(static_cast<gpio_num_t>(INPUT_RIGHT))    == 0) return INPUT_RIGHT;
    if (gpio_get_level(static_cast<gpio_num_t>(INPUT_LEFT))     == 0) return INPUT_LEFT;
    if (gpio_get_level(static_cast<gpio_num_t>(INPUT_PRESS))    == 0) return INPUT_PRESS;

    return INPUT_NONE;
}