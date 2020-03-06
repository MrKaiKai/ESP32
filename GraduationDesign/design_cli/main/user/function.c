/**
  ******************************************************************************
  * @file	: function.c
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
#include "function.h"
#include "tcp_bsp.h"
#include "key_bsp.h"
#include "led_bsp.h"
#include "oled.h"
#include "myled.h"
#include "myi2c.h"
#include "at24c02.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include <errno.h>
#include "esp_err.h"
#include "nvs_flash.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
const char* TAG_function = "function";
extern xQueueHandle key_queue;
KeyTCB_Def keyTCB;
SYS_Def sys;
xQueueHandle sys_queue = NULL;
/* USER CODE BEGIN PV */

/*
* 接收数据任务
* @param[in]   void  		       :无
* @retval      void                :无
* @note        修改日志
*               Ver0.0.1:
                    hx-zsj, 2018/08/06, 初始化版本\n
*/
void recv_data(void *pvParameters)
{
    int len = 0;            //长度
    char databuff[1024];    //缓存

    while (1)
    {
        //清空缓存
        memset(databuff, 0x00, sizeof(databuff));
        //读取接收数据
        len = recv(connect_socket, databuff, sizeof(databuff), 0);
        g_rxtx_need_restart = false;
        if (len > 0)
        {

            //打印接收到的数组
            ESP_LOGI(TAG_function, "recvData: %s", databuff);
//*************************接受数据进行处理*****************************//
//可以直接处理，也可发送队列
            char_struct(databuff);     //数据解析
            //接收数据回发
//            send(connect_socket, databuff, strlen(databuff), 0);
            //sendto(connect_socket, databuff , sizeof(databuff), 0, (struct sockaddr *) &remote_addr,sizeof(remote_addr));
//********************************************************************//
        }
        else
        {
            //打印错误信息
            show_socket_error_reason("recv_data", connect_socket);
            //服务器故障，标记重连
            g_rxtx_need_restart = true;

#if TCP_SERVER_CLIENT_OPTION
            //服务器接收异常，不用break后close socket,因为有其他client
            //break;
            vTaskDelete(NULL);
#else
            //client
            break;
#endif
        }
    }
    close_socket();
    //标记重连
    g_rxtx_need_restart = true;
    vTaskDelete(NULL);
}

void led_color(int i) {
	switch (i) {
	case 0:
		red_RGB();
		break;
	case 1:
		green_RGB();
		break;
	case 2:
		blue_RGB();
		break;
	case 3:
		open_RGB();
		break;
	case 4:
		yellow_RGB();
		break;
	case 5:
		pink_RGB();
		break;
	case 6:
		cyan_RGB();
		break;
	case 7:
		close_RGB();
		break;
	default:
		i = 0;
		break;
	}
}


void led_test (void *arg)
{
	uint32_t key;
	writeSHRGB_byte(0x00);
	static int ii = 0;
	while(1)
	{
		key = 0;
		if (xQueueReceive(key_queue, &key, portMAX_DELAY) == pdTRUE) {
			ESP_LOGI(TAG_function, "key receive: %d",key);
			if (key == PLAY) {

				led_color(ii);
				ii ++;
				if(ii > 8)ii = 0;
			}
			if (key == VOLDOWN) {
				write_RGB(0x00, GREEN);
				writeSHRGB_byte(0xff);
			}
			if (key == VOLUP) {
				write_RGB(0x00, BLUE);
				writeSHRGB_byte(0xff);
			}
			if (key == Set) {
				writeSHRGB_byte(0x00);
			}
			if (key == oPEN_THE_LIGHT) {

				 open_RGB();
			}
			if (key == cLOSE_THE_LIGHT) {
				close_RGB();
			}

		}
//		write_RGB(0x00, RED);
//		writeSHRGB_byte(0xff);
//		vTaskDelay(1000 / portTICK_PERIOD_MS);
//		write_RGB(0x00, GREEN);
//		writeSHRGB_byte(0xff);
//		vTaskDelay(1000 / portTICK_PERIOD_MS);
//		write_RGB(0x00, BLUE);
//		writeSHRGB_byte(0xff);
//		vTaskDelay(1000 / portTICK_PERIOD_MS);
//		write_RGB(0x00, 1000);
//		writeSHRGB_byte(0xff);
//		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

void fan_test(fan_Def arg) {
	switch (arg) {
	case fan_open:
		open_fan();
		break;
	case fan_close:
		close_fan();
		break;
	case fan_up:
		up_fan();
		break;
	case fan_down:
		down_fan();
		break;
	case fan_set:
		set_fan(sys.pow);
		break;
	default:
		break;
	}

}

void RGB_test(RGB_Def arg) {
	switch (arg) {
	case RGB_open:
		open_RGB();
		break;
	case RGB_close:
		close_RGB();
		break;
	case RGB_red:
		red_RGB();
		break;
	case RGB_blue:
		blue_RGB();
		break;
	case RGB_green:
		green_RGB();
		break;
	case RGB_pink:
		pink_RGB();
		break;
	case RGB_yellow:
		yellow_RGB();
		break;
	case RGB_cyan:
		cyan_RGB();
			break;
	case RGB_auto:
		auto_RGB();
			break;

	default:
		break;
	}
}

void data_test(void* pv){
	SYS_Def sys_msg = {0};
	while(1){

		if(xQueueReceive(sys_queue, &sys_msg, portMAX_DELAY) == pdTRUE){
			if(sys.myRGB != RGB_null)RGB_test(sys.myRGB);
			else if(sys.myfan != fan_null)fan_test(sys.myfan);
			else if(sys.mymp3 != mp3_null){
			}
			else{

			}
		}
	}
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */


/*
 * @信息解码
 * @2019-4-16
 */
void char_struct(char* a) {
	int b1, b2, b3,b4,b5;
	if (a[0] == 'T' && a[1] == ':') {
		sscanf(a, "T:%2d,%2d,%2d,%2d,%2d", &b1, &b2, &b3, &b4,&b5);
		keyTCB.KeyState = (KeyState_TypeDef) b1;
		keyTCB.ControlState = (ControlState_TypeDef) b2;
		keyTCB.Incident = (Incident_TypeDef) b3;
		keyTCB.Sub_mode = (KeyState_TypeDef)b4;
		keyTCB.WorkMode = (WorkMode_TypeDef)b5;
		xQueueSend(key_queue, &(keyTCB.Incident), 1000);
	}
	else if(a[0] == 'S' && a[1] == ':')
	{
		sscanf(a, "S:%2d,%2d,%2d,%d", &b1, &b2, &b3,&b4);
		sys.mymp3 = b1;
		sys.myRGB = b2;
		sys.myfan = b3;
		sys.pow = b4;
		xQueueSend(sys_queue, &sys, 1000);
	}

}
/*
 * @任务初始化
 *
 */
void function_init(void){

	key_queue = xQueueCreate(10, sizeof(uint32_t));
	sys_queue = xQueueCreate(10, sizeof(SYS_Def));

	xTaskCreate(&led_test, "led_test", 4096, NULL, 6, NULL);
	xTaskCreate(&data_test, "data_test", 4096, NULL, 6, NULL);
}
/* USER CODE END PFP */

