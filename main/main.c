#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "actuator_control.h"
#include "wifi_connect.h"

void app_main(void) {
    // Initialize Wi-Fi
    wifi_init_sta();

    // Initialize MQTT
    mqtt_app_start();

    // Initialize the servo
    servo_init();

    // Create the servo stepping task
    xTaskCreate(servo_step_task, "servo_step_task", 2048, NULL, 5, NULL);
}
