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

#include "led_bsp.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
//static const char* TAG = __FILE__;
TaskHandle_t led_auto_Hand = NULL;
char a[] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
EventGroupHandle_t led_auto_group;
/* USER CODE BEGIN PV */
/*
 * 自定義模式
 */
void led_auto_test(void *pv){

	while (1) {
		xEventGroupWaitBits(led_auto_group,
				LED_AUTO_EVEAT, false, pdFALSE, portMAX_DELAY);

		for(int j = 0;j < 5; j ++)
		for (int i = 0; i < 8; i++) {
			writeSHRGB_byte(a[i]);
			writeRGB_byte(rand() % 16);
			gpio_set_level(red_EN, rand() % 2);
			gpio_set_level(green_EN, rand() % 2);
			gpio_set_level(blue_EN, rand() % 2);
			vTaskDelay(100 / portTICK_PERIOD_MS);
		}

		for (int i = 0; i < 20; i++) {
			writeSHRGB_byte(rand() % 16);
			writeRGB_byte(rand() % 16);
			gpio_set_level(red_EN, rand() % 2);
			gpio_set_level(green_EN, rand() % 2);
			gpio_set_level(blue_EN, rand() % 2);
			vTaskDelay(100 / portTICK_PERIOD_MS);
		}


		for (int i = 0; i < 16; i++) {
			writeSHRGB_byte(a[i]|a[i + 4]);
			writeRGB_byte(rand() % 16);
			gpio_set_level(red_EN, rand() % 2);
			gpio_set_level(green_EN, rand() % 2);
			gpio_set_level(blue_EN, rand() % 2);
			vTaskDelay(100 / portTICK_PERIOD_MS);
		}

		for (int j = 0; j < 8; j++) {
			writeSHRGB_byte(a[j]);
			for (int i = 0; i < 8; i++) {

				writeRGB_byte(~a[i]);
				gpio_set_level(red_EN, rand() % 2);
				gpio_set_level(green_EN, rand() % 2);
				gpio_set_level(blue_EN, rand() % 2);
				vTaskDelay(100 / portTICK_PERIOD_MS);
			}
		}

		for (int j = 0; j < 8; j++) {
			writeRGB_byte(~a[j]);
			for (int i = 0; i < 8; i++) {

				writeSHRGB_byte(a[i]);
				gpio_set_level(red_EN, rand() % 2);
				gpio_set_level(green_EN, rand() % 2);
				gpio_set_level(blue_EN, rand() % 2);
				vTaskDelay(50 / portTICK_PERIOD_MS);
			}
		}

	}
	vTaskDelete(NULL);
}

void auto_RGB(void) {

	if (led_auto_Hand == NULL) {
		xTaskCreatePinnedToCore(&led_auto_test, "led_auto_test", 4096, NULL, 4,
				&led_auto_Hand,0);
		xEventGroupSetBits(led_auto_group, LED_AUTO_EVEAT);

	} else {
		xEventGroupSetBits(led_auto_group, LED_AUTO_EVEAT);
		vTaskResume(led_auto_Hand);
	}
}

void open_RGB(void) {

	gpio_set_level(red_EN, 1);
	gpio_set_level(green_EN, 1);
	gpio_set_level(blue_EN, 1);
	writeRGB_byte(0x00);
	writeSHRGB_byte(0xff);
	gpio_set_level(red_EN, 0);
	gpio_set_level(green_EN, 0);
	gpio_set_level(blue_EN, 0);
}

void close_RGB(void){
	xEventGroupClearBits(led_auto_group, LED_AUTO_EVEAT);
	if(led_auto_Hand != NULL){
//		vTaskDelete(led_auto_Hand);

		vTaskSuspend(led_auto_Hand);
	}
	writeSHRGB_byte(0x00);

}

//青色
void cyan_RGB(void) {
	gpio_set_level(red_EN, 1);
	gpio_set_level(green_EN, 1);
	gpio_set_level(blue_EN, 1);
	writeRGB_byte(0xff);
	gpio_set_level(red_EN, 0);
	gpio_set_level(green_EN, 1);
	gpio_set_level(blue_EN, 0);
	writeRGB_byte(0x00);
	writeSHRGB_byte(0xff);
	gpio_set_level(red_EN, 0);
	gpio_set_level(green_EN, 0);
	gpio_set_level(blue_EN, 1);
}
void pink_RGB(void) {
	gpio_set_level(red_EN, 1);
	gpio_set_level(green_EN, 1);
	gpio_set_level(blue_EN, 1);
	writeRGB_byte(0xff);
	gpio_set_level(red_EN, 1);
	gpio_set_level(green_EN, 0);
	gpio_set_level(blue_EN, 1);
	writeRGB_byte(0x00);
	writeSHRGB_byte(0xff);
	gpio_set_level(red_EN, 0);
	gpio_set_level(green_EN, 0);
	gpio_set_level(blue_EN, 0);
}
void yellow_RGB(void) {
	gpio_set_level(red_EN, 1);
	gpio_set_level(green_EN, 1);
	gpio_set_level(blue_EN, 1);
	writeRGB_byte(0xff);
	gpio_set_level(red_EN, 1);
	gpio_set_level(green_EN, 1);
	gpio_set_level(blue_EN, 0);
	writeRGB_byte(0x00);
	writeSHRGB_byte(0xff);
	gpio_set_level(red_EN, 0);
	gpio_set_level(green_EN, 0);
	gpio_set_level(blue_EN, 0);
}

