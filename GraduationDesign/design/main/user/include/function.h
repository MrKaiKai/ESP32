/**
  ******************************************************************************
  * @file	: function.h
  * @brief	: 
  * Details
  *
  * Created on			: 2019年4月4日
  * Author				: 123456
  *
  ******************************************************************************
  */
#ifndef MAIN_USER_INCLUDE_FUNCTION_H_
#define MAIN_USER_INCLUDE_FUNCTION_H_

#include "mymusic.h"
#include "audio_element.h"
#include "audio_pipeline.h"
#include "audio_event_iface.h"
#include "audio_common.h"
#include "fatfs_stream.h"
#include "i2s_stream.h"
#include "mp3_decoder.h"
#include "asr_bsp.h"
#include "tcp_bsp.h"
#include "led_bsp.h"
#include "touch_bsp.h"

#define DATA_PRIO      11
#define MUSIC_PRIO      9
#define ASR_PRIO        8
#define MYTOUCH_PRIO    10
#define TCP_CON_PRIO    7
#define TCP_RXD_PRIO    11
#define TCP_RXD1_PRIO    11
#define TCP_TXD_PRIO    11
#define function_BIT    BIT2
#define APP_BIT    BIT3
#define SEND_TIME portMAX_DELAY

typedef enum
{
	mp3_null = 0,
	mp3_play,
	mp3_pause,
	mp3_resume,
	mp3_song_previous,
	mp3_song_next,
	mp3_mute,
	mp3_volup,
	mp3_voldown,
	mp3_volup_,
	mp3_voldown_,
	mp3_local_songs,
	mp3_vol_set,
}mp3_Def;

typedef enum
{
	RGB_null = 0,
	RGB_open,
	RGB_close,
	RGB_red,
	RGB_blue,
	RGB_green,
	RGB_yellow,
	RGB_pink,
	RGB_cyan,
	RGB_auto,
}RGB_Def;

typedef enum
{
	fan_null = 0,
	fan_open,
	fan_close,
	fan_up,
	fan_down,
	fan_set,

}fan_Def;

typedef struct SYS
{
	mp3_Def    mymp3;
	RGB_Def 	myRGB;
	fan_Def    myfan;
	int vol;      //音量
	int pow;      //风扇转速

}SYS_Def;
TaskHandle_t listen_Hand;
EventGroupHandle_t function_group;
extern SYS_Def sys;
extern QueueHandle_t sys_msg;
void function_Init(void);

#endif /* MAIN_USER_INCLUDE_FUNCTION_H_ */
