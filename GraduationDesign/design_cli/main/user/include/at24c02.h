/**
  ******************************************************************************
  * @file	: at24c02.h
  * @brief	: 
  * Details
  *
  * Created on			: 2019年3月17日
  * Author				: 123456
  *
  ******************************************************************************
  */
#ifndef MAIN_USER_INCLUDE_AT24C02_H_
#define MAIN_USER_INCLUDE_AT24C02_H_


#include "driver/i2c.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"


#define AT24c02_ADDR 0xa0
void AT24C02_Write_Byte(uint8_t addr,uint8_t data);
uint8_t AT24C02_Read_Byte(uint8_t addr);

#endif /* MAIN_USER_INCLUDE_AT24C02_H_ */
