/**
  ******************************************************************************
  * @file	: touch_bsp.h
  * @brief	: 
  * Details
  *
  * Created on			: 2019年2月18日
  * Author				: 123456
  *
  ******************************************************************************
  */
#ifndef COMPONENTS_TOUCH_BSP_INCLUDE_TOUCH_BSP_H_
#define COMPONENTS_TOUCH_BSP_INCLUDE_TOUCH_BSP_H_

#include "esp_peripherals.h"
#include "periph_sdcard.h"
#include "periph_touch.h"
#include "audio_hal.h"
#include "board.h"
#include <string.h>



#include "esp_err.h"
//#include "esp_log.h"
#include "nvs_flash.h"

#include "esp_err.h"

#define TOUCH_eg 0
#define TOUCH_RXT_TEST 0
#define TOUCH_EVEAT     BIT1

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
	KeyUnPress = 0x00,
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

extern KeyTCB_Def KeyTCB;
extern audio_event_iface_handle_t evt_touch;
extern audio_event_iface_msg_t msg;
extern EventGroupHandle_t touch_event_group;
extern QueueHandle_t xQueueKeyValue;
void touch_test(void *arg);
void touch_bsp_init (void);

#endif /* COMPONENTS_TOUCH_BSP_INCLUDE_TOUCH_BSP_H_ */
