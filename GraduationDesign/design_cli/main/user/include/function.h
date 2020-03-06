/**
  ******************************************************************************
  * @file	: function.h
  * @brief	: 
  * Details
  *
  * Created on			: 2019年3月20日
  * Author				: 123456
  *
  ******************************************************************************
  */
#ifndef MAIN_USER_INCLUDE_FUNCTION_H_
#define MAIN_USER_INCLUDE_FUNCTION_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"



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

typedef enum
{
	NULL_Mode 	= 0x00,
	MP3_Mode 	 	= 0x01,
	RGB_Mode 	 	= 0x02,
	FAN_Mode,

}WorkMode_TypeDef;

typedef enum
{
	null   	=  0x00,
	press 	 	= 0x01,    //轻轻按下
	loosen 	 	= 0x02,     //松开
	longpress,            //长按
	putdown,             //放开

}KeyState_TypeDef;

typedef enum {
	keyUnPress = 0x00,
	wAKE_UP = 1,
	oPEN_THE_LIGHT,
	cLOSE_THE_LIGHT,
	vOLUME_INCREASE,
	vOLUME_DOWN,
	pLAY,
	pAUSE,
	mUTE,
	pLAY_LOCAL_MUSIC,
	play,
	set,
	volDWN,
	volUP,
}Incident_TypeDef;

typedef enum
{
	NULL_control 	 = 0x00,
	ASR_control 	 = 0x01,
	TOUCH_control 	 	 = 0x02,
	APP_control		 = 0x03,

}ControlState_TypeDef;

typedef enum
{
	first_mode 	 = 0x01,
	second_mode 	 	 = 0x02,
	exit_mode 	 = 0x05,

}SubMode_TypeDef;

typedef struct KeyTCB
{
	WorkMode_TypeDef            WorkMode;
	KeyState_TypeDef 	 		 	KeyState;
	ControlState_TypeDef 		ControlState;
	Incident_TypeDef	  			Incident;
	SubMode_TypeDef              Sub_mode;
}KeyTCB_Def;

extern SYS_Def sys;
extern KeyTCB_Def KeyTCB;
void led_color(int i);
void fan_test (fan_Def arg);
void RGB_test (RGB_Def arg);
void function_init(void);
void led_test (void *arg);
void char_struct(char* a);
#endif /* MAIN_USER_INCLUDE_FUNCTION_H_ */
