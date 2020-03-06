/**
  ******************************************************************************
  * @file	: mymusic.c
  * @brief	: 
  * Details
  *
  * Created on			: 2019年3月11日
  * Author				: liu
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include "function.h"
#include "mymusic.h"
#include "audio_element.h"
#include "audio_pipeline.h"
#include "audio_event_iface.h"
#include "audio_common.h"
#include "fatfs_stream.h"
#include "i2s_stream.h"
#include "mp3_decoder.h"
#include "filter_resample.h"

#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "ff.h"
#include "ffconf.h"

#include <dirent.h>
#include "esp_peripherals.h"
#include "periph_sdcard.h"
#include "periph_touch.h"
#include "audio_hal.h"
#include "board.h"
#include "touch_bsp.h"
#include "esp_heap_caps.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
const char* TAG_MYMUSIC = "TAG_MYmusic";

static audio_pipeline_handle_t pipeline;
audio_element_handle_t i2s_stream_writer, mp3_decoder,filter_touch;
audio_hal_handle_t hal;
audio_event_iface_handle_t evt_music;
audio_event_iface_msg_t msg;
int player_volume; //音量
int mp3_touch_flag = 0 ,i2s_touch_flag = 0;

static u16 curindex = 0;		//图片当前索引


int playlist_len, nowplay_offset, list_offset;

#define LIST_PATH (char*)"/sdcard/music/list/music_list.db"
/* USER CODE BEGIN PV */

static int check_music_file(char* filename) {
  char l[5];
  memset(l, 0, sizeof(l));
  int len = strlen(filename);
  for(int i = 0; i < 4; ++i) {
    l[i] = filename[len - (4 - i)];
  }
  if(strcmp(l,".mp3") == 0 || strcmp(l, ".MP3") == 0)
    return 1;
  else if(strcmp(l, ".wav") == 0 || strcmp(l, ".WAV") == 0) return 2;

  return 0;
}

void utf16_to_utf8(char *data, size_t length) {
  if(data[0] != 0xFF || data[1] != 0xFE) {
    ESP_LOGE(TAG_MYMUSIC, "Not utf16");
    return;
  }
  int len = length,oOffset = 0;
  uint16_t tmp[256];
  uint32_t unicode = 0;
  memset(tmp, 0, sizeof(tmp));
  for(int i = 2; i < len; i += 2) {
    tmp[(i - 2) / 2] = ((uint16_t)data[i + 1] << 8) | data[i];
  }
  len = (len - 2) / 2;
  memset(data, 0, sizeof(char) * length);
  for(int i = 0; i < len; ++i) {
    if(tmp[i] < 0xD800 || tmp[i] > 0xDFFF) unicode = tmp[i];
    else if(tmp[i] >= 0xD800 && tmp[i] <= 0xDBFF
            && i != len - 1 && tmp[i+1] >= 0xDC00
            && tmp[i+1] <= 0xDFFF) {
      unicode = ((((tmp[i] - 0xD800) & 0x3FF) << 10) | ((tmp[i+1] - 0xDC00) & 0x3FF)) + 0x10000;
      i++;
    } else {
      continue;
    }

    if(unicode <= 0x7F) {
      data[oOffset] = unicode & 0x7F;
      oOffset++;
    } else if(unicode <= 0x7FF) {
      data[oOffset] = ((unicode & 0x7C0) >> 6) | 0xC0;
      data[oOffset + 1] = (unicode & 0x3F) | 0x80;
      oOffset += 2;
    } else if(unicode <= 0xFFFF) {
      data[oOffset] = ((unicode & 0xF000) >> 12) | 0xE0;
      data[oOffset + 1] = ((unicode & 0xFC0) >> 6) | 0x80;
      data[oOffset + 2] = (unicode & 0x3F) | 0x80;
      oOffset += 3;
    } else {
      data[oOffset] = ((unicode & 0x1C0000) >> 18) | 0xF0;
      data[oOffset + 1] = ((unicode & 0x3F000) >> 12) | 0x80;
      data[oOffset + 2] = ((unicode & 0xFC0) >> 6) | 0x80;
      data[oOffset + 3] = (unicode & 0x3F) | 0x80;
      oOffset += 4;
    }

  }
}

