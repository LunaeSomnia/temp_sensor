#ifndef BME280_DRIVER_H_
#define BME280_DRIVER_H_

#include <stdint.h>
#include "driver/i2c_master.h"
#include "esp_err.h"

#define BME280_DIR                      0x76
#define BME280_CHIP_ID                  0xD0
#define BME280_CHIP_ID_VALUE            0x60
#define BME280_RESET                    0xE0
#define BME280_RESET_VALUE              0xB6
#define BME280_STATUS                   0xF3
#define BME280_STATUS_MEASURING         0x08
#define BME280_STATUS_IM_UPDATE         0x01
#define BME280_TIMEOUT_MS               100
#define BME280_CALIBRATION_REG_1        0x88
#define BME280_CALIBRATION_REG_1_BYTES  26
#define BME280_CALIBRATION_REG_2        0xE1
#define BME280_CALIBRATION_REG_2_BYTES  7
#define BME280_CTRL_MEAS_DIR            0xF4
#define BME280_CTRL_HUM_DIR             0xF2
#define BME280_DATA_DIR                 0xF7
#define BME280_DATA_BYTES               8

typedef enum bme280_ctrl_osrs {
    BME280_CTRL_MEAS_SKIP             = 0b000,
    BME280_CTRL_MEAS_OVERSAMPLING_1X  = 0b001,
    BME280_CTRL_MEAS_OVERSAMPLING_2X  = 0b010,
    BME280_CTRL_MEAS_OVERSAMPLING_4X  = 0b011,
    BME280_CTRL_MEAS_OVERSAMPLING_8X  = 0b100,
    BME280_CTRL_MEAS_OVERSAMPLING_16X = 0b101,
} bme280_ctrl_osrs;

typedef enum bme280_ctrl_meas_mode {
    BME280_CTRL_MEAS_SLEEP_MODE  = 0b00,
    BME280_CTRL_MEAS_FORCED_MODE = 0b01,
    BME280_CTRL_MEAS_NORMAL_MODE = 0b11,
} bme280_ctrl_meas_mode;

typedef struct bme280_data {
    float temperature;
    float pressure;
    float humidity;
} bme280_data;

typedef struct bme280_calibration_data {
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;
    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;
    uint8_t  dig_H1;
    int16_t  dig_H2;
    uint8_t  dig_H3;
    int16_t  dig_H4;
    int16_t  dig_H5;
    int8_t   dig_H6;

    int32_t t_fine;
} bme280_calibration_data;

typedef struct bme280_handle {
    i2c_master_dev_handle_t i2c_handle;
    bme280_calibration_data calibration_data;
} bme280_handle;

typedef struct bme280_config {
    i2c_master_dev_handle_t i2c_handle;
    bme280_calibration_data calibration_data;
} bme280_config;

esp_err_t bme280_initialize(i2c_master_bus_handle_t i2c_bus, bme280_handle *handle);
esp_err_t bme280_reset(bme280_handle *handle);
esp_err_t bme280_await_status_measuring(bme280_handle *handle);
esp_err_t bme280_await_status_im_update(bme280_handle *handle);
esp_err_t bme280_read_calibration(bme280_handle *handle);
esp_err_t bme280_read_raw_data(bme280_handle *handle, int32_t *raw_t, int32_t *raw_p, int32_t *raw_h);
esp_err_t bme280_read_data(bme280_handle *handle, bme280_data *data);
int32_t bme280_compensate_T_int32(bme280_handle *handle, int32_t adc_T);
int32_t bme280_compensate_P_int64(bme280_handle *handle, int32_t adc_P);
int32_t bme280_compensate_H_int32(bme280_handle *handle, int32_t adc_H);

#endif /* BME280_DRIVER_H_ */
