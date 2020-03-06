/**
  ******************************************************************************
  * @file	: at24c02.c
  * @brief	: 
  * Details
  *
  * Created on			: 2019年3月17日
  * Author				: 123456
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include "myi2c.h"
#include "at24c02.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
const char* TAG = __FILE__;

/* USER CODE BEGIN PV */
void AT24C02_Write_Byte(uint8_t addr,uint8_t data)
{
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, ( AT24c02_ADDR) | WRITE_BIT, ACK_CHECK_EN);
	i2c_master_write_byte(cmd, (addr) | WRITE_BIT, ACK_CHECK_EN);
	i2c_master_write_byte(cmd, 0, ACK_CHECK_EN);
	i2c_master_write_byte(cmd, data, ACK_CHECK_EN);
	i2c_master_stop(cmd);
	esp_err_t ret = i2c_master_cmd_begin(I2C_Part_NUM, cmd,
			1000 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);
//	i2c_write(I2C_Part_NUM,AT24c02_ADDR, (uint8_t*) data, 1);
}

uint8_t AT24C02_Read_Byte(uint8_t addr)
{
	uint8_t *data_rd;
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, ( AT24c02_ADDR) | WRITE_BIT, ACK_CHECK_EN);
	i2c_master_write_byte(cmd, (addr) | WRITE_BIT, ACK_CHECK_EN);
	i2c_master_write_byte(cmd, (( AT24c02_ADDR + 1)) | READ_BIT,ACK_CHECK_EN);

//	i2c_master_write_byte(cmd, AT24c02_ADDR +1,ACK_CHECK_EN);
	i2c_master_read_byte(cmd, &data_rd, NACK_VAL);
	i2c_master_stop(cmd);
	esp_err_t ret = i2c_master_cmd_begin(I2C_Part_NUM, cmd,
			1000 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);
	return  *data_rd;
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