void utf16be_to_utf8(char *data, size_t length) {
  if(data[0] != 0xFE || data[1] != 0xFF) {
    ESP_LOGE(TAG_MYMUSIC, "Not utf16be");
    return;
  }
  int len = length,oOffset = 0;
  uint16_t tmp[256];
  uint32_t unicode = 0;
  memset(tmp, 0, sizeof(tmp));
  for(int i = 2; i < len; i += 2) {
    tmp[(i - 2) / 2] = ((uint16_t)data[i] << 8) | data[i + 1];
  }
  len = (len - 2) / 2;
  memset(data, 0, sizeof(char) * length);
  for(int i = 0; i < len; ++i) {
    if(tmp[i] < 0xD800 || tmp[i] > 0xDFFF) unicode = tmp[i];
    else if(tmp[i] >= 0xD800 && tmp[i] <= 0xDBFF
            && i != len - 1 && tmp[i+1] >= 0xDC00
            && tmp[i+1] <= 0xDFFF) {
      unicode = ((((tmp[i] - 0xD800) & 0x3FF) << 10) | ((tmp[i+1] - 0xDC00) & 0x3FF)) + 0x10000;
      i++;
    } else {
      continue;
    }

    if(unicode <= 0x7F) {
      data[oOffset] = unicode & 0x7F;
      oOffset++;
    } else if(unicode <= 0x7FF) {
      data[oOffset] = ((unicode & 0x7C0) >> 6) | 0xC0;
      data[oOffset + 1] = (unicode & 0x3F) | 0x80;
      oOffset += 2;
    } else if(unicode <= 0xFFFF) {
      data[oOffset] = ((unicode & 0xF000) >> 12) | 0xE0;
      data[oOffset + 1] = ((unicode & 0xFC0) >> 6) | 0x80;
      data[oOffset + 2] = (unicode & 0x3F) | 0x80;
      oOffset += 3;
    } else {
      data[oOffset] = ((unicode & 0x1C0000) >> 18) | 0xF0;
      data[oOffset + 1] = ((unicode & 0x3F000) >> 12) | 0x80;
      data[oOffset + 2] = ((unicode & 0xFC0) >> 6) | 0x80;
      data[oOffset + 3] = (unicode & 0x3F) | 0x80;
      oOffset += 4;
    }

  }
}

void parse_mp3_info(FILE *mp3File, char *title, char *author, char *album) {
    if(mp3File == NULL) return;
    rewind(mp3File);
    int tag_len = 0;
    char tag[10];
    int read_bytes = fread(tag, 1, 10, mp3File);
    if(read_bytes == 10) {
      if (memcmp(tag,"ID3",3) == 0) {
        tag_len = ((tag[6] & 0x7F)<< 21)|((tag[7] & 0x7F) << 14) | ((tag[8] & 0x7F) << 7) | (tag[9] & 0x7F);
        while(ftell(mp3File) <= tag_len) {
          read_bytes = fread(tag, 1, 10, mp3File);
          int frame_len = 0;
          if(read_bytes == 10) {
            frame_len = (tag[4] << 24)|(tag[5] << 16)|(tag[6] << 8)|tag[7];
            char FrameID[5] = "", *FrameData = NULL;
            for(int i = 0; i < 4; ++i) FrameID[i] = tag[i];
            if(!strcmp(FrameID, "TIT2")) {
              FrameData = title;
            } else if(!strcmp(FrameID, "TPE1")) {
              FrameData = author;
            } else if(!strcmp(FrameID, "TALB")) {
              FrameData = album;
            } else {
              fseek(mp3File, frame_len, SEEK_CUR);
              continue;
            }
            if(FrameData == NULL) {
              fseek(mp3File, frame_len, SEEK_CUR);
              continue;
            }
            uint8_t code = 0x0;
            fread(&code, 1, 1, mp3File);
            fread(FrameData, 1, frame_len - 1, mp3File);
            switch(code) {
              case 0x00:break;//iso-8859-1
              case 0x01:utf16_to_utf8(FrameData, frame_len - 1);break;
              case 0x02:utf16be_to_utf8(FrameData, frame_len - 1);break;
              case 0x03:break;
              default:break;
            }
          }
        }
      }
      else {
        return;
      }
    }
}

