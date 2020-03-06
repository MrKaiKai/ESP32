/* Examples of speech recognition with multiple keywords.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"

#include "function.h"
#include "key_bsp.h"
#include "tcp_bsp.h"
#include "led_bsp.h"
#include "oled.h"
#include "myled.h"
#include "myi2c.h"
#include "at24c02.h"
#include "function.h"

void app_main(void)
{

//	uint8_t num_ [10];
//	uint8_t nm[] = {12,15,16,19,80,12,15,16,19,80};
	led_bsp_init();
	function_init();

	tcp_bsp_init();

	key_bsp_init();

	myled_init();




//	I2C_GPIO_Init();
//	OLED_Init();
//	OLED_Clear();
////	i2c_write(I2C_Part_NUM,AT24c02_ADDR,nm, 10);
////	xTaskCreate(&user_event_Handel, "user_event_Handel", 4096, NULL, 15, NULL);
////	OLED_ShowCH(0,0,"温度:");
//	AT24C02_Write_Byte(1,55);
//	AT24C02_Write_Byte(2,10);
//	OLED_ShowCH(0,2,"abc,413585122 ");
////	OLED_ShowFloatNum(0,4,12.3456,2,4,16);
////	OLED_ShowCH(0,6,"一");
////	num = AT24C02_Read_Byte(1);
//	i2c_read(I2C_Part_NUM,AT24c02_ADDR,num_, 10);
////	OLED_ShowCH(0,6,(char*)num);
//	for(int i = 0;i < 10;i ++)OLED_ShowFloatNum(0+i*24,4,num_[i],2,0,16);


//		OLED_ShowFloatNum(60,4,num,2,0,16);
//	while(1)
//	{
////		int num = 0;
////		PWM_Set(num);
////		if(num > 1000)num = 0;
////		vTaskDelay(1000 / portTICK_PERIOD_MS);
////		num += 10;
////		OLED_ShowFloatNum(0,4,num,4,0,16);
//	}
}
