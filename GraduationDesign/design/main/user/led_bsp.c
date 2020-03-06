/**
  ******************************************************************************
  * @file	: led_bsp.c
  * @brief	: 
  * Details
  *
  * Created on			: 2019年2月18日
  * Author				: 123456
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include "function.h"
#include "led_bsp.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
//static const char* TAG = __FILE__;

/* USER CODE BEGIN PV */
void led_mp3Mode(void *pv){
	while(1){
		gpio_set_level(GPIO_LED_GREEN, 1);
		vTaskDelay(1000);
		gpio_set_level(GPIO_LED_GREEN, 0);
		vTaskDelay(1000);
	}
}
void led_RGBMode(void *pv) {
	while (1) {
		gpio_set_level(GPIO_LED_GREEN, 1);
		vTaskDelay(500);
		gpio_set_level(GPIO_LED_GREEN, 0);
		vTaskDelay(500);
	}
}
void led_fanMode(void *pv) {
	while (1) {
		gpio_set_level(GPIO_LED_GREEN, 1);
		vTaskDelay(1000);
		gpio_set_level(GPIO_LED_GREEN, 0);
		vTaskDelay(500);
	}
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

void led_bsp_init(void)
{
	gpio_config_t gpio_conf = {
	        .pin_bit_mask = 1UL << GPIO_LED_GREEN,
	        .mode = GPIO_MODE_OUTPUT,
	        .pull_up_en = 0,
	        .pull_down_en = 0,
	        .intr_type = 0
	    };
	    gpio_config(&gpio_conf);

}

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

