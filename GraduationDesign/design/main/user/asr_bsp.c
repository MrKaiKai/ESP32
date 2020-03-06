/**
  ******************************************************************************
  * @file	: asr_bsp.c
  * @brief	: 
  * Details
  *
  * Created on			: 2019年2月19日
  * Author				: 123456
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include "touch_bsp.h"
#include "function.h"
#include "asr_bsp.h"
#include "tcp_bsp.h"
#include "led_bsp.h"
#include <sys/socket.h>
#include "touch_bsp.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_log.h"
#include "audio_hal.h"
#include "board.h"
#include "zl38063.h"
#include "audio_common.h"
#include "audio_pipeline.h"
#include "mp3_decoder.h"
#include "i2s_stream.h"
#include "raw_stream.h"
#include "filter_resample.h"
#include "esp_sr_iface.h"
#include "esp_sr_models.h"
#include <sys/socket.h>
#include "esp_heap_caps.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
const char *EVENT_TAG = "asr_event";
const char *TAG_asr = "Asr_bsp";
int16_t *buff;
audio_pipeline_handle_t pipeline_asr;
audio_element_handle_t asr_i2s_stream_reader, filter, raw_read_asr;
int audio_chunksize;
const model_iface_data_t *iface;
const esp_sr_iface_t *model;
audio_hal_handle_t hal_asr;
EventGroupHandle_t asr_group;


/* USER CODE BEGIN PV */

