#include "./bme280.h"
#include "driver/i2c_master.h"
#include "esp_err.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"

static const i2c_device_config_t BME280_I2C_CONFIG = {
    .dev_addr_length = I2C_ADDR_BIT_LEN_7,
    .device_address = BME280_DIR,
    .scl_speed_hz = 100000,
};

// static const bme280_config BME280_CONFIG {

// };

esp_err_t bme280_initialize(i2c_master_bus_handle_t i2c_bus, bme280_handle *handle) {
    ESP_ERROR_CHECK(i2c_master_bus_add_device(i2c_bus, &BME280_I2C_CONFIG, &handle->i2c_handle));

    uint8_t write_buffer[] = {BME280_CHIP_ID};
    uint8_t read_buffer = 0x00;

    ESP_ERROR_CHECK(i2c_master_transmit_receive(handle->i2c_handle, write_buffer, sizeof(write_buffer), &read_buffer, 1, BME280_TIMEOUT_MS));

    if (read_buffer != BME280_CHIP_ID_VALUE)
        return ESP_ERR_INVALID_RESPONSE;

    ESP_ERROR_CHECK(bme280_reset(handle));

    return bme280_read_calibration(handle);
}

esp_err_t bme280_reset(bme280_handle *handle) {
    uint8_t write_buffer[] = {BME280_RESET, BME280_RESET_VALUE};

    ESP_ERROR_CHECK(i2c_master_transmit(handle->i2c_handle, write_buffer, sizeof(write_buffer), BME280_TIMEOUT_MS));

    // 10ms delay (2ms specified in datasheet + 8ms margin)
    vTaskDelay(pdMS_TO_TICKS(10));

    return bme280_await_status_im_update(handle);
}

