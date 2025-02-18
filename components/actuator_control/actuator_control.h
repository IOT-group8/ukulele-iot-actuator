#ifndef ACTUATOR_CONTROL_H
#define ACTUATOR_CONTROL_H

#include <stdint.h>
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define SERVO_GPIO                   5   // PWM-capable GPIO for servo
#define SERVO_MIN_PULSE_WIDTH_US   500   // 0° pulse width (500µs)
#define SERVO_MAX_PULSE_WIDTH_US  2500   // 180° pulse width (2500µs)
#define SERVO_FREQUENCY             50   // Standard servo frequency (50Hz)

#define LEDC_TIMER           LEDC_TIMER_0
#define LEDC_MODE            LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL         LEDC_CHANNEL_0
#define LEDC_RESOLUTION      LEDC_TIMER_16_BIT // 16-bit resolution

#define ANGLE_STEP  2   // Incremental step (in degrees) when moving toward 45°
#define DELAY_MS    5   // Delay between each incremental step (in ms)
#define HOLD_DELAY 5000 // Hold delay in ms (5 seconds)
#define RELEASE_DELAY 1000 // Hold delay in ms (1 seconds)


// Convert angle (0-180°) to duty cycle value
uint32_t angle_to_duty(int angle);

// Initialize the servo PWM control
void servo_init(void);

// Servo stepping task
void servo_step_task(void *arg);

#endif // ACTUATOR_CONTROL_H
