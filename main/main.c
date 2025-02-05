#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "actuator_control.h"

void app_main(void) {
    // Initialize the servo
    servo_init();

    // Create the servo stepping task
    xTaskCreate(servo_step_task, "servo_step_task", 2048, NULL, 5, NULL);
}