void asr_test(void *arg)
{

	while (1)
	{
		while (1) {
			raw_stream_read(raw_read_asr, (char *) buff,
					audio_chunksize * sizeof(short));
			int keyword = model->detect(iface, (int16_t *) buff);

			switch (keyword) {

			case WAKE_UP:
				KeyTCB.Incident = wAKE_UP;
				KeyTCB.ControlState = ASR_control;
				xQueueSend(xQueueKeyValue, &KeyTCB, 1000);
				xEventGroupSetBits(asr_group, TOUCH_EVEAT);
				ESP_LOGI(TAG_asr, "Wake up");
				break;
			case OPEN_THE_LIGHT:
				KeyTCB.ControlState = ASR_control;
				KeyTCB.Incident = oPEN_THE_LIGHT;
				xQueueSend(xQueueKeyValue, &KeyTCB, 1000);
				xEventGroupSetBits(asr_group, TOUCH_EVEAT);
				ESP_LOGI(TAG_asr, "Turn on the light");
#if defined CONFIG_ESP_LYRAT_V4_3_BOARD
				gpio_set_level(GPIO_LED_GREEN, 1);
#endif
				break;
			case CLOSE_THE_LIGHT:
				KeyTCB.ControlState = ASR_control;
				KeyTCB.Incident = cLOSE_THE_LIGHT;
				xQueueSend(xQueueKeyValue, &KeyTCB, 1000);
				xEventGroupSetBits(asr_group, TOUCH_EVEAT);
				ESP_LOGI(TAG_asr, "Turn off the light");
#if defined CONFIG_ESP_LYRAT_V4_3_BOARD
				gpio_set_level(GPIO_LED_GREEN, 0);
#endif
				break;
			case VOLUME_INCREASE:
				KeyTCB.ControlState = ASR_control;
				KeyTCB.Incident = vOLUME_INCREASE;
				xQueueSend(xQueueKeyValue, &KeyTCB, 1000);
				xEventGroupSetBits(asr_group, TOUCH_EVEAT);
				ESP_LOGI(TAG_asr, "volume increase");
				break;
			case VOLUME_DOWN:
				KeyTCB.ControlState = ASR_control;
				KeyTCB.Incident = vOLUME_DOWN;
				xQueueSend(xQueueKeyValue, &KeyTCB, 1000);
				xEventGroupSetBits(asr_group, TOUCH_EVEAT);
				ESP_LOGI(TAG_asr, "volume down");
				break;
			case PLAY:
				KeyTCB.ControlState = ASR_control;
				KeyTCB.Incident = pLAY;
				xQueueSend(xQueueKeyValue, &KeyTCB, 1000);
				xEventGroupSetBits(asr_group, TOUCH_EVEAT);
				ESP_LOGI(TAG_asr, "play");
				break;
			case PAUSE:
				KeyTCB.ControlState = ASR_control;
				KeyTCB.Incident = pAUSE;
				xQueueSend(xQueueKeyValue, &KeyTCB, 1000);
				xEventGroupSetBits(asr_group, TOUCH_EVEAT);
				ESP_LOGI(TAG_asr, "pause");
				break;
			case MUTE:
				KeyTCB.Incident = mUTE;
				KeyTCB.ControlState = ASR_control;
				xQueueSend(xQueueKeyValue, &KeyTCB, 1000);
				xEventGroupSetBits(asr_group, TOUCH_EVEAT);
				ESP_LOGI(TAG_asr, "mute");
				break;
			case PLAY_LOCAL_MUSIC:
				KeyTCB.Incident = pLAY_LOCAL_MUSIC;
				KeyTCB.ControlState = ASR_control;
				xQueueSend(xQueueKeyValue, &KeyTCB, 1000);
				xEventGroupSetBits(asr_group, TOUCH_EVEAT);
				ESP_LOGI(TAG_asr, "play local music");
				break;
			default:
//				KeyTCB.ControlState = NULL_control;
				ESP_LOGD(TAG_asr, "Not supported keyword")
				;
				break;
			}
		}

		ESP_LOGI(EVENT_TAG, "[ 6 ] Stop audio_pipeline");

		audio_pipeline_terminate(pipeline_asr);

		/* Terminate the pipeline before removing the listener */
		audio_pipeline_remove_listener(pipeline_asr);

		audio_pipeline_unregister(pipeline_asr, raw_read_asr);
		audio_pipeline_unregister(pipeline_asr, asr_i2s_stream_reader);
		audio_pipeline_unregister(pipeline_asr, filter);

		/* Release all resources */
		audio_pipeline_deinit(pipeline_asr);
		audio_element_deinit(raw_read_asr);
		audio_element_deinit(asr_i2s_stream_reader);
		audio_element_deinit(filter);

		ESP_LOGI(EVENT_TAG, "[ 7 ] Destroy model");
		model->destroy(iface);
		model = NULL;
		free(buff);
		buff = NULL;

	}
}
/*
static esp_err_t asr_login_timer_cb (void ){

	audio_pipeline_terminate (pipeline_asr);

	vTaskDelay(100);

	 audio_pipeline_resume(pipeline_asr);

	return ESP_OK;
}
*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */
void asr_bsp_init(void)
{
//	esp_log_level_set("*", ESP_LOG_WARN);
//		esp_log_level_set(TAG_asr, ESP_LOG_INFO);
//		esp_log_level_set(EVENT_TAG, ESP_LOG_INFO);

		ESP_LOGI(TAG_asr, "Initialize SR handle");
	#if CONFIG_SR_MODEL_WN4_QUANT
		model = &esp_sr_wakenet4_quantized;
	#else
		model = &esp_sr_wakenet3_quantized;
	#endif
		iface = model->create(DET_MODE_90);
		int num = model->get_word_num(iface);
		for (int i = 1; i <= num; i++) {
			char *name = model->get_word_name(iface, i);
			ESP_LOGI(TAG_asr, "keywords: %s (index = %d)", name, i);
		}
		float threshold = model->get_det_threshold_by_mode(iface, DET_MODE_90, 1);
		int sample_rate = model->get_samp_rate(iface);
		audio_chunksize = model->get_samp_chunksize(iface);
		ESP_LOGI(EVENT_TAG,
				"keywords_num = %d, threshold = %f, sample_rate = %d, chunksize = %d, sizeof_uint16 = %d",
				num, threshold, sample_rate, audio_chunksize, sizeof(int16_t));
		buff = (int16_t *) heap_caps_malloc(audio_chunksize * sizeof(short), MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
		if (NULL == buff) {
			ESP_LOGE(EVENT_TAG, "Memory allocation failed!");
			model->destroy(iface);
			model = NULL;
			return;
		}

		ESP_LOGI(EVENT_TAG, "[ 1 ] Start codec chip");
	#if (CONFIG_ESP_LYRAT_V4_3_BOARD || CONFIG_ESP_LYRAT_V4_2_BOARD)
		audio_hal_codec_config_t audio_hal_codec_cfg_asr = AUDIO_HAL_ES8388_DEFAULT();
		hal_asr = audio_hal_init(&audio_hal_codec_cfg_asr, 0);
	#endif

	#if (CONFIG_ESP_LYRATD_MSC_V2_1_BOARD || CONFIG_ESP_LYRATD_MSC_V2_2_BOARD)
		audio_hal_codec_config_t audio_hal_codec_cfg_asr = AUDIO_HAL_ZL38063_DEFAULT();
		hal_asr = audio_hal_init(&audio_hal_codec_cfg_asr, 2);
	#endif
		audio_hal_ctrl_codec(hal_asr, AUDIO_HAL_CODEC_MODE_BOTH, AUDIO_HAL_CTRL_START);

		ESP_LOGI(EVENT_TAG, "[ 2.0 ] Create audio pipeline for recording");
		audio_pipeline_cfg_t pipeline_cfg_asr = DEFAULT_AUDIO_PIPELINE_CONFIG();
		pipeline_asr = audio_pipeline_init(&pipeline_cfg_asr);
		mem_assert(pipeline_asr);

		ESP_LOGI(EVENT_TAG,
				"[ 2.1 ] Create i2s stream to read audio data from codec chip");
		i2s_stream_cfg_t i2s_cfg_asr = I2S_STREAM_CFG_DEFAULT();
		i2s_cfg_asr.i2s_config.sample_rate = 48000;
		i2s_cfg_asr.type = AUDIO_STREAM_READER;
		i2s_cfg_asr.i2s_port = 0;
		asr_i2s_stream_reader = i2s_stream_init(&i2s_cfg_asr);

		ESP_LOGI(EVENT_TAG, "[ 2.2 ] Create filter to resample audio data");
		rsp_filter_cfg_t rsp_cfg_asr = DEFAULT_RESAMPLE_FILTER_CONFIG();
		rsp_cfg_asr.src_rate = 48000;
		rsp_cfg_asr.src_ch = 2;
		rsp_cfg_asr.dest_rate = 16000;
		rsp_cfg_asr.dest_ch = 1;
		rsp_cfg_asr.type = AUDIO_CODEC_TYPE_ENCODER;
		filter = rsp_filter_init(&rsp_cfg_asr);

		ESP_LOGI(EVENT_TAG, "[ 2.3 ] Create raw to receive data");
		raw_stream_cfg_t raw_cfg_asr = { .out_rb_size = 8 * 1024, .type =
				AUDIO_STREAM_READER, };
		raw_read_asr = raw_stream_init(&raw_cfg_asr);

		ESP_LOGI(EVENT_TAG, "[ 3 ] Register all elements to audio pipeline");
		audio_pipeline_register(pipeline_asr, asr_i2s_stream_reader, "asr_i2s");
		audio_pipeline_register(pipeline_asr, filter, "filter");
		audio_pipeline_register(pipeline_asr, raw_read_asr, "raw");

		ESP_LOGI(EVENT_TAG,
				"[ 4 ] Link elements together [codec_chip]-->i2s_stream-->filter-->raw-->[SR]");
		audio_pipeline_link(pipeline_asr, (const char *[] ) { "asr_i2s", "filter", "raw" },
				3);

//		xTimerHandle asr_login_timer = xTimerCreate("asr_login", 200 / portTICK_PERIOD_MS,
//		                                     pdTRUE, 1, asr_login_timer_cb);

//		if(xTimerStart(asr_login_timer, 100) != pdPASS)
//			ESP_LOGI(EVENT_TAG, "asr_login_timer set flad!!!");


		ESP_LOGI(EVENT_TAG, "[ 5 ] Start audio_pipeline");
		audio_pipeline_run(pipeline_asr);

		asr_group = xEventGroupCreate();
		//创建语音识别任务

		//创建结束
}
/* USER CODE END PFP */

