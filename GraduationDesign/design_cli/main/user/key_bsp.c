/**
  ******************************************************************************
  * @file	: key_bsp.c
  * @brief	: 
  * Details
  *
  * Created on			: 2019年3月20日
  * Author				: 123456
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include "key_bsp.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
const char* TAG_key = __FILE__;
xQueueHandle key_queue = NULL;
/* USER CODE BEGIN PV */
//gpio_get_level();
static void IRAM_ATTR key_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    if (xQueueSendFromISR(key_queue, &gpio_num, NULL)!= pdTRUE) {
    ESP_LOGI(TAG_key, "key set：　%d",gpio_num);
    }
}

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */
void key_bsp_init(void)
{
	gpio_config_t gpio_conf = { .pin_bit_mask = key_all,
			.mode = GPIO_MODE_INPUT, .pull_up_en = 0, .pull_down_en = 0,
			.intr_type = GPIO_INTR_NEGEDGE };
	gpio_config(&gpio_conf);

	//install gpio isr service
	gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
	//hook isr handler for specific gpio pin
	gpio_isr_handler_add(key_play, key_handler, (void*) PLAY);
	gpio_isr_handler_add(key_set, key_handler, (void*) Set);
	gpio_isr_handler_add(key_volDown, key_handler, (void*) VOLDOWN);
	gpio_isr_handler_add(key_volUp, key_handler, (void*) VOLUP);
	ESP_LOGI(TAG_key, "key init OK!");
}
/* USER CODE END PFP */

