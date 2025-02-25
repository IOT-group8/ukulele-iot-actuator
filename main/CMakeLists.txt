idf_component_register(SRCS "main.c"
                       PRIV_REQUIRES spi_flash actuator_control mqtt_connect firebase_http
                       REQUIRES driver
                       INCLUDE_DIRS "")
