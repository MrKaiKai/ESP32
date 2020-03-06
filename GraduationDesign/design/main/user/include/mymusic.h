/**
  ******************************************************************************
  * @file	: mymusic.h
  * @brief	: 
  * Details
  *
  * Created on			: 2019年3月11日
  * Author				: 123456
  *
  ******************************************************************************
  */
#ifndef MAIN_USER_INCLUDE_MYMUSIC_H_
#define MAIN_USER_INCLUDE_MYMUSIC_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "audio_element.h"
#include "audio_pipeline.h"
#include "audio_event_iface.h"
#include "audio_common.h"
#include "fatfs_stream.h"
#include "i2s_stream.h"
#include "mp3_decoder.h"

#include "esp_log.h"
#include "nvs_flash.h"



#define MP3_FILE_COUNT sizeof(mp3_file)/sizeof(char*)

#define CURRENT 0
#define NEXT    1
#define PREVIOUS    2

#define MUSICDB_FN_LEN 128
#define MUSICDB_TITLE_LEN 128

#define MYMUSIC  0
#define music_new !0

#define MUSIC_4_11  !0
extern int playlist_len, nowplay_offset, list_offset;
typedef enum {
    NONE = 0, WAV, MP3, APE, FLAC
} musicType_t;

typedef unsigned char 	u8;
typedef unsigned short int	u16;
typedef unsigned  int	u32;

extern  audio_element_handle_t i2s_stream_writer, mp3_decoder;
extern int mp3_touch_flag ,i2s_touch_flag;
void music_test(void *arg);
void mymusic_init(void);
void mp3_get_num(void);
void mp3_get_tnum(void);
int scan_music_file(const char *basePath, int dep_cur, const int dep, FILE *db);
void parse_mp3_info(FILE *mp3File, char *title, char *author, char *album);
#endif /* MAIN_USER_INCLUDE_MYMUSIC_H_ */
