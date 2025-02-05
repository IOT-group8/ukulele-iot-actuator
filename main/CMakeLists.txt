idf_component_register(SRCS "main.c"
                       PRIV_REQUIRES spi_flash actuator_control
                       REQUIRES driver
                       INCLUDE_DIRS "")