int scan_music_file(const char *basePath, int dep_cur, const int dep, FILE *db) {
	if (dep == dep_cur) {
		ESP_LOGE(TAG_MYMUSIC, "Failed to opendir:0");
		return 0;
	}
	if (db == NULL) {
		ESP_LOGE(TAG_MYMUSIC, "Failed to opendir:-1");
		return -1;
	}
	DIR *dir_p;
	struct dirent *dirent_p;
	FILE *file_p = NULL;
	char path[256];
	int type = 0;
	memset(path, 0, sizeof(path));
	strcpy(path, basePath);
	char tmp_title[MUSICDB_TITLE_LEN], tmp_fn[MUSICDB_FN_LEN];

	dir_p = opendir(basePath);
	if (dir_p == NULL) {
		ESP_LOGE(TAG_MYMUSIC, "Failed to opendir: %s", basePath);
		return -1;
	}
	dirent_p = readdir(dir_p);
	while (dirent_p != NULL) {
		if (strcmp(dirent_p->d_name, ".") == 0
				|| strcmp(dirent_p->d_name, "..") == 0)
			continue;

		switch (dirent_p->d_type) {
		case 1:		//file
			strcat(path, dirent_p->d_name);
			type = check_music_file(path);
			if (type != 0) {
				playlist_len++;
				memset(tmp_title, 0, sizeof(tmp_title));
				memset(tmp_fn, 0, sizeof(tmp_fn));
				sprintf(tmp_fn, "%s/%s", basePath, dirent_p->d_name);
				file_p = fopen(tmp_fn, "rb");
				if (type == 1)
					parse_mp3_info(file_p, tmp_title, NULL, NULL);
				else if (type == 2)
					memcpy(tmp_title, tmp_fn, MUSICDB_TITLE_LEN);
				fclose(file_p);
				fwrite(tmp_fn, 1, MUSICDB_FN_LEN, db);
				fwrite(tmp_title, 1, MUSICDB_TITLE_LEN, db);
			}

			break;
		case 2:
			strcat(path, dirent_p->d_name);
			//strcat(path, "/");
			scan_music_file(path, dep_cur + 1, dep, db);
			break;
		default:
			break;
		}
		dirent_p = readdir(dir_p);
		memset(path, 0, sizeof(path));
		strcpy(path, basePath);
	}
	return 0;
}

void mp3_get_num(void) {
	FILE *db = fopen(LIST_PATH, "ar");
	scan_music_file("/sdcard/", 0, 2, db);
	fflush(db);
	playlist_len = ftell(db) / (MUSICDB_FN_LEN + MUSICDB_TITLE_LEN);
	fseek(db, 0, SEEK_SET);
	fclose(db);
	ESP_LOGI(TAG_MYMUSIC, "Music scanning completed.Playlist length: %d", playlist_len);
}
//得到path路径下,目标文件的总个数
//path:路径
//返回值:总有效文件数
void mp3_get_tnum(void)
{
    /*************目录文件查看********************/
	/*************目录文件查看********************/
	/*************目录文件查看********************/
//	DIR *dir = NULL;
//	int i =  0;
//	    struct dirent *entry = NULL;
//
//	    if ((dir = opendir("/sdcard/music")) == NULL) {
//	    	ESP_LOGI(TAG_MYMUSIC,"opendir failed!");
////	        return -1;
//	    } else {
//	        while ((entry = readdir(dir)) != NULL) {
//	        	ESP_LOGI(TAG_MYMUSIC,"filename%d = %s ,", i++, entry->d_name);  //输出文件或者目录的名称
//	        	ESP_LOGI(TAG_MYMUSIC,"filetype = %d\n", entry->d_type);   //输出文件类型
//	        }
//
//	        closedir(dir);
//	    }
	    /***********************************************/
	    /***********************************************/
	    /***********************************************/

	FILE *db = fopen(LIST_PATH, "r");

	  if(db == NULL )
	  {
	    fclose(db);
	    db = fopen(LIST_PATH, "ar");
	    scan_music_file("/sdcard/", 0, 2, db);
	    fflush(db);
	    playlist_len = ftell(db) / (MUSICDB_FN_LEN + MUSICDB_TITLE_LEN);
	    fseek(db, 0, SEEK_SET);
	    fclose(db);
	  }
	  else {
	    fseek(db, 0, SEEK_END);
	    playlist_len = ftell(db) / (MUSICDB_FN_LEN + MUSICDB_TITLE_LEN);
	    fseek(db, 0, SEEK_SET);
	    fclose(db);
	  }
	  ESP_LOGI(TAG_MYMUSIC, "Music scanning completed.Playlist length: %d", playlist_len);

}


