/**
 ******************************************************************************
 * @file	: touch_bsp.c
 * @brief	:
 * Details
 *
 * Created on		: 2019年2月18日
 * Author				: 123456
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include "touch_bsp.h"
#include "led_bsp.h"
#include "function.h"
#include "mymusic.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "audio_element.h"
#include "audio_pipeline.h"
#include "audio_event_iface.h"
#include "audio_common.h"
#include "fatfs_stream.h"
#include "i2s_stream.h"
#include "mp3_decoder.h"
#include "fatfs_stream.h"
#include "esp_log.h"
#include "esp_peripherals.h"
#include "periph_sdcard.h"
#include "periph_button.h"
#include "periph_touch.h"

#if MYMUSIC

#endif
/* USER CODE END Includes */
const char *TAG_Touch = "Touch_bsp";


audio_event_iface_cfg_t evt_cfg = { 0 };
QueueHandle_t xQueueKeyValue;
KeyTCB_Def KeyTCB;


#if TOUCH_eg

audio_event_iface_msg_t msg_touch;
audio_event_iface_handle_t evt_touch = { 0 };
static esp_periph_handle_t touch_periph = { 0 };
KeyTCB_Def KeyTCB1;

#endif

///* Private variables ---------------------------------------------------------*/

///* USER CODE BEGIN PV */

#if TOUCH_RXT_TEST
void touch_rxt_test(void *arg) {
	KeyTCB_Def IncidentValue;
	while (1) {
		if (xQueueReceive(xQueueKeyValue, &IncidentValue,
						portMAX_DELAY) == pdTRUE) {
			/* Handle touch pad events
			 to start, pause, resume, finish current song and adjust volume
			 */
			switch (IncidentValue.Incident) {
				case play: {
					ESP_LOGI(TAG_Touch, "[ <-- ] [Play] touch tap event");
					break;
				}
				case set: {
					ESP_LOGI(TAG_Touch, "[ <-- ] [Set] touch tap event");
					break;
				}
				case volDWN: {
					ESP_LOGI(TAG_Touch, "[ <-- ] [Vol-] touch tap event");
					break;
				}
				case volUP: {
					ESP_LOGI(TAG_Touch, "[ <-- ] [Vol+] touch tap event");
					break;
				}
				default:
				break;
			}

		}
		vTaskDelay(40 / portTICK_RATE_MS);
	}
}
#endif

#if TOUCH_eg
//
//*触摸按键处理任务
//
void touch_test(void *arg) {
	esp_err_t ret;
	ESP_LOGE(TAG_Touch, "Touch_Test start!");
	while (1) {

		ret = audio_event_iface_listen(evt_touch, &msg_touch, portMAX_DELAY);
		if (ret != ESP_OK) {
			ESP_LOGE(TAG_Touch, "[ * ] Event interface error : %d", ret);
			continue;
		}

#if MYMUSIC
		if (msg_touch.source_type == AUDIO_ELEMENT_TYPE_ELEMENT) {
			// Set music info for a new song to be played
			if (msg_touch.source == (void *) mp3_decoder
					&& msg_touch.cmd == AEL_MSG_CMD_REPORT_MUSIC_INFO) {
				mp3_touch_flag = 1;

				continue;
				msg_touch.source = NULL;
			}

			// Advance to the next song when previous finishes
			if (msg_touch.source == (void *) i2s_stream_writer
					&& msg_touch.cmd == AEL_MSG_CMD_REPORT_STATUS) {
				i2s_touch_flag = 1;
				msg_touch.source = NULL;
				continue;
			}

		}
#endif
		if (msg_touch.source_type == PERIPH_ID_TOUCH
				&& msg_touch.cmd == PERIPH_TOUCH_TAP
				&& msg_touch.source == (void *) touch_periph) {
			KeyTCB.TouchState = TOUCH_control;
			if ((int) msg_touch.data == TOUCH_PLAY) {
////*
//				switch (KeyTCB.KeyState) {
//				case MP3_Mode:
//					KeyTCB.Sub_mode = KeyTCB1.Sub_mode;
//					break;
//				case RGB_Mode:
//
//					break;
//				case FAN_Mode:
//
//					break;
//				default:
//					break;
//				}
//
////*/

				KeyTCB.Incident = play;
				xQueueSend(xQueueKeyValue, &KeyTCB, 1000);
				ESP_LOGI(TAG_Touch, "[-->] [Play] touch tap event");
			} else if ((int) msg_touch.data == TOUCH_SET) {

//				if (KeyTCB.Sub_mode == exit_mode) {
//					if (KeyTCB.KeyState == MP3_Mode)
//						KeyTCB.KeyState = RGB_Mode;
//					if (KeyTCB.KeyState == RGB_Mode)
//						KeyTCB.KeyState = FAN_Mode;
//					if (KeyTCB.KeyState == FAN_Mode)
//						KeyTCB.KeyState = MP3_Mode;
//					KeyTCB.Sub_mode = first_mode;
//				}
//
//				switch (KeyTCB.KeyState) {
//
//				case MP3_Mode:
//					if (KeyTCB.Sub_mode == first_mode)
//						KeyTCB.KeyState = second_mode;
//					if (KeyTCB.Sub_mode == second_mode)
//						KeyTCB.KeyState = exit_mode;
////					KeyTCB.Sub_mode = KeyTCB1.Sub_mode;
//					break;
//				case RGB_Mode:
//					if (KeyTCB.Sub_mode == first_mode)
//						KeyTCB.KeyState = second_mode;
//					if (KeyTCB.Sub_mode == second_mode)
//						KeyTCB.KeyState = exit_mode;
//					break;
//				case FAN_Mode:
//					if (KeyTCB.Sub_mode == first_mode)
//						KeyTCB.KeyState = second_mode;
//					if (KeyTCB.Sub_mode == second_mode)
//						KeyTCB.KeyState = exit_mode;
//					break;
//				default:
//					break;
//
//				}

				KeyTCB.Incident = set;
				xQueueSend(xQueueKeyValue, &KeyTCB, 1000);
				ESP_LOGI(TAG_Touch, "[ --> ] [Set] touch tap event");
			} else if ((int) msg_touch.data == TOUCH_VOLUP) {
				KeyTCB.Incident = volUP;
				xQueueSend(xQueueKeyValue, &KeyTCB, 1000);
				ESP_LOGI(TAG_Touch, "[ --> ] [Vol+] touch tap event");
			} else if ((int) msg_touch.data == TOUCH_VOLDWN) {
				KeyTCB.Incident = volDWN;
				xQueueSend(xQueueKeyValue, &KeyTCB, 1000);
				ESP_LOGI(TAG_Touch, "[ --> ] [Vol-] touch tap event");
			}
			xEventGroupSetBits(touch_event_group, TOUCH_EVEAT);
			KeyTCB.Incident = KeyUnPress;
		}
		vTaskDelay(100 / portTICK_RATE_MS);
	}
}

