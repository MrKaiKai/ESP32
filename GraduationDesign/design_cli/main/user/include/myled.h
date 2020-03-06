/**
  ******************************************************************************
  * @file	: myled.h
  * @brief	: 
  * Details
  *
  * Created on			: 2019年3月7日
  * Author				: 123456
  *
  ******************************************************************************
  */
#ifndef MAIN_USER_INCLUDE_MYLED_H_
#define MAIN_USER_INCLUDE_MYLED_H_

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "driver/gpio.h"
#include "esp_log.h"
#define PWM_TIMER          LEDC_TIMER_0
#define PWM_MODE           LEDC_LOW_SPEED_MODE
#define PWM_GPIO       (13)
#define PWM_CHANNEL    LEDC_CHANNEL_0
#define PWM_OFF 0

#define FAN_S          (uint16_t)500
#define FAN_MAX          (uint16_t)1000
#define FAN_MIN          (uint16_t)200
#define FAN_critical          (uint16_t)100

extern uint16_t fan_Speed;

void set_fan(int pow);
void open_fan(void);
void close_fan(void);
void up_fan(void);
void down_fan(void);
void myled_init(void);
void PWM_Set(uint16_t num);

#endif /* MAIN_USER_INCLUDE_MYLED_H_ */