static FILE *get_file(int next_file) {
	static FILE *file;

	if (next_file == NEXT) {
		// advance to the next file
		if (++curindex > playlist_len) {
			curindex = 0;
		}
		if (file != NULL) {
			fclose(file);
			file = NULL;
		}
		ESP_LOGI(TAG_MYMUSIC, "[ * ] File index %d", curindex);
	}
	else if (next_file == PREVIOUS) {
		// advance to the previous file
		if (curindex-- < 0) {
			curindex = playlist_len;
		}
		if (file != NULL) {
			fclose(file);
			file = NULL;
		}
		ESP_LOGI(TAG_MYMUSIC, "[ * ] File index %d", curindex);
	}
	// return a handle to the current file
	if (file == NULL) {

		char tmp_fn[MUSICDB_FN_LEN], tmp_title[MUSICDB_TITLE_LEN];
		FILE *db = fopen(LIST_PATH, "rb");
		fseek(db, curindex * (MUSICDB_FN_LEN + MUSICDB_TITLE_LEN), SEEK_SET);
		fread(tmp_fn, 1, MUSICDB_FN_LEN, db);
		fread(tmp_title, 1, MUSICDB_TITLE_LEN, db);
		fseek(db, 0, SEEK_SET);
		fclose(db);


		file = fopen(tmp_fn, "rb");


		if (!file) {
			ESP_LOGE(TAG_MYMUSIC, "Error opening file");

			return NULL;
		}
	}
	return file;
}

/*
 * Callback function to feed audio data stream from sdcard to mp3 decoder element
 */
static int my_sdcard_read_cb(audio_element_handle_t el, char *buf, int len, TickType_t wait_time, void *ctx)
{
    int read_len = fread(buf, 1, len, get_file(CURRENT));
    if (read_len == 0) {
        read_len = AEL_IO_DONE;
    }
    return read_len;
}
/**************************
 * @流监听任务
 *
 **************************/
void music_evt (void* pv){
	while(1){
#if !MYMUSIC
			int ret = audio_event_iface_listen(evt_music, &msg, portMAX_DELAY); //
			if (ret != ESP_OK) {
				ESP_LOGE(TAG_MYMUSIC, "[ * ] { * music_evt * }Event interface error : %d", ret);
				continue;
			}
			if (msg.source_type == AUDIO_ELEMENT_TYPE_ELEMENT) {
				// Set music info for a new song to be played
				if (msg.source == (void *) mp3_decoder
						&& msg.cmd == AEL_MSG_CMD_REPORT_MUSIC_INFO) {
					mp3_touch_flag = 1;

					continue;
					msg.source = NULL;
				}

				// Advance to the next song when previous finishes
				if (msg.source == (void *) i2s_stream_writer
						&& msg.cmd == AEL_MSG_CMD_REPORT_STATUS) {
					i2s_touch_flag = 1;
					msg.source = NULL;
					continue;
				}

			}
#endif
			// Set music info for a new song to be played
			if (mp3_touch_flag) {
				audio_element_info_t music_info = { 0 };
				audio_element_getinfo(mp3_decoder, &music_info);
				ESP_LOGI(TAG_MYMUSIC,
						"[ * ] { * music_evt * } Received music info from mp3 decoder, sample_rates=%d, bits=%d, ch=%d",
						music_info.sample_rates, music_info.bits,
						music_info.channels);
				audio_element_setinfo(i2s_stream_writer, &music_info);
				i2s_stream_set_clk(i2s_stream_writer, music_info.sample_rates,
						music_info.bits, music_info.channels);
				mp3_touch_flag = 0;
				continue;
			}

			// Advance to the next song when previous finishes
			if (i2s_touch_flag) {
				audio_element_state_t el_state = audio_element_get_state(
						i2s_stream_writer);
				if (el_state == AEL_STATE_FINISHED) {
					ESP_LOGI(TAG_MYMUSIC,
							"[ * ] { * music_evt * } Finished, advancing to the next song");
					audio_pipeline_stop(pipeline);
					audio_pipeline_wait_for_stop(pipeline);
					get_file(NEXT);
					audio_pipeline_run(pipeline);
				}
				i2s_touch_flag = 0;
				continue;
			}
	}
}
/*
 * @音量调节任务
 */
void mp3_vol(void *pv) {
	int* fl = (int*) pv;
	while (1) {
		if (*fl == mp3_volup_) {
			player_volume += 2;
			if (player_volume > 100) {
				player_volume = 100;
			}
			audio_hal_set_volume(hal, player_volume);
			ESP_LOGI(TAG_MYMUSIC, "[ * ] Volume set to %d %%", player_volume);
			vTaskDelay(100 / portTICK_RATE_MS);
		}
		else if (*fl == mp3_voldown_) {
			player_volume -= 2;
			if (player_volume < 0) {
				player_volume = 0;
			}
			audio_hal_set_volume(hal, player_volume);
			ESP_LOGI(TAG_MYMUSIC, "[ * ] Volume set to %d %%", player_volume);
			vTaskDelay(100 / portTICK_RATE_MS);
		} else
			vTaskDelete(NULL);
	}

}
/*
 * @音乐模式功能
 *
 */
