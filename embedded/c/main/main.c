#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c_master.h"

#include "driver/bme280.h"

static i2c_master_bus_handle_t bus;
static bme280_handle bme280;

void app_main(void)
{
    printf("Hello world!\n");

    i2c_master_bus_config_t bus_cfg = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = GPIO_NUM_21,
        .scl_io_num = GPIO_NUM_22,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_cfg, &bus));

    for (uint8_t addr = 0x08; addr < 0x78; addr++) {
        if (i2c_master_probe(bus, addr, 50) == ESP_OK) {
            printf("found device at 0x%02X\n", addr);
        }
    }

    // Device initialization
    ESP_ERROR_CHECK(bme280_initialize(bus, &bme280));

    bme280_data data;
    ESP_ERROR_CHECK(bme280_read_data(&bme280, &data));

    printf("%f\n", data.temperature);
    printf("%f\n", data.pressure);
    printf("%f\n", data.humidity);

    vTaskDelay(10000 / portTICK_PERIOD_MS);
    fflush(stdout);
    esp_restart();
}