///* USER CODE END PV */
//
///* Private function prototypes -----------------------------------------------*/
void touch_bsp_init(void) {
	esp_periph_config_t periph_cfg = {0};
	esp_periph_init(&periph_cfg);
	ESP_LOGI(TAG_Touch, "Touch peripheral Init Start!");

	periph_touch_cfg_t touch_cfg = {.touch_mask = TOUCH_SEL_SET
		| TOUCH_SEL_PLAY | TOUCH_SEL_VOLUP | TOUCH_SEL_VOLDWN,
		.tap_threshold_percent = 70,};
	touch_periph = periph_touch_init(&touch_cfg);

	esp_periph_start(touch_periph);

	/*
	 evt_cfg = AUDIO_EVENT_IFACE_DEFAULT_CFG();
	 */
	evt_cfg.internal_queue_size = DEFAULT_AUDIO_EVENT_IFACE_SIZE;
	evt_cfg.external_queue_size = DEFAULT_AUDIO_EVENT_IFACE_SIZE;
	evt_cfg.queue_set_size = DEFAULT_AUDIO_EVENT_IFACE_SIZE;
	evt_cfg.on_cmd = NULL;
	evt_cfg.context = NULL;
	evt_cfg.wait_time = portMAX_DELAY;
	evt_cfg.type = 0;
	evt_touch = audio_event_iface_init(&evt_cfg);

	audio_event_iface_set_listener(esp_periph_get_event_iface(), evt_touch);

#if MYMUSIC

#endif
	ESP_LOGI(TAG_Touch, "Touch peripheral Init OK!");

	xQueueKeyValue = xQueueCreate(2, sizeof(KeyTCB_Def));
	if (xQueueKeyValue != NULL)
	ESP_LOGI(TAG_Touch, "Touch xQueueKeyValue get OK!");
	vTaskDelay(1000 / portTICK_RATE_MS);

	touch_event_group = xEventGroupCreate();
	xEventGroupClearBits(touch_event_group, TOUCH_EVEAT);
	//创建按键扫描任务
	xTaskCreatePinnedToCore(&touch_test, "touch_test", 4096*2, NULL,MYTOUCH_PRIO, NULL, 1);
	//代码结束

#if TOUCH_RXT_TEST
	//创建按键接受扫描任务,用于测试
	xTaskCreatePinnedToCore(&touch_rxt_test, "touch_rxt_test", 4096, NULL, 8, NULL,0);
	//代码结束
#endif
}

#else

