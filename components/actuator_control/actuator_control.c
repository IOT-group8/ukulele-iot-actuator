#include "actuator_control.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Define servo parameters for MG996R
#define MG996R_MIN_PULSE_WIDTH_US  500     // Minimum pulse width (0°)
#define MG996R_MAX_PULSE_WIDTH_US  2500    // Maximum pulse width (180°)
#define SERVO_FREQUENCY            50      // 50Hz for standard servos

// Convert angle (0-180°) to duty cycle value for MG996R
uint32_t angle_to_duty(int angle) {
    uint32_t pulse_width = MG996R_MIN_PULSE_WIDTH_US + 
                           (angle * (MG996R_MAX_PULSE_WIDTH_US - MG996R_MIN_PULSE_WIDTH_US) / 180);
    return (pulse_width * ((uint32_t)1 << LEDC_RESOLUTION)) / (1000000 / SERVO_FREQUENCY);
}

// Initialize servo motor for MG996R
void servo_init(void) {
    ledc_timer_config_t timer_conf = {
        .speed_mode       = LEDC_MODE,
        .duty_resolution  = LEDC_RESOLUTION,
        .timer_num        = LEDC_TIMER,
        .freq_hz          = SERVO_FREQUENCY,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer_conf);

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
}

// Task to move the MG996R servo from 0° to 45° and back.
void servo_step_task(void *arg) {
    while (1) {
        // Gradually move from 0° to 45°
        for (int angle = 0; angle <= 150; angle += ANGLE_STEP) {
            uint32_t duty = angle_to_duty(angle);
            ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty);
            ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
            vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
        }
        vTaskDelay(pdMS_TO_TICKS(HOLD_DELAY));

        // Reset servo back to 0°
        uint32_t duty = angle_to_duty(0);
        ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty);
        ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
        vTaskDelay(pdMS_TO_TICKS(RELEASE_DELAY));
    }
}