esp_err_t bme280_await_status_measuring(bme280_handle *handle) {
    uint8_t write_buffer[] = {BME280_STATUS};

    for (int i = 0; i < 50; i++) {
        uint8_t status;
        ESP_ERROR_CHECK(i2c_master_transmit_receive(handle->i2c_handle, write_buffer, 1, &status, 1, BME280_TIMEOUT_MS));

        if (!(status & BME280_STATUS_MEASURING)) return ESP_OK;
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    return ESP_ERR_TIMEOUT;
}

esp_err_t bme280_await_status_im_update(bme280_handle *handle) {
    uint8_t write_buffer[] = {BME280_STATUS};

    for (int i = 0; i < 50; i++) {
        uint8_t status;
        ESP_ERROR_CHECK(i2c_master_transmit_receive(handle->i2c_handle, write_buffer, 1, &status, 1, BME280_TIMEOUT_MS));

        if (!(status & BME280_STATUS_IM_UPDATE)) return ESP_OK;
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    return ESP_ERR_TIMEOUT;
}

esp_err_t bme280_read_calibration(bme280_handle *handle) {
    uint8_t reg_1[] = {BME280_CALIBRATION_REG_1};
    uint8_t reg_1_read[BME280_CALIBRATION_REG_1_BYTES];
    ESP_ERROR_CHECK(i2c_master_transmit_receive(handle->i2c_handle, reg_1, 1, reg_1_read, BME280_CALIBRATION_REG_1_BYTES, BME280_TIMEOUT_MS));
    handle->calibration_data.dig_T1 = (uint16_t) ((reg_1_read[1]  << 8) | reg_1_read[0]);
    handle->calibration_data.dig_T2 = (int16_t)  ((reg_1_read[3]  << 8) | reg_1_read[2]);
    handle->calibration_data.dig_T3 = (int16_t)  ((reg_1_read[5]  << 8) | reg_1_read[4]);
    handle->calibration_data.dig_P1 = (uint16_t) ((reg_1_read[7]  << 8) | reg_1_read[6]);
    handle->calibration_data.dig_P2 = (int16_t)  ((reg_1_read[9]  << 8) | reg_1_read[8]);
    handle->calibration_data.dig_P3 = (int16_t)  ((reg_1_read[11] << 8) | reg_1_read[10]);
    handle->calibration_data.dig_P4 = (int16_t)  ((reg_1_read[13] << 8) | reg_1_read[12]);
    handle->calibration_data.dig_P5 = (int16_t)  ((reg_1_read[15] << 8) | reg_1_read[14]);
    handle->calibration_data.dig_P6 = (int16_t)  ((reg_1_read[17] << 8) | reg_1_read[16]);
    handle->calibration_data.dig_P7 = (int16_t)  ((reg_1_read[19] << 8) | reg_1_read[18]);
    handle->calibration_data.dig_P8 = (int16_t)  ((reg_1_read[21] << 8) | reg_1_read[20]);
    handle->calibration_data.dig_P9 = (int16_t)  ((reg_1_read[23] << 8) | reg_1_read[22]);
    handle->calibration_data.dig_H1 = (uint8_t)  reg_1_read[25];

    uint8_t reg_2[] = {BME280_CALIBRATION_REG_2};
    uint8_t reg_2_read[BME280_CALIBRATION_REG_2_BYTES];
    ESP_ERROR_CHECK(i2c_master_transmit_receive(handle->i2c_handle, reg_2, 1, reg_2_read, BME280_CALIBRATION_REG_2_BYTES, BME280_TIMEOUT_MS));
    handle->calibration_data.dig_H2 = (int16_t)  ((reg_2_read[1]  << 8) | reg_2_read[0]);
    handle->calibration_data.dig_H3 = (uint8_t)  reg_2_read[2];
    handle->calibration_data.dig_H4 = (int16_t)  ((((int8_t) reg_2_read[3]) * 16) | (reg_2_read[4] & 0x0F));
    handle->calibration_data.dig_H5 = (int16_t)  ((((int8_t) reg_2_read[5]) * 16) | (reg_2_read[4] >> 4));
    handle->calibration_data.dig_H6 = (int8_t)   reg_2_read[6];

    return ESP_OK;
}

esp_err_t bme280_read_raw_data(bme280_handle *handle, int32_t *raw_t, int32_t *raw_p, int32_t *raw_h) {
    // Set the ctrl_hum to oversampling x1 (enabling)
    uint8_t ctrl_hum[] = {BME280_CTRL_HUM_DIR, BME280_CTRL_MEAS_OVERSAMPLING_1X};
    ESP_ERROR_CHECK(i2c_master_transmit(handle->i2c_handle, ctrl_hum, 2, BME280_TIMEOUT_MS));

    // Set the ctrl_meas to oversampling x1 (enabling) for temperature, pressure and forced mode
    const uint8_t ctrl_meas_value = (BME280_CTRL_MEAS_OVERSAMPLING_1X << 5) | (BME280_CTRL_MEAS_OVERSAMPLING_1X << 2) | BME280_CTRL_MEAS_FORCED_MODE;
    uint8_t ctrl_meas[] = {BME280_CTRL_MEAS_DIR, ctrl_meas_value};
    ESP_ERROR_CHECK(i2c_master_transmit(handle->i2c_handle, ctrl_meas, 2, BME280_TIMEOUT_MS));

    // 100ms delay (2tick, 20ms specified in datasheet + 80ms margin)
    vTaskDelay(pdMS_TO_TICKS(100));

    ESP_ERROR_CHECK(bme280_await_status_measuring(handle));

    // Burst read data
    uint8_t data_reg[] = {BME280_DATA_DIR};
    uint8_t data_reg_read[BME280_DATA_BYTES];
    ESP_ERROR_CHECK(i2c_master_transmit_receive(handle->i2c_handle, data_reg, 1, data_reg_read, BME280_DATA_BYTES, BME280_TIMEOUT_MS));
    *raw_p = (int32_t) ((((uint32_t) data_reg_read[0]) << 12) | (((uint32_t) data_reg_read[1]) << 4) | (((uint32_t) data_reg_read[2] >> 4)));
    *raw_t = (int32_t) ((((uint32_t) data_reg_read[3]) << 12) | (((uint32_t) data_reg_read[4]) << 4) | (((uint32_t) data_reg_read[5] >> 4)));
    *raw_h = (int32_t) ((data_reg_read[6] << 8) | (data_reg_read[7]));

    return ESP_OK;
}

int32_t bme280_compensate_T_int32(bme280_handle *handle, int32_t adc_T)
{
    int32_t var1, var2, t;
    var1 = ((((adc_T>>3) - ((int32_t)handle->calibration_data.dig_T1<<1))) * ((int32_t)handle->calibration_data.dig_T2)) >> 11;
    var2 = (((((adc_T>>4) - ((int32_t)handle->calibration_data.dig_T1)) * ((adc_T>>4) - ((int32_t)handle->calibration_data.dig_T1)))
    >> 12) *
    ((int32_t)handle->calibration_data.dig_T3)) >> 14;
    handle->calibration_data.t_fine = var1 + var2;
    t = (handle->calibration_data.t_fine * 5 + 128) >> 8;
    return t;
}

int32_t bme280_compensate_P_int64(bme280_handle *handle, int32_t adc_P)
{
    int64_t var1, var2, p;
    var1 = ((int64_t)handle->calibration_data.t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)handle->calibration_data.dig_P6;
    var2 = var2 + ((var1*(int64_t)handle->calibration_data.dig_P5)<<17);
    var2 = var2 + (((int64_t)handle->calibration_data.dig_P4)<<35);
    var1 = ((var1 * var1 * (int64_t)handle->calibration_data.dig_P3)>>8) + ((var1 * (int64_t)handle->calibration_data.dig_P2)<<12);
    var1 = (((((int64_t)1)<<47)+var1))*((int64_t)handle->calibration_data.dig_P1)>>33;
    if (var1 == 0)
    {
        return 0; // avoid exception caused by division by zero
    }
    p = 1048576-adc_P;
    p = (((p<<31)-var2)*3125)/var1;
    var1 = (((int64_t)handle->calibration_data.dig_P9) * (p>>13) * (p>>13)) >> 25;
    var2 = (((int64_t)handle->calibration_data.dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)handle->calibration_data.dig_P7)<<4);
    return (int32_t)p;
}


int32_t bme280_compensate_H_int32(bme280_handle *handle, int32_t adc_H)
{
    int32_t v_x1_u32r;
    v_x1_u32r = (handle->calibration_data.t_fine - ((int32_t)76800));
    v_x1_u32r = (((((adc_H << 14) - (((int32_t)handle->calibration_data.dig_H4) << 20) - (((int32_t)handle->calibration_data.dig_H5) *
    v_x1_u32r)) + ((int32_t)16384)) >> 15) * (((((((v_x1_u32r *
    ((int32_t)handle->calibration_data.dig_H6)) >> 10) * (((v_x1_u32r * ((int32_t)handle->calibration_data.dig_H3)) >> 11) +
    ((int32_t)32768))) >> 10) + ((int32_t)2097152)) * ((int32_t)handle->calibration_data.dig_H2) +
    8192) >> 14));
    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
    ((int32_t)handle->calibration_data.dig_H1)) >> 4));
    v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
    v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
    return (int32_t)(v_x1_u32r >> 12);
}

esp_err_t bme280_read_data(bme280_handle *handle, bme280_data *data) {

    int32_t raw_t;
    int32_t raw_p;
    int32_t raw_h;
    ESP_ERROR_CHECK(bme280_read_raw_data(handle, &raw_t, &raw_p, &raw_h));

    data->temperature = bme280_compensate_T_int32(handle, raw_t) / 100.0f;
    data->pressure = bme280_compensate_P_int64(handle, raw_p) / 256.0f;
    data->humidity =  bme280_compensate_H_int32(handle, raw_h) / 1024.0f;

    return ESP_OK;
}
