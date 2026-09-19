#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    printf("Hello world!\n");

    vTaskDelay(10000 / portTICK_PERIOD_MS);
    fflush(stdout);
    esp_restart();
}
