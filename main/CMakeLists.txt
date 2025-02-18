idf_component_register(SRCS "main.c"
                       PRIV_REQUIRES spi_flash actuator_control mqtt_client
                       REQUIRES driver
                       INCLUDE_DIRS "")
