#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"

#define SERVO_GPIO                   4   // PWM-capable GPIO for servo
#define SERVO_MIN_PULSE_WIDTH_US   500   // 0° pulse width (500µs)
#define SERVO_MAX_PULSE_WIDTH_US  2500   // 180° pulse width (2500µs)
#define SERVO_FREQUENCY             50   // Standard servo frequency (50Hz)

#define LEDC_TIMER           LEDC_TIMER_0
#define LEDC_MODE            LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL         LEDC_CHANNEL_0
#define LEDC_RESOLUTION      LEDC_TIMER_16_BIT // 16-bit resolution

#define ANGLE_STEP  2   // Incremental step (in degrees) when moving toward 45°
#define DELAY_MS    5   // Delay between each incremental step (in ms)
#define HOLD_DELAY 2000 // Hold delay in ms (2 seconds)

// Convert angle (0-180°) to duty cycle value
uint32_t angle_to_duty(int angle) {
    // Calculate the pulse width (in microseconds) corresponding to the given angle.
    uint32_t pulse_width = SERVO_MIN_PULSE_WIDTH_US + 
                           (angle * (SERVO_MAX_PULSE_WIDTH_US - SERVO_MIN_PULSE_WIDTH_US) / 180);
    // Convert pulse width (in µs) to duty based on the LEDC resolution and frequency.
    return (pulse_width * ((uint32_t)1 << LEDC_RESOLUTION)) / (1000000 / SERVO_FREQUENCY);
}

// Task to move the servo in one direction (increasing angle) for 45° and then pause.
void servo_step_task(void *arg) {
    while (1) {
        // Gradually move from 0° to 45° (only moving in one direction)
        for (int angle = 0; angle <= 45; angle += ANGLE_STEP) {
            uint32_t duty = angle_to_duty(angle);
            ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty);
            ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
            vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
        }
        // Hold at 45° for 2 seconds
        vTaskDelay(pdMS_TO_TICKS(HOLD_DELAY));

        // Reset the servo back to 0° immediately (to simulate one directional stepping)
        {
            uint32_t duty = angle_to_duty(0);
            ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty);
            ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
        }
        // Hold at 0° for 2 seconds before starting the next cycle
        vTaskDelay(pdMS_TO_TICKS(HOLD_DELAY));
    }
}

void app_main(void) {
    // Configure LEDC Timer for PWM signal generation.
    ledc_timer_config_t timer_conf = {
        .speed_mode       = LEDC_MODE,
        .duty_resolution  = LEDC_RESOLUTION,
        .timer_num        = LEDC_TIMER,
        .freq_hz          = SERVO_FREQUENCY,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer_conf);

    // Configure LEDC Channel to output the PWM signal on the desired GPIO.
    ledc_channel_config_t channel_conf = {
        .gpio_num   = SERVO_GPIO,
        .speed_mode = LEDC_MODE,
        .channel    = LEDC_CHANNEL,
        .intr_type  = LEDC_INTR_DISABLE,
        .timer_sel  = LEDC_TIMER,
        .duty       = angle_to_duty(0), // Start at 0°
        .hpoint     = 0
    };
    ledc_channel_config(&channel_conf);

    // Create the servo stepping task.
    xTaskCreate(servo_step_task, "servo_step_task", 2048, NULL, 5, NULL);
}
