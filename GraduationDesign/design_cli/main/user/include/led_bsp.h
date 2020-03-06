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
#include "esp_log.h"
#define RGB_1 15
#define RGB_2 2
#define RGB_3 0
#define RGB_4 4
#define RGB_5 16
#define RGB_6 17
#define RGB_7 5
#define RGB_8 18
#define SH_RGB_1 19
#define SH_RGB_2 21
#define SH_RGB_3 22
#define SH_RGB_4 23
#define SH_RGB_5 25
#define SH_RGB_6 26
#define SH_RGB_7 27
#define SH_RGB_8 14
#define red_EN 33
#define green_EN 32
#define blue_EN GPIO_NUM_12
#define Pin_All ((1u<<RGB_1)|(1u<<RGB_2)|(1u<<RGB_3)|(1u<<RGB_4)|(1u<<RGB_5)|(1u<<RGB_6)|(1u<<RGB_7)|(1u<<RGB_8)\
		|(1u<<SH_RGB_1)|(1u<<SH_RGB_2)|(1u<<SH_RGB_3)|(1u<<SH_RGB_4)|(1u<<SH_RGB_5)|(1u<<SH_RGB_6)|(1u<<SH_RGB_7)|(1u<<SH_RGB_8)\
		|(1u<<red_EN)|(1u<<green_EN)|(1u<<blue_EN))

#define LED_AUTO_EVEAT BIT0


typedef enum {
    RED = 0x01,               /*!< Pad pull up            */
    GREEN = 0x02,             /*!< Pad pull down          */
    BLUE = 0x04,           /*!< Pad pull up + pull down*/
   RGB_ALL = 0x07,                  /*!< Pad floating           */
} RGB_;
void auto_RGB(void);
void close_RGB(void);
void open_RGB(void);
void cyan_RGB(void);
void pink_RGB(void);
void yellow_RGB(void);
void blue_RGB(void);
void red_RGB(void);
void green_RGB(void);
void led_bsp_init(void);
void writeRGB_byte (char by);
void write_RGB(char by,RGB_ RGB);
void writeSHRGB_byte (char by);
#endif /* COMPONENTS_LED_BSP_INCLUDE_LED_BSP_H_ */
