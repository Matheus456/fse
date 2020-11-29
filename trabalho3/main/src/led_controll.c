#include "sdkconfig.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_controll.h"
#define LED 2

void config_led()
{
    gpio_pad_select_gpio(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);
}

void pisca_led()
{
    gpio_set_level(LED, 0);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    gpio_set_level(LED, 1);
    vTaskDelay(500 / portTICK_PERIOD_MS);
}
