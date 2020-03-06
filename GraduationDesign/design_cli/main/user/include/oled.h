/**
  ******************************************************************************
  * @file	: oled.h
  * @brief	: 
  * Details
  *
  * Created on			: 2019年3月6日
  * Author				: 123456
  *
  ******************************************************************************
  */
#ifndef MAIN_USER_INCLUDE_OLED_H_
#define MAIN_USER_INCLUDE_OLED_H_

#include "myi2c.h"
#include "stdlib.h"
#include "esp_log.h"
#include "esp_system.h"

typedef  uint8_t u8;
typedef  uint16_t u16;
typedef  uint32_t u32;

#define OLED_ADS           		  	0x78
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF
#define X_WIDTH 	128
#define Y_WIDTH 	64

void OLED_WR_Byte (uint8_t data,uint8_t cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowNum_F(u8 x,u8 y,u32 num,u8 len,u8 size2);
void OLED_ShowFloatNum (u8 x,u8 y,float num,u8 IntLen,u8 FloatLen,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p,u8 Char_Size);
void OLED_Set_Pos(unsigned char x, unsigned char y);
u8 OLED_ShowCH(u8 x,u8 y,char *cn);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);

#endif /* MAIN_USER_OLED_INCLUDE_OLED_H_ */