void music_test(void *arg) {
	int vol = 0;
#if !MUSIC_4_11
	KeyTCB_Def IncidentValue_key;
#else
			SYS_Def sys_mp3;
#endif
	TaskHandle_t mp3_vol_= NULL;
	while (1) {
		ESP_LOGE(TAG_MYMUSIC, "music_test star!");
		while (1) {
			ESP_LOGE(TAG_MYMUSIC, "music_test run1!");
			/* Handle event interface messages from pipeline
			 to set music info and to advance to the next song
			 */

#if MUSIC_4_11

			if (xQueueReceive(sys_msg, &sys_mp3,
					portMAX_DELAY) == pdTRUE) {
				switch (sys_mp3.mymp3) {
				case mp3_play:
					ESP_LOGE(TAG_MYMUSIC, "[ * ] [Play] touch tap event")
					;
					audio_element_state_t el_state = audio_element_get_state(
							i2s_stream_writer);
					switch (el_state) {
					case AEL_STATE_INIT:
						ESP_LOGI(TAG_MYMUSIC, "[ * ] Starting audio pipeline");
						audio_pipeline_run(pipeline);
						break;
					case AEL_STATE_PAUSED:
						ESP_LOGI(TAG_MYMUSIC, "[ * ] Resuming audio pipeline");
						audio_pipeline_resume(pipeline);
						break;
					default:
						audio_hal_set_volume(hal, player_volume);

						break;
					}

					break;
				case mp3_pause:
					ESP_LOGI(TAG_MYMUSIC, "[ * ] Pausing audio pipeline");
					audio_pipeline_pause(pipeline);
					break;
				case mp3_resume:
					ESP_LOGI(TAG_MYMUSIC, "[ * ] Resuming audio pipeline");
					audio_pipeline_resume(pipeline);
					break;
				case mp3_mute:

					vol = player_volume;
					player_volume = 0;
					audio_hal_set_volume(hal, player_volume);
					player_volume = vol;
					break;
				case mp3_song_previous:
					audio_pipeline_terminate(pipeline);
					ESP_LOGI(TAG_MYMUSIC,
							"[ * ] Stopped, advancing to the previous song");
					get_file(PREVIOUS);
					audio_pipeline_run(pipeline);
					break;
				case mp3_song_next:
					audio_pipeline_terminate(pipeline);
					ESP_LOGI(TAG_MYMUSIC,
							"[ * ] Stopped, advancing to the next song");
					get_file(NEXT);
					audio_pipeline_run(pipeline);
					break;
				case mp3_volup:
					player_volume += 10;
					if (player_volume > 100) {
						player_volume = 100;
					}
					audio_hal_set_volume(hal, player_volume);
					ESP_LOGI(TAG_MYMUSIC, "[ * ] Volume set to %d %%",
							player_volume);
					break;
				case mp3_volup_:
					if (mp3_vol_ == NULL)
						xTaskCreatePinnedToCore(&mp3_vol, "mp3_vol", 1024,
								&sys_mp3.mymp3, 4, mp3_vol_, 1);
					break;
				case mp3_voldown:
					player_volume -= 10;
					if (player_volume < 0) {
						player_volume = 0;
					}
					audio_hal_set_volume(hal, player_volume);
					ESP_LOGI(TAG_MYMUSIC, "[ * ] Volume set to %d %%",
							player_volume);
					break;
				case mp3_voldown_:
					if (mp3_vol_ == NULL)
						xTaskCreatePinnedToCore(&mp3_vol, "mp3_vol", 1024,
								&sys_mp3.mymp3, 4, mp3_vol_, 1);
					break;
				case mp3_vol_set:
					player_volume = sys_mp3.vol;
					audio_hal_set_volume(hal, player_volume);
					ESP_LOGI(TAG_MYMUSIC, "[ * ] Volume set to %d %%",
							player_volume);
					break;
				case mp3_local_songs:
					mp3_get_num();

					audio_pipeline_run(pipeline);

					break;
				case mp3_null:
					if (mp3_vol_ != NULL)
						vTaskDelete(mp3_vol_);
					break;
				default:
					break;
				}


			}

#else
			if (xQueueReceive(xQueueKeyValue, &IncidentValue_key,
							portMAX_DELAY) == pdTRUE) {
				/* Handle touch pad events
				 to start, pause, resume, finish current song and adjust volume
				 */
				ESP_LOGE(TAG_MYMUSIC, "music_test run5!");
#if music_new
				switch (IncidentValue_key.ControlState) {
					case ASR_control:
					switch (IncidentValue_key.Incident) {
						case wAKE_UP:

						break;
						case oPEN_THE_LIGHT:

						break;
						case cLOSE_THE_LIGHT:

						break;
						case vOLUME_INCREASE:

						break;
						case vOLUME_DOWN:

						break;
						case pLAY:

						break;
						case pAUSE:

						break;
						case mUTE:

						break;
						case pLAY_LOCAL_MUSIC:

						break;
						default:
						break;
					}
					break;
					case TOUCH_control:
					if (IncidentValue_key.WorkMode == MP3_Mode) { //判断是否为MP3
						switch (IncidentValue_key.Incident) {
							case play:
							ESP_LOGE(TAG_MYMUSIC,
									"[ * ] [Play] touch tap event")
							;
							audio_element_state_t el_state =
							audio_element_get_state(i2s_stream_writer);
							switch (el_state) {
								case AEL_STATE_INIT:
								ESP_LOGI(TAG_MYMUSIC,
										"[ * ] Starting audio pipeline");
								audio_pipeline_run(pipeline);
								break;
								case AEL_STATE_RUNNING:
								ESP_LOGI(TAG_MYMUSIC,
										"[ * ] Pausing audio pipeline");
								audio_pipeline_pause(pipeline);
								break;
								case AEL_STATE_PAUSED:
								ESP_LOGI(TAG_MYMUSIC,
										"[ * ] Resuming audio pipeline");
								audio_pipeline_resume(pipeline);
								break;
								default:
								ESP_LOGI(TAG_MYMUSIC,
										"[ * ] Not supported state %d",
										el_state);
							}

							break;
							case set:

							switch (IncidentValue_key.KeyState) {
								case press:

								break;
								case loosen:

								break;
								case longpress:

								break;
								case putdown:

								break;
								default:
								break;
							}
							break;
							case volDWN:
							//
							ESP_LOGI(TAG_MYMUSIC,
									"[ * ] [Vol-] touch tap event");
							switch (IncidentValue_key.KeyState) {
								case press:
								player_volume -= 10;
								if (player_volume < 0) {
									player_volume = 0;
								}
								audio_hal_set_volume(hal, player_volume);
								ESP_LOGI(TAG_MYMUSIC,
										"[ * ] Volume set to %d %%",
										player_volume);
								break;
								case loosen:

								break;
								case longpress:

								break;
								case putdown:

								break;
								default:
								break;
							}
							//
							break;
							case volUP:
							//
							ESP_LOGI(TAG_MYMUSIC,
									"[ * ] [Vol+] touch tap event");
							switch (IncidentValue_key.KeyState) {
								case press:
								player_volume += 10;
								if (player_volume > 100) {
									player_volume = 100;
								}
								audio_hal_set_volume(hal, player_volume);
								ESP_LOGI(TAG_MYMUSIC,
										"[ * ] Volume set to %d %%",
										player_volume);
								break;
								case loosen:

								break;
								case longpress:

								break;
								case putdown:

								break;
								default:
								break;
							}
							//

							break;
							default:
							break;
						}
					}
					break;
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
					break;
					default:
					break;
				}

#else
				/******************************************************
				 *
				 *
				 * ****************************************************
				 */

				switch (IncidentValue_key.Incident) {
					case (pLAY): {   //play ||语音
						ESP_LOGI(TAG_MYMUSIC, "[ * ] [Play] touch tap event");
						if (audio_element_get_state(i2s_stream_writer)
								== AEL_STATE_PAUSED)
						audio_pipeline_resume(pipeline);
						else
						audio_pipeline_run(pipeline);
						break;
					}

					case (play): {   //play ||按键
						ESP_LOGE(TAG_MYMUSIC, "[ * ] [Play] touch tap event");
						audio_element_state_t el_state = audio_element_get_state(
								i2s_stream_writer);
						switch (el_state) {
							case AEL_STATE_INIT:
							ESP_LOGI(TAG_MYMUSIC, "[ * ] Starting audio pipeline");
							audio_pipeline_run(pipeline);
							break;
							case AEL_STATE_RUNNING:
							ESP_LOGI(TAG_MYMUSIC, "[ * ] Pausing audio pipeline");
							audio_pipeline_pause(pipeline);
							break;
							case AEL_STATE_PAUSED:
							ESP_LOGI(TAG_MYMUSIC, "[ * ] Resuming audio pipeline");
							audio_pipeline_resume(pipeline);
							break;
							default:
							ESP_LOGI(TAG_MYMUSIC, "[ * ] Not supported state %d",
									el_state);
						}
						break;
					}
					case set: {
						ESP_LOGE(TAG_MYMUSIC, "[ * ] [Set] touch tap event");
						audio_pipeline_terminate(pipeline);
						ESP_LOGI(TAG_MYMUSIC,
								"[ * ] Stopped, advancing to the next song");
						get_file(NEXT);
						audio_pipeline_run(pipeline);
						break;
					}
					case (vOLUME_INCREASE): {  //volUP ||
						ESP_LOGI(TAG_MYMUSIC, "[ * ] [Vol+] touch tap event");
						player_volume += 10;
						if (player_volume > 100) {
							player_volume = 100;
						}
						audio_hal_set_volume(hal, player_volume);
						ESP_LOGI(TAG_MYMUSIC, "[ * ] Volume set to %d %%",
								player_volume);
						break;
					}
					case (VOLUME_DOWN): {  //volDWN ||
						ESP_LOGI(TAG_MYMUSIC, "[ * ] [Vol-] touch tap event");
						player_volume -= 10;
						if (player_volume < 0) {
							player_volume = 0;
						}
						audio_hal_set_volume(hal, player_volume);
						ESP_LOGI(TAG_MYMUSIC, "[ * ] Volume set to %d %%",
								player_volume);
						break;
					}

					case (volUP): {  //volUP ||
						ESP_LOGI(TAG_MYMUSIC, "[ * ] [Vol+] touch tap event");
						player_volume += 10;
						if (player_volume > 100) {
							player_volume = 100;
						}
						audio_hal_set_volume(hal, player_volume);
						ESP_LOGI(TAG_MYMUSIC, "[ * ] Volume set to %d %%",
								player_volume);
						break;
					}
					case (volDWN): {  //volDWN ||
						ESP_LOGI(TAG_MYMUSIC, "[ * ] [Vol-] touch tap event");
						player_volume -= 10;
						if (player_volume < 0) {
							player_volume = 0;
						}
						audio_hal_set_volume(hal, player_volume);
						ESP_LOGI(TAG_MYMUSIC, "[ * ] Volume set to %d %%",
								player_volume);
						break;
					}
					case (pAUSE): {  //volDWN ||
						audio_pipeline_pause(pipeline);
						break;
					}
					default:
					break;
				}
				/******************************************************
				 *
				 *
				 * ****************************************************
				 */
#endif
#endif

			ESP_LOGE(TAG_MYMUSIC, "music_test run2!");
		}

	}
	ESP_LOGI(TAG_MYMUSIC, "[ 6 ] Stop audio_pipeline");
	audio_pipeline_terminate(pipeline);

	audio_pipeline_unregister(pipeline, mp3_decoder);

	audio_pipeline_unregister(pipeline, i2s_stream_writer);

	/* Terminate the pipeline before removing the listener */
	audio_pipeline_remove_listener(pipeline);

	/* Stop all peripherals before removing the listener */
	esp_periph_stop_all();
	audio_event_iface_remove_listener(esp_periph_get_event_iface(), evt_music);

	/* Make sure audio_pipeline_remove_listener & audio_event_iface_remove_listener are called before destroying event_iface */
	audio_event_iface_destroy(evt_music);

	/* Release all resources */
	audio_pipeline_deinit(pipeline);
	audio_element_deinit(i2s_stream_writer);
	audio_element_deinit(mp3_decoder);
	esp_periph_destroy();
}


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */
void mymusic_init(void)
{

//	    esp_log_level_set("*", ESP_LOG_WARN);
//	    esp_log_level_set(TAG_MYMUSIC, ESP_LOG_INFO);

	    ESP_LOGI(TAG_MYMUSIC, "[1.0] Initialize peripherals management");
	    esp_periph_config_t periph_cfg = { 0 };
	    esp_periph_init(&periph_cfg);

	    ESP_LOGI(TAG_MYMUSIC, "[1.1] Start SD card peripheral");
	    periph_sdcard_cfg_t sdcard_cfg = {
	        .root = "/sdcard",
	        .card_detect_pin = SD_CARD_INTR_GPIO, //GPIO_NUM_34
	    };
	    esp_periph_handle_t sdcard_handle = periph_sdcard_init(&sdcard_cfg);
	    ESP_LOGI(TAG_MYMUSIC, "[1.2] Start SD card peripheral");
	    esp_periph_start(sdcard_handle);

	    // Wait until sdcard was mounted
	    while (!periph_sdcard_is_mounted(sdcard_handle)) {
	        vTaskDelay(100 / portTICK_PERIOD_MS);
	        ESP_LOGI(TAG_MYMUSIC, "sdcard mounting...");
	    }


	    ESP_LOGI(TAG_MYMUSIC, "[ 2 ] Start codec chip");
	    audio_hal_codec_config_t audio_hal_codec_cfg =  AUDIO_HAL_ES8388_DEFAULT();
	    audio_hal_codec_cfg.i2s_iface.samples = AUDIO_HAL_44K_SAMPLES;
	    hal = audio_hal_init(&audio_hal_codec_cfg, 0);
	    audio_hal_ctrl_codec(hal, AUDIO_HAL_CODEC_MODE_DECODE, AUDIO_HAL_CTRL_START);

	    audio_hal_get_volume(hal, &player_volume);
	    player_volume = 35;
	    audio_hal_set_volume(hal, player_volume);

	    ESP_LOGI(TAG_MYMUSIC, "[3.0] Create audio pipeline for playback");
	    audio_pipeline_cfg_t pipeline_cfg = DEFAULT_AUDIO_PIPELINE_CONFIG();
	    pipeline = audio_pipeline_init(&pipeline_cfg);
	    mem_assert(pipeline);

	    ESP_LOGI(TAG_MYMUSIC, "[3.1] Create i2s stream to write data to codec chip");
	    i2s_stream_cfg_t i2s_cfg = I2S_STREAM_CFG_DEFAULT();
	    i2s_cfg.i2s_port = 0;
//	    i2s_cfg.task_prio = 15;
	    i2s_cfg.type = AUDIO_STREAM_WRITER;
	    i2s_stream_writer = i2s_stream_init(&i2s_cfg);


	    ESP_LOGI(TAG_MYMUSIC, "[3.2] Create mp3 decoder to decode mp3 file");
	    mp3_decoder_cfg_t mp3_cfg = DEFAULT_MP3_DECODER_CONFIG();
	    mp3_cfg.task_prio = 6;
	    mp3_cfg.task_core = (0);
	    mp3_cfg.out_rb_size = 10*1024;
	    mp3_cfg.task_stack = 10*1024;
	    mp3_decoder = mp3_decoder_init(&mp3_cfg);
	    audio_element_set_read_cb(mp3_decoder, my_sdcard_read_cb, NULL);

	    ESP_LOGI(TAG_MYMUSIC, "[3.3] Register all elements to audio pipeline");
	    audio_pipeline_register(pipeline, mp3_decoder, "mp3");
	    audio_pipeline_register(pipeline, i2s_stream_writer, "i2s");

	    ESP_LOGI(TAG_MYMUSIC, "[3.4] Link it together [my_sdcard_read_cb]-->mp3_decoder-->i2s_stream-->[codec_chip]");
	    audio_pipeline_link(pipeline, (const char *[]) {"mp3", "i2s"}, 2);

	    ESP_LOGI(TAG_MYMUSIC, "[4.0] Setup event listener");
	    audio_event_iface_cfg_t evt_cfg = AUDIO_EVENT_IFACE_DEFAULT_CFG();
	    evt_music = audio_event_iface_init(&evt_cfg);

	    ESP_LOGI(TAG_MYMUSIC, "[4.1] Listen for all pipeline events");
	    audio_pipeline_set_listener(pipeline, evt_music);

	    ESP_LOGI(TAG_MYMUSIC, "[4.2] Listening event from peripherals");
	    audio_event_iface_set_listener(esp_periph_get_event_iface(), evt_music);


	    ESP_LOGW(TAG_MYMUSIC, "[ 5 ] Tap touch buttons to control music player:");
	    ESP_LOGW(TAG_MYMUSIC, "      [Play] to start, pause and resume, [Set] next song.");
	    ESP_LOGW(TAG_MYMUSIC, "      [Vol-] or [Vol+] to adjust volume.");

	    mp3_get_tnum();

	    xTaskCreatePinnedToCore(&music_evt, "music_evt", 4096, NULL,5, NULL,0);

}
/* USER CODE END PFP */

