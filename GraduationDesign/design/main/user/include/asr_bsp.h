/**
  ******************************************************************************
  * @file	: asr_bsp.h
  * @brief	: 
  * Details
  *
  * Created on			: 2019年2月19日
  * Author				: 123456
  *
  ******************************************************************************
  */
#ifndef MAIN_USER_INCLUDE_ASR_BSP_H_
#define MAIN_USER_INCLUDE_ASR_BSP_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"


typedef enum {
    WAKE_UP = 1,
    OPEN_THE_LIGHT,
    CLOSE_THE_LIGHT,
    VOLUME_INCREASE,
    VOLUME_DOWN,
    PLAY,
    PAUSE,
    MUTE,
    PLAY_LOCAL_MUSIC,
} asr_event_t;

extern EventGroupHandle_t asr_group;

void asr_test(void *arg) ;
void asr_bsp_init(void);

#endif /* MAIN_USER_INCLUDE_ASR_BSP_H_ */
