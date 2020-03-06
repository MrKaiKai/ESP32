/**
  ******************************************************************************
  * @file	: key_bsp.h
  * @brief	: 
  * Details
  *
  * Created on			: 2019年3月20日
  * Author				: 123456
  *
  ******************************************************************************
  */
#ifndef MAIN_USER_INCLUDE_KEY_BSP_H_
#define MAIN_USER_INCLUDE_KEY_BSP_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define key_play    35
#define key_set     34
#define key_volDown 39
#define key_volUp   36
#define key_all ((1ULL<<key_play) | (1ULL<<key_set)|(1ULL<<key_volDown)|(1ULL<<key_volUp))
#define ESP_INTR_FLAG_DEFAULT 0
extern xQueueHandle key_queue;

typedef enum {
	KeyUnPress = 0x00,
    PLAY = 10,
    Set = 11,
    VOLDOWN = 12,
   VOLUP = 13,
} KEY_;


void key_bsp_init(void);

#endif /* MAIN_USER_INCLUDE_KEY_BSP_H_ */
