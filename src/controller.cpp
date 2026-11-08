#include "controller.h"
#include "freertos/FreeRTOS.h"
#include <FreeRTOSConfig.h>
#include <portmacro.h>
#include <freertos/projdefs.h>
#include <freertos/queue.h>
#include "freertos/task.h"
#include "driver/gpio.h"
#include "screen_driver/Core.h"

void inputTask(void *pvParameters);
InputKeys controllerGetInput();

QueueHandle_t inputQueue;
InputKeys lastKeyPressed = INPUT_NONE;

void ControllerInit()
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

    inputQueue = xQueueCreate(10, sizeof(InputKeys));

    // Create the task and pin it to Core 0 (leaving Core 1 for rendering/logic)
    xTaskCreatePinnedToCore(
        inputTask,     // Task function
        "InputTask",    // Name
        2048,           // Stack size
        nullptr,        // Parameters
        3,              // Priority (higher than rendering)
        nullptr,        // Task handle
        0               // Core ID (0)
    );
}

void inputTask(void *pvParameters) {
    InputKeys lastState = INPUT_NONE;

    while (true) {
        InputKeys currentState = controllerGetInput();

        if (currentState != lastState) {
            xQueueSend(inputQueue, &currentState, 0);
            lastState = currentState;
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

InputKeys controllerGetInput()
{
    if (gpio_get_level(static_cast<gpio_num_t>(INPUT_FORWARD))  == 0) return INPUT_FORWARD;
    if (gpio_get_level(static_cast<gpio_num_t>(INPUT_BACKWARD)) == 0) return INPUT_BACKWARD;
    if (gpio_get_level(static_cast<gpio_num_t>(INPUT_RIGHT))    == 0) return INPUT_RIGHT;
    if (gpio_get_level(static_cast<gpio_num_t>(INPUT_LEFT))     == 0) return INPUT_LEFT;
    if (gpio_get_level(static_cast<gpio_num_t>(INPUT_PRESS))    == 0) return INPUT_PRESS;

    return INPUT_NONE;
}

InputKeys ControllerGetLastKeyPressed() {
    xQueueReceive(inputQueue, &lastKeyPressed, 0);
    return lastKeyPressed;
}