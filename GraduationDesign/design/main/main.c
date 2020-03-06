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

#include "tcp_bsp.h"
#include "led_bsp.h"
#include "asr_bsp.h"
#include "touch_bsp.h"
#include "mymusic.h"
#include "function.h"
//static const char *TAG = "example_asr_keywords";


void app_main(void)
{

	KeyTCB.WorkMode = MP3_Mode;
	function_group = xEventGroupCreate();
	xEventGroupClearBits(function_group, TOUCH_EVEAT);
	xEventGroupClearBits(function_group, function_BIT);
	xEventGroupClearBits(function_group, APP_BIT);
	sys_msg = xQueueCreate(3, sizeof(SYS_Def));

	tcp_bsp_init();

	led_bsp_init();

	touch_bsp_init();

	mymusic_init();

	asr_bsp_init();

	function_Init ();
}
