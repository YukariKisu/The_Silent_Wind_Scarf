// servo.c
#include "servo.h"
#include "driver/ledc.h"
#include "esp_log.h"

static const char *TAG = "SERVO";

#define SERVO_GPIO      (18)   // GPIO18
#define SERVO_MIN_PULSEWIDTH 500   // Palse width 0
#define SERVO_MAX_PULSEWIDTH 2500  // Palse width 180
#define SERVO_MAX_DEGREE     180   // Max angle

// LEDC Channel
#define SERVO_LEDC_MODE       LEDC_LOW_SPEED_MODE
#define SERVO_LEDC_TIMER      LEDC_TIMER_0
#define SERVO_LEDC_CHANNEL    LEDC_CHANNEL_0
#define SERVO_LEDC_DUTY_RES   LEDC_TIMER_16_BIT 
#define SERVO_LEDC_FREQUENCY  50                 

void servo_init(void)
{
    
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = SERVO_LEDC_MODE,
        .timer_num        = SERVO_LEDC_TIMER,
        .duty_resolution  = SERVO_LEDC_DUTY_RES,
        .freq_hz          = SERVO_LEDC_FREQUENCY,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = SERVO_LEDC_MODE,
        .channel        = SERVO_LEDC_CHANNEL,
        .timer_sel      = SERVO_LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = SERVO_GPIO,
        .duty           = 0, // initial duty
        .hpoint         = 0
    };
    ledc_channel_config(&ledc_channel);

    ESP_LOGI(TAG, "Servo initialized on GPIO %d", SERVO_GPIO);
}

void servo_set_angle(int angle) // angle -> Duty value 
{
    if (angle < 0) angle = 0;
    if (angle > SERVO_MAX_DEGREE) angle = SERVO_MAX_DEGREE;

    
    uint32_t pulse_us = SERVO_MIN_PULSEWIDTH +
                        ((SERVO_MAX_PULSEWIDTH - SERVO_MIN_PULSEWIDTH) * angle) / SERVO_MAX_DEGREE;

    
    // Duty value -> 16 Bit                    
    uint32_t max_duty = (1 << 16); // 16bit = 65536
    float duty_f = ((float)pulse_us / 20000.0f) * (float)max_duty;
    uint32_t duty = (uint32_t)duty_f;

    // Update Duty value
    ledc_set_duty(SERVO_LEDC_MODE, SERVO_LEDC_CHANNEL, duty);
    ledc_update_duty(SERVO_LEDC_MODE, SERVO_LEDC_CHANNEL);
    

    ESP_LOGI(TAG, "Angle %d -> pulse %dus -> duty %d", angle, pulse_us, duty);
}
