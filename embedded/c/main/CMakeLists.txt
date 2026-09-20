idf_component_register(SRCS
                           "main.c"
                           "driver/bme280.c"
                       PRIV_REQUIRES spi_flash
                       INCLUDE_DIRS
                           "driver/"
                       REQUIRES esp_driver_i2c)
