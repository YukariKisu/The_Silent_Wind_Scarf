#ifndef SERVO_H
#define SERVO_H
#include "driver/ledc.h"

#define SERVO_LEDC_MODE       LEDC_LOW_SPEED_MODE
#define SERVO_LEDC_CHANNEL    LEDC_CHANNEL_0

void servo_init(void);
void servo_set_angle(int angle);

#endif