void red_RGB(void) {
	write_RGB(0x00, BLUE);
	writeSHRGB_byte(0xff);
}

void blue_RGB(void) {
	write_RGB(0x00, RED);
	writeSHRGB_byte(0xff);
}

void green_RGB(void) {
	write_RGB(0x00, GREEN);
	writeSHRGB_byte(0xff);
}

void write_RGB(char by,RGB_ RGB)
{
	if (RGB == BLUE) {
		gpio_set_level(red_EN, 1);
		gpio_set_level(green_EN, 1);
		gpio_set_level(blue_EN, 1);
		writeRGB_byte(0xff);
		gpio_set_level(red_EN, 1);
		gpio_set_level(green_EN, 0);
		gpio_set_level(blue_EN, 0);
	} else if (RGB == GREEN) {
		gpio_set_level(red_EN, 1);
		gpio_set_level(green_EN, 1);
		gpio_set_level(blue_EN, 1);
		writeRGB_byte(0xff);
		gpio_set_level(red_EN, 0);
		gpio_set_level(green_EN, 1);
		gpio_set_level(blue_EN, 0);
	} else if (RGB == RED) {
		gpio_set_level(red_EN, 1);
		gpio_set_level(green_EN, 1);
		gpio_set_level(blue_EN, 1);
		writeRGB_byte(0xff);
		gpio_set_level(red_EN, 0);
		gpio_set_level(green_EN, 0);
		gpio_set_level(blue_EN, 1);
	} else {
		gpio_set_level(red_EN, 1);
		gpio_set_level(green_EN, 1);
		gpio_set_level(blue_EN, 1);
	}

	writeRGB_byte(by);
	gpio_set_level(red_EN, 0);
	gpio_set_level(green_EN, 0);
	gpio_set_level(blue_EN, 0);
}
void writeRGB_byte (char by)
{
	gpio_set_level(RGB_1, 0x01&by);
	gpio_set_level(RGB_2, 0x02&by);
	gpio_set_level(RGB_3, 0x04&by);
	gpio_set_level(RGB_4, 0x08&by);
	gpio_set_level(RGB_5, 0x10&by);
	gpio_set_level(RGB_6, 0x20&by);
	gpio_set_level(RGB_7, 0x40&by);
	gpio_set_level(RGB_8, 0x80&by);
}

void writeSHRGB_byte (char by)
{
	gpio_set_level(SH_RGB_1, 0x01&by);
	gpio_set_level(SH_RGB_2, 0x02&by);
	gpio_set_level(SH_RGB_3, 0x04&by);
	gpio_set_level(SH_RGB_4, 0x08&by);
	gpio_set_level(SH_RGB_5, 0x10&by);
	gpio_set_level(SH_RGB_6, 0x20&by);
	gpio_set_level(SH_RGB_7, 0x40&by);
	gpio_set_level(SH_RGB_8, 0x80&by);
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

void led_bsp_init(void)
{
	gpio_config_t gpio_conf =
			{ .pin_bit_mask = Pin_All, .mode = GPIO_MODE_OUTPUT,
					.pull_up_en = 0, .pull_down_en = 0, .intr_type = 0 };
	gpio_config(&gpio_conf);
	gpio_set_direction(blue_EN,GPIO_MODE_OUTPUT);
	gpio_set_direction(green_EN,GPIO_MODE_OUTPUT);
	gpio_set_direction(red_EN,GPIO_MODE_OUTPUT);

//	xTaskCreate(&led_test, "led_test", 4096, NULL, 15, NULL);
	writeRGB_byte(0xff);
	writeSHRGB_byte(0x00);
	gpio_set_level(red_EN, 0);
	gpio_set_level(green_EN, 0);
	gpio_set_level(blue_EN, 0);

	led_auto_group = xEventGroupCreate();
	xEventGroupClearBits(led_auto_group, LED_AUTO_EVEAT);
}

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

