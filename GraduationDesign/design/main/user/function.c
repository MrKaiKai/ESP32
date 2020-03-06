/**
  ******************************************************************************
  * @file	: function.c
  * @brief	: 
  * Details
  *
  * Created on			: 2019年4月4日
  * Author				: 123456
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include "function.h"
#include "tcp_bsp.h"
#include "touch_bsp.h"
#include "led_bsp.h"
#include "mymusic.h"

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
char* TAG_founction = "function";

/* USER CODE BEGIN PV */
/*
* 接收数据任务
* @param[in]   void  		       :无
* @retval      void                :无
* @note        修改日志
*               Ver0.0.1:
                    hx-zsj, 2018/08/06, 初始化版本\n
*/
void recv_data(void *pvParameters)                  //8086
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
            //g_total_data += len;
            //打印接收到的数组
            ESP_LOGI(TAG_founction, "recvData: %s", databuff);
//*************************接受数据进行处理*****************************//
//可以直接处理，也可发送队列



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
            flag_con = 0;
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
    flag_con = 0;
    close_socket();
    //标记重连
    g_rxtx_need_restart = true;
    vTaskDelete(NULL);
}
/*
* 接收数据任务
* @param[in]   void  		       :无
* @retval      void                :无
* @note        修改日志
*               Ver0.0.1:
                    hx-zsj, 2018/08/06, 初始化版本\n
*/
void recv_data1(void *pvParameters)        //8080
{
    int len = 0;            //长度
    char databuff[1024];    //缓存
    while (1)
    {
        //清空缓存
        memset(databuff, 0x00, sizeof(databuff));
        //读取接收数据
        len = recv(connect_socket1, databuff, sizeof(databuff), 0);
        g_rxtx_need_restart = false;
        if (len > 0)
        {
            //g_total_data += len;
            //打印接收到的数组
            ESP_LOGI(TAG_founction, "recvData1: %s", databuff);
//*************************接受数据进行处理*****************************//
//可以直接处理，也可发送队列
//作数据解析
			char c;
			int h;
			KeyTCB.ControlState = APP_control;
			if (databuff[0] == 'M') {                               //MP3模式
				sys.myRGB = RGB_null;
				sys.myfan = fan_null;
				sscanf(databuff, "M:%c,%d", &c,&h);
				if (c == 'P') {                     //大P播放
					sys.mymp3 = mp3_play;
					xQueueSend(sys_msg, &sys, SEND_TIME);
				}
				if (c == 'p') {                           //小p暂停
					sys.mymp3 = mp3_pause;
					xQueueSend(sys_msg, &sys, SEND_TIME);
				}
				if (c == 'r') {                               //继续
					sys.mymp3 = mp3_resume;
					xQueueSend(sys_msg, &sys, SEND_TIME);
				}
				if (c == 'm') {                               //静音
					sys.mymp3 = mp3_mute;
					xQueueSend(sys_msg, &sys, SEND_TIME);
				}
				if (c == 'n') {                               //下一曲
					sys.mymp3 = mp3_song_next;
					xQueueSend(sys_msg, &sys, SEND_TIME);
				}
				if (c == 'r') {                               //上一曲
					sys.mymp3 = mp3_song_previous;
					xQueueSend(sys_msg, &sys, SEND_TIME);
				}
				if (c == 'u') {                                //音量加大
					sys.mymp3 = mp3_volup;
					xQueueSend(sys_msg, &sys, SEND_TIME);
				}
				if (c == 'd') {                                //音量减小
					sys.mymp3 = mp3_voldown;
					xQueueSend(sys_msg, &sys, SEND_TIME);
				}
				if (c == 'v') {                                //设置音量
					sys.mymp3 = mp3_vol_set;
					sys.vol = h;
					xQueueSend(sys_msg, &sys, SEND_TIME);
				}

				if (c == 'l') {                                //扫描本地歌曲再播放
					sys.mymp3 = mp3_local_songs;
					xQueueSend(sys_msg, &sys, SEND_TIME);
				}
			} else if (databuff[0] == 'R') {                               //RGB模式
				sys.myfan = fan_null;
				sys.mymp3 = mp3_null;
				sscanf(databuff, "R:%c,%d", &c,&h);
				if (c == 'o') {                                //
					sys.myRGB = RGB_open;
					xQueueSend(sys_msg, &sys, SEND_TIME);
				}
				if (c == 'c') {                                //
					sys.myRGB = RGB_close;
					xQueueSend(sys_msg, &sys, SEND_TIME);
								}
				if (c == 'r') {                                //
					sys.myRGB = RGB_red;
					xQueueSend(sys_msg, &sys, SEND_TIME);
				}
				if (c == 'b') {                                //
					sys.myRGB = RGB_blue;
					xQueueSend(sys_msg, &sys, SEND_TIME);
				}
				if (c == 'g') {                                //
					sys.myRGB = RGB_green;
					xQueueSend(sys_msg, &sys, SEND_TIME);
				}
				if (c == 'p') {                                //
					sys.myRGB = RGB_pink;
					xQueueSend(sys_msg, &sys, SEND_TIME);
				}
				if (c == 'y') {                                //
					sys.myRGB = RGB_yellow;
					xQueueSend(sys_msg, &sys, SEND_TIME);
				}
				if (c == 'q') {                                //
					sys.myRGB = RGB_cyan;
					xQueueSend(sys_msg, &sys, SEND_TIME);
				}
				if (c == 'a') {                                //
					sys.myRGB = RGB_auto;
					xQueueSend(sys_msg, &sys, SEND_TIME);
				}
			} else if (databuff[0] == 'F') {                               //FAN模式
				sys.myRGB = RGB_null;
				sys.mymp3 = mp3_null;
				sscanf(databuff, "F:%c,%d", &c,&h);
				if (c == 'u') {                                //
					sys.myfan = fan_up;
//					xQueueSend(sys_msg, &sys, SEND_TIME);
				}
				if (c == 'd') {                                //
					sys.myfan = fan_down;
//					xQueueSend(sys_msg, &sys, SEND_TIME);
				}
				if (c == 's') {                                //
					sys.myfan = fan_set;
					sys.pow = h;
//					xQueueSend(sys_msg, &sys, SEND_TIME);
				}
				if (c == 'o') {                                //
					sys.myfan = fan_open;
//					xQueueSend(sys_msg, &sys, SEND_TIME);
				}
				if (c == 'c') {                                //
					sys.myfan = fan_close;
//					xQueueSend(sys_msg, &sys, SEND_TIME);
				}
			}
			xEventGroupSetBits(function_group, APP_BIT);
            //接收数据回发
//            send(connect_socket1, databuff, strlen(databuff), 0);

//********************************************************************//
        }
        else
        {
            //打印错误信息
            show_socket_error_reason("recv_data1", connect_socket1);
            //服务器故障，标记重连
            g_rxtx_need_restart1 = true;
            flag_con1 = 0;
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
    close_socket1();
    flag_con1 = 0;
    //标记重连
    g_rxtx_need_restart1 = true;
    vTaskDelete(NULL);
}


void send_data(void *pvParameters)
{
	EventBits_t data_falg = 0;
	char databuff[1024];    //缓存
	ssize_t ret = 0;

	while (1) {

		memset(databuff, 0x00, sizeof(databuff));
		data_falg = xEventGroupWaitBits(function_group,
		(TOUCH_EVEAT | function_BIT |APP_BIT), false, pdFALSE, portMAX_DELAY);
		if ((data_falg & TOUCH_EVEAT) == TOUCH_EVEAT) {
			memset(databuff, 0x00, sizeof(databuff));
			sprintf(databuff, "T:%d,%d,%d,%d,%d", KeyTCB.KeyState,
					KeyTCB.ControlState, KeyTCB.Incident, KeyTCB.Sub_mode,
					KeyTCB.WorkMode);
			ESP_LOGE(TAG_founction, "sendData: %s", databuff);
			ret = send(connect_socket, databuff, strlen(databuff), 0);
		}
		if ((data_falg & function_BIT) == function_BIT) {
			memset(databuff, 0x00, sizeof(databuff));
			sprintf(databuff, "S:%d,%d,%d,%d", sys.mymp3, sys.myRGB, sys.myfan,sys.pow);
			ESP_LOGE(TAG_founction, "sendData: %s", databuff);
			ret = send(connect_socket, databuff, strlen(databuff), 0);
		}
		if ((data_falg & APP_BIT) == APP_BIT) {
			memset(databuff, 0x00, sizeof(databuff));
			sprintf(databuff, "S:%d,%d,%d,%d", sys.mymp3, sys.myRGB, sys.myfan,
					sys.pow);
			ESP_LOGE(TAG_founction, "sendData: %s", databuff);
			ret = send(connect_socket, databuff, strlen(databuff), 0);
		}
		if (ret > 0) {
			xEventGroupClearBits(function_group, TOUCH_EVEAT);
			xEventGroupClearBits(function_group, function_BIT);
			xEventGroupClearBits(function_group, APP_BIT);
			KeyTCB.Incident = KeyUnPress;
		} else {
			//打印错误信息
			show_socket_error_reason("send_data", connect_socket);
			//服务器故障，标记重连
			flag_con = 0;
			g_rxtx_need_restart = true;
			vTaskDelete(NULL);
		}
	}
    close_socket();
    //标记重连
    flag_con = 0;
    g_rxtx_need_restart = true;
    vTaskDelete(NULL);
}


SYS_Def sys;
QueueHandle_t sys_msg;

void data_handling(void* pv) {
	KeyTCB_Def IncidentValue_key;
	WorkMode_TypeDef workmode;
	while (1) {

		if (xQueueReceive(xQueueKeyValue, &IncidentValue_key,
				portMAX_DELAY) == pdTRUE) {
			vTaskResume(listen_Hand);
			ESP_LOGE(TAG_founction,"Receive:%d,%d",IncidentValue_key.ControlState,IncidentValue_key.Incident);
			/* Handle touch pad events
			 to start, pause, resume, finish current song and adjust volume
			 */
			switch (IncidentValue_key.ControlState) {
			case ASR_control:
				xEventGroupWaitBits(asr_group,
						(TOUCH_EVEAT), false, pdFALSE, portMAX_DELAY);
				switch (IncidentValue_key.Incident) {
				case wAKE_UP:

					break;
				case oPEN_THE_LIGHT:
					sys.myRGB = RGB_open;
					xQueueSend(sys_msg, &sys, SEND_TIME);

					break;
				case cLOSE_THE_LIGHT:
					sys.myRGB = RGB_close;
					xQueueSend(sys_msg, &sys, SEND_TIME);
					break;
				case vOLUME_INCREASE:
					sys.mymp3 = mp3_volup;
					xQueueSend(sys_msg, &sys, SEND_TIME);
					ESP_LOGI(TAG_founction,"mp3_volup");
					break;
				case vOLUME_DOWN:
					sys.mymp3 = mp3_voldown;
					xQueueSend(sys_msg, &sys, SEND_TIME);
					ESP_LOGI(TAG_founction,"mp3_voldown");
					break;
				case pLAY:
					sys.mymp3 = mp3_play;
					xQueueSend(sys_msg, &sys,SEND_TIME);
					ESP_LOGI(TAG_founction,"mp3_play");
					break;
				case pAUSE:
					sys.mymp3 = mp3_pause;
					xQueueSend(sys_msg, &sys, SEND_TIME);
					ESP_LOGI(TAG_founction,"mp3_pause");
					break;
				case mUTE:
					sys.mymp3 = mp3_mute;
					xQueueSend(sys_msg, &sys, SEND_TIME);
					ESP_LOGI(TAG_founction,"mp3_mute");
					break;
				case pLAY_LOCAL_MUSIC:

					break;
				default:
					break;
				}
				xEventGroupClearBits(asr_group, TOUCH_EVEAT);
				break;
			case TOUCH_control:
//							if (IncidentValue_key.WorkMode == MP3_Mode) {           //判断是否为MP3
				switch (IncidentValue_key.Incident) {
				case play:
				{
					switch (KeyTCB.WorkMode) {
					case MP3_Mode:
						sys.mymp3 = mp3_play;
						sys.myRGB = RGB_null;
						sys.myfan = fan_null;
						xQueueSend(sys_msg, &sys, 1000);
						break;
					case RGB_Mode:
						sys.myRGB = RGB_open;
						sys.mymp3 = mp3_null;
						sys.myfan = fan_null;
						xQueueSend(sys_msg, &sys, 1000);
						break;
					case FAN_Mode:
						sys.myfan = fan_open;
						sys.mymp3 = mp3_null;
						sys.myRGB = RGB_null;
						xQueueSend(sys_msg, &sys, 1000);
						break;
					default:

						break;
					}
				}
					break;
				case set:

					switch (IncidentValue_key.KeyState) {
					case press:

						break;
					case loosen:

						break;
					case longpress:

						workmode = KeyTCB.WorkMode;
						if (workmode == MP3_Mode)
							KeyTCB.WorkMode = RGB_Mode;
						if (workmode == RGB_Mode)
							KeyTCB.WorkMode = FAN_Mode;
						if (workmode == FAN_Mode)
							KeyTCB.WorkMode = MP3_Mode;
						KeyTCB.Sub_mode = first_mode;
						ESP_LOGI(TAG_founction, "MODE:%d", KeyTCB.WorkMode);
//						xQueueSend(xQueueKeyValue, &KeyTCB, 1000);
						IncidentValue_key.KeyState = null;
						break;
					case putdown:

						break;
					default:
						break;
					}
					break;
				case volDWN:
					//
					switch (IncidentValue_key.KeyState) {
					case press:

						if (KeyTCB.WorkMode == RGB_Mode) {
//													sys.myRGB = RGB_play;
//													xQueueSend(sys_msg, &sys, 1000);
						}
						if (KeyTCB.WorkMode == FAN_Mode) {
							sys.myfan = fan_down;
							xQueueSend(sys_msg, &sys, 1000);
						}
						break;
					case loosen:
						if (KeyTCB.WorkMode == MP3_Mode) {
							sys.mymp3 = mp3_song_next;
							xQueueSend(sys_msg, &sys, 1000);
						}
						break;
					case longpress:
						if (KeyTCB.WorkMode == MP3_Mode) {
							sys.mymp3 = mp3_voldown_;
							xQueueSend(sys_msg, &sys, 1000);
						}
						break;
					case putdown:
						if (KeyTCB.WorkMode == MP3_Mode) {
							sys.mymp3 = mp3_null;
							xQueueSend(sys_msg, &sys, 1000);
						}
						break;
					default:
						break;
					}
					//
					break;
				case volUP:
					//

					switch (IncidentValue_key.KeyState) {
					case press:

						if (KeyTCB.WorkMode == RGB_Mode) {
//							sys.myRGB = RGB_play;
//							xQueueSend(sys_msg, &sys, 1000);
						}
						if (KeyTCB.WorkMode == FAN_Mode) {
							sys.myfan = fan_up;
							xQueueSend(sys_msg, &sys, 1000);
						}
						break;
					case loosen:
						if (KeyTCB.WorkMode == MP3_Mode) {
							sys.mymp3 = mp3_song_previous;
							xQueueSend(sys_msg, &sys, 1000);
						}
						break;
					case longpress:
						if (KeyTCB.WorkMode == MP3_Mode) {
							sys.mymp3 = mp3_volup_;
							xQueueSend(sys_msg, &sys, 1000);
						}
						break;
					case putdown:
						if (KeyTCB.WorkMode == MP3_Mode) {
							sys.mymp3 = mp3_null;
							xQueueSend(sys_msg, &sys, 1000);
						}
						break;
					default:
						break;
					}


					break;
				default:
					break;
				}

				break;
/*
 * APP控制程序放在了接受数据中，此处不再列举
 */
			case APP_control:
				switch (IncidentValue_key.Incident) {
				case ASR_control:

					break;
				case TOUCH_control:

					break;
				case APP_control:

					break;
				default:
					break;
				}
/*
 * end
 */
				break;
			default:
				break;
			}
			xEventGroupSetBits(function_group, function_BIT);
			vTaskResume(listen_Hand);
		}

	}
}

void led_listen(void *pv)
{
	while(1){
		if (KeyTCB.WorkMode == MP3_Mode) {
			vTaskResume(ledMP3_Hand);
			vTaskSuspend(ledRGB_Hand);
			vTaskSuspend(ledfan_Hand);
		} else if (KeyTCB.WorkMode == RGB_Mode) {
			vTaskSuspend(ledMP3_Hand);
			vTaskResume(ledRGB_Hand);
			vTaskSuspend(ledfan_Hand);
		} else if (KeyTCB.WorkMode == FAN_Mode) {
			vTaskSuspend(ledMP3_Hand);
			vTaskSuspend(ledRGB_Hand);
			vTaskResume(ledfan_Hand);
		} else if (KeyTCB.KeyState == longpress) {
			vTaskSuspend(ledMP3_Hand);
			vTaskSuspend(ledRGB_Hand);
			vTaskSuspend(ledfan_Hand);
			gpio_set_level(GPIO_LED_GREEN, 1);
		} else if (KeyTCB.KeyState == putdown) {
			vTaskSuspend(ledMP3_Hand);
			vTaskSuspend(ledRGB_Hand);
			vTaskSuspend(ledfan_Hand);
			gpio_set_level(GPIO_LED_GREEN, 0);
		}
		vTaskSuspend(listen_Hand);
	}
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */


void function_Init(void)
{

	xTaskCreatePinnedToCore(&data_handling, "data_handling", 4096*1, NULL, DATA_PRIO, NULL,1);
	xTaskCreatePinnedToCore(&music_test, "music_test", 4096*4, NULL,MUSIC_PRIO, NULL,0);
	xTaskCreatePinnedToCore(&asr_test, "asr_test", 4096*1, NULL, ASR_PRIO, NULL,1);
	xTaskCreatePinnedToCore(&led_listen, "led_listen", 2048, NULL, 4, &listen_Hand,1);
	vTaskSuspend(listen_Hand);
    xTaskCreatePinnedToCore(&led_mp3Mode, "led_mp3Mode", 512, NULL, 3, &ledMP3_Hand,1);
    xTaskCreatePinnedToCore(&led_RGBMode, "led_RGBMode", 512, NULL, 3, &ledRGB_Hand,1);
    xTaskCreatePinnedToCore(&led_fanMode, "led_fanMode", 512, NULL, 3, &ledfan_Hand,1);
    vTaskSuspend(ledMP3_Hand);
    vTaskSuspend(ledRGB_Hand);
    vTaskSuspend(ledfan_Hand);
}
/* USER CODE END PFP */

