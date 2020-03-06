/**
  ******************************************************************************
  * @file	: myi2c.h
  * @brief	: 
  * Details
  *
  * Created on			: 2019年3月6日
  * Author				: 123456
  *
  ******************************************************************************
  */
#ifndef MAIN_USER_INCLUDE_MYI2C_H_
#define MAIN_USER_INCLUDE_MYI2C_H_

#include <stdio.h>
#include "driver/i2c.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"

//#define BYDtouch_ADS						0xC0

#define I2C_SCL_IO           3          		 /*!< gpio number for I2C master clock */
#define I2C_SDA_IO           1         		 /*!< gpio number for I2C master data  */

#define I2C_Part_NUM              I2C_NUM_0   		 /*!< I2C port number for master dev */
#define I2C_FirstPart_TX_BUF_DISABLE   0           		 /*!< I2C master do not need buffer */
#define I2C_FirstPart_RX_BUF_DISABLE   0           		 /*!< I2C master do not need buffer */
#define I2C_FirstPart_FREQ_HZ          400000       		 /*!< I2C master clock frequency */

#define WRITE_CMD                     0x00
#define WRITE_DATA                    0x40
#define WRITE_BIT  					  I2C_MASTER_WRITE 	 /*!< I2C master write */
#define READ_BIT   					  I2C_MASTER_READ  	 /*!< I2C master read */
#define ACK_CHECK_EN   				  0x1     			 /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS  				  0x0     			 /*!< I2C master will not check ack from slave */
#define ACK_VAL    					  0x0         		 /*!< I2C ack value */
#define NACK_VAL   					  0x1        		 /*!< I2C nack value */

/* Exported constants --------------------------------------------------------*/
typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

typedef enum {ERROR = 0, NOMAL = !ERROR} ErrorStatus;
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void I2C_GPIO_Init(void);
int IIC_write_cmd(uint8_t command,uint8_t addr);
int IIC_write_data(uint8_t data,uint8_t addr);
uint8_t IIC_read_data(uint8_t addr);
esp_err_t i2c_write(i2c_port_t i2c_num,uint8_t ADDER, uint8_t* data_wr, size_t size);
esp_err_t i2c_read(i2c_port_t i2c_num, uint8_t ADDER,uint8_t* data_rd, size_t size);
#endif /* MAIN_USER_INCLUDE_I2C_H_ */