static esp_err_t _periph_event_handle(audio_event_iface_msg_t *event,
		void *context) {
	switch ((int) event->source_type) {
	case PERIPH_ID_BUTTON:
		ESP_LOGI(TAG_Touch, "BUTTON[%d], event->event_id=%d", (int) event->data,
				event->cmd);
		break;
	case PERIPH_ID_SDCARD:
		ESP_LOGI(TAG_Touch, "SDCARD status, event->event_id=%d", event->cmd);
		break;
	case PERIPH_ID_TOUCH:
//		ESP_LOGI(TAG_Touch, "TOUCH[%d], event->event_id=%d", (int) event->data,
//				event->cmd);
		KeyTCB.ControlState = TOUCH_control;
		switch ((int) event->data) {
		case TOUCH_PLAY:
			KeyTCB.Incident = play;
			if (event->cmd == press)
				KeyTCB.KeyState = press;
			if (event->cmd == loosen)
				KeyTCB.KeyState = loosen;
			if (event->cmd == longpress)
				KeyTCB.KeyState = longpress;
			if (event->cmd == putdown)
				KeyTCB.KeyState = putdown;
			xQueueSend(xQueueKeyValue, &KeyTCB, 1000);
			ESP_LOGI(TAG_Touch, "[-->] [Play] touch tap event,state:[%d]",
					event->cmd);

			break;
		case TOUCH_SET:
			KeyTCB.Incident = set;
			if (event->cmd == press)
				KeyTCB.KeyState = press;
			if (event->cmd == loosen)
				KeyTCB.KeyState = loosen;
			if (event->cmd == longpress)
				KeyTCB.KeyState = longpress;
			if (event->cmd == putdown)
				KeyTCB.KeyState = putdown;
			xQueueSend(xQueueKeyValue, &KeyTCB, 1000);
			ESP_LOGI(TAG_Touch, "[-->] [set] touch tap event,state:[%d]",
					event->cmd);
			break;
		case TOUCH_VOLUP:
			KeyTCB.Incident = volUP;
			if (event->cmd == press)
				KeyTCB.KeyState = press;
			if (event->cmd == loosen)
				KeyTCB.KeyState = loosen;
			if (event->cmd == longpress)
				KeyTCB.KeyState = longpress;
			if (event->cmd == putdown)
				KeyTCB.KeyState = putdown;
			xQueueSend(xQueueKeyValue, &KeyTCB, 1000);
			ESP_LOGI(TAG_Touch, "[-->] [vol+] touch tap event,state:[%d]",
					event->cmd);
			break;
		case TOUCH_VOLDWN:
			KeyTCB.Incident = volDWN;
			if (event->cmd == press)
				KeyTCB.KeyState = press;
			if (event->cmd == loosen)
				KeyTCB.KeyState = loosen;
			if (event->cmd == longpress)
				KeyTCB.KeyState = longpress;
			if (event->cmd == putdown)
				KeyTCB.KeyState = putdown;
			xQueueSend(xQueueKeyValue, &KeyTCB, 1000);
			ESP_LOGI(TAG_Touch, "[-->] [vol-] touch tap event,state:[%d]",
					event->cmd);
			break;
		}
		break;
	case PERIPH_ID_WIFI:
		ESP_LOGI(TAG_Touch, "WIFI, event->event_id=%d", event->cmd);
		break;
	}
	return ESP_OK;
}

///* USER CODE END PV */
//
///* Private function prototypes -----------------------------------------------*/
void touch_bsp_init(void) {

	esp_periph_config_t periph_cfg;
	// Initialize Peripherals pool
	periph_cfg.event_handle = _periph_event_handle;
	esp_periph_init(&periph_cfg);
	ESP_LOGI(TAG_Touch, "Touch peripheral Init Start!");

//	    esp_periph_set_register_callback(set, _periph_event_handle, NULL);
//	    // Setup SDCARD peripheral
//	    periph_sdcard_cfg_t sdcard_cfg = {
//	        .root = "/sdcard",
//	        .card_detect_pin = GPIO_NUM_34,
//	    };
//	    esp_periph_handle_t sdcard_handle = periph_sdcard_init(&sdcard_cfg);
//
	// Setup BUTTON peripheral
	periph_button_cfg_t btn_cfg = { .gpio_mask = GPIO_SEL_36 | GPIO_SEL_39 };
	esp_periph_handle_t button_handle = periph_button_init(&btn_cfg);

	// Setup TOUCH peripheral
	periph_touch_cfg_t touch_cfg = { .touch_mask = TOUCH_PAD_SEL4
			| TOUCH_PAD_SEL7 | TOUCH_PAD_SEL8 | TOUCH_PAD_SEL9,
			.tap_threshold_percent = 70,
			.long_tap_time_ms = 1000, };
	esp_periph_handle_t touch_handle = periph_touch_init(&touch_cfg);

	esp_periph_start(button_handle);
	esp_periph_start(touch_handle);

	xQueueKeyValue = xQueueCreate(3, sizeof(KeyTCB_Def));
	if (xQueueKeyValue != NULL)
		ESP_LOGI(TAG_Touch, "Touch xQueueKeyValue get OK!");
	vTaskDelay(1000 / portTICK_RATE_MS);


#if TOUCH_RXT_TEST
	//创建按键接受扫描任务,用于测试
	xTaskCreatePinnedToCore(&touch_rxt_test, "touch_rxt_test", 4096, NULL, 8, NULL,0);
	//代码结束
#endif
}

#endif
///* USER CODE BEGIN PFP */
//
///* USER CODE END PFP */
//
