/**
  ******************************************************************************
  * @file	: led_bsp.h
  * @brief	: 
  * Details
  *
  * Created on			: 2019年2月18日
  * Author				: 123456
  *
  ******************************************************************************
  */
#ifndef COMPONENTS_LED_BSP_INCLUDE_LED_BSP_H_
#define COMPONENTS_LED_BSP_INCLUDE_LED_BSP_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#define GPIO_LED_GREEN 22

TaskHandle_t ledMP3_Hand;
TaskHandle_t ledRGB_Hand;
TaskHandle_t ledfan_Hand;

void led_bsp_init(void);
void led_mp3Mode(void *pv);
void led_RGBMode(void *pv);
void led_fanMode(void *pv);


#endif /* COMPONENTS_LED_BSP_INCLUDE_LED_BSP_H_ */
