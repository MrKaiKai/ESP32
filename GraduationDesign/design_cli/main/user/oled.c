/**
 ******************************************************************************
 * @file	: oled.c
 * @brief	:
 * Details
 *
 * Created on			: 2019年3月6日
 * Author				: 123456
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include "oled.h"

#include "oledfont.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
const char* TAG_oled = "TAG_OLED";

/* USER CODE BEGIN PV */
void OLED_WR_Byte(uint8_t data, uint8_t cmd) {
	if (cmd)
		IIC_write_data(data, OLED_ADS);
	else
		IIC_write_cmd(data, OLED_ADS);
}

/********************************************
 // fill_Picture
 ********************************************/
void fill_picture(unsigned char fill_Data) {
	unsigned char m, n;
	for (m = 0; m < 8; m++) {
		OLED_WR_Byte(0xb0 + m, 0);		//page0-page1
		OLED_WR_Byte(0x00, 0);		//low column start address
		OLED_WR_Byte(0x10, 0);		//high column start address
		for (n = 0; n < 128; n++) {
			OLED_WR_Byte(fill_Data, 1);
		}
	}
}

//坐标设置

void OLED_Set_Pos(unsigned char x, unsigned char y) {
	OLED_WR_Byte(0xb0 + y, OLED_CMD);
	OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
	OLED_WR_Byte((x & 0x0f), OLED_CMD);
}
//开启OLED显示
void OLED_Display_On(void) {
	OLED_WR_Byte(0X8D, OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14, OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF, OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示
void OLED_Display_Off(void) {
	OLED_WR_Byte(0X8D, OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10, OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE, OLED_CMD);  //DISPLAY OFF
}
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void OLED_Clear(void) {
	u8 i, n;
	for (i = 0; i < 8; i++) {
		OLED_WR_Byte(0xb0 + i, OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte(0x00, OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte(0x10, OLED_CMD);      //设置显示位置—列高地址
		for (n = 0; n < 128; n++)
			OLED_WR_Byte(0, OLED_DATA);
	} //更新显示
}
void OLED_On(void) {
	u8 i, n;
	for (i = 0; i < 8; i++) {
		OLED_WR_Byte(0xb0 + i, OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte(0x00, OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte(0x10, OLED_CMD);      //设置显示位置—列高地址
		for (n = 0; n < 128; n++)
			OLED_WR_Byte(1, OLED_DATA);
	} //更新显示
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示
//size:选择字体 16/12
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 size) {
	unsigned char c = 0, i = 0, Nom;
	c = chr - ' '; //得到偏移后的值
	if (x > Max_Column - 1) {
		x = 0;
		y = y + 2;
	}
	if (size == 16) {
		for (Nom = 0; Nom < 95; Nom++) {
			if (F8X16[Nom].Index1[0] == chr) {
				OLED_Set_Pos(x, y);
				for (i = 0; i < 8; i++)
					OLED_WR_Byte(F8X16[Nom].Msk1[i], OLED_DATA);
				OLED_Set_Pos(x, y + 1);
				for (i = 0; i < 8; i++)
					OLED_WR_Byte(F8X16[Nom].Msk1[i + 8], OLED_DATA);
			}
		}
	} else {
		OLED_Set_Pos(x, y + 1);
		for (i = 0; i < 6; i++)
			OLED_WR_Byte(F6x8[c][i], OLED_DATA);

	}
}
//m^n函数
u32 oled_pow(u8 m, u8 n) {
	u32 result = 1;
	while (n--)
		result *= m;
	return result;
}
//显示2个数字
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size2) {
	u8 t, temp;
	u8 enshow = 0;
	for (t = 0; t < len; t++) {
		temp = (num / oled_pow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1)) {
			if (temp == 0) {
				OLED_ShowChar(x + (size2 / 2) * t, y, ' ', size2);
				continue;
			} else
				enshow = 1;

		}
		OLED_ShowChar(x + (size2 / 2) * t, y, temp + '0', size2);
	}
}
/******************************************************
 ****函数名：
 ****功  能：本函数不在主函数中调用
 ****输入值：
 ****返回值：无
 ****日  期：
 ******************************************************/

void OLED_ShowNum_F(u8 x, u8 y, u32 num, u8 len, u8 size2) {
	u8 t, temp;
	u8 enshow = 0;
	for (t = 0; t < len; t++) {
		temp = (num / oled_pow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1)) {
			if (temp == 0) {
				OLED_ShowChar(x + (size2 / 2) * t, y, '0', size2);
				continue;
			} else
				enshow = 1;

		}
		OLED_ShowChar(x + (size2 / 2) * t, y, temp + '0', size2);
	}
}
/******************************************************
 ****函数名：
 ****功  能：显示小数
 ****输入值：IntLen整数长度,FloatLen小数长度
 ****返回值：无
 ****日  期：2017-03-04
 ******************************************************/
void OLED_ShowFloatNum(u8 x, u8 y, float num, u8 IntLen, u8 FloatLen, u8 size) {
	float Num = num - (float) ((u16) num);
	if (FloatLen == 0) {
		OLED_ShowNum(x, y, (u16) num, IntLen, size);
	}
	else {
		u16 u = (u16) ((float) Num * (float) oled_pow(10, FloatLen))
				% oled_pow(10, FloatLen);
		OLED_ShowNum(x, y, (u16) num, IntLen, size);
		OLED_ShowChar(x + IntLen * (size / 2), y, '.', size);
		OLED_ShowNum_F(x + (IntLen + 1) * (size / 2), y, u, FloatLen, size);
	}
}
//显示一个字符号串
void OLED_ShowString(u8 x, u8 y, u8 *chr, u8 Char_Size) {
	unsigned char j = 0;
	while (chr[j] != '\0') {
		OLED_ShowChar(x, y, chr[j], Char_Size);
		x += 8;
		if (x > 120) {
			x = 0;
			y += 2;
		}
		j++;
	}
}

u8 OLED_ShowCH(u8 x, u8 y, char *cn) {
	u8 t, adder = 0, wordNum,flag = 0;
	if (y > 64)
		return 0;
	if (x > 128)
		return 0;

	while (*cn)	 //在C语言中字符串结束以‘\0’结尾
	{
		for (wordNum = 0;wordNum<95; wordNum++) {
			//--查询要写的字在字库中的位置--//
			if ((CN16CHAR[wordNum].Index[0] == *cn)
					&& (CN16CHAR[wordNum].Index[1] == *(cn + 1))) {
				OLED_Set_Pos(x, y);
				for (t = 0; t < 16; t++) {
					OLED_WR_Byte(CN16CHAR[wordNum].Msk[t], OLED_DATA);
					adder += 1;
				}
				OLED_Set_Pos(x, y + 1);
				for (t = 0; t < 16; t++) {
					OLED_WR_Byte(CN16CHAR[wordNum].Msk[16 + t], OLED_DATA);
					adder += 1;
				}
				x += 16;
//				flag = 1;
			}	 //if查到字结束
			else if ((F8X16[wordNum].Index1[0] == *cn)) {
				OLED_Set_Pos(x, y);
				for (t = 0; t < 8; t++)
					OLED_WR_Byte(F8X16[wordNum].Msk1[t], OLED_DATA);
				OLED_Set_Pos(x, y + 1);
				for (t = 0; t < 8; t++)
					OLED_WR_Byte(F8X16[wordNum].Msk1[t + 8], OLED_DATA);
				x += 8;
				flag = 0;
			}
			if (x > 120) {
				x = 0;
				y += 2;
			}
		} //for查字结束
		cn += (flag + 1);
//		if ((*cn & 0x80) && (*(cn + 1) & 0x80)) cn += 2;
//		else cn += 1;
	}	//while结束
	return 1;
}

//
//u8 OLED_ShowCH(u8 x, u8 y, char *cn) {
//	int t, adder = 0, wordNum = 0,flag = 0;
//	if (y > 64)
//		return 0;
//	if (x > 128)
//		return 0;
//
//	while (*cn)	 //在C语言中字符串结束以‘\0’结尾
//	{
////		(*cn & 0x80) && (*(cn + 1) & 0x80)
//		if ((u8)(*cn) >= 0x80) {
//			flag = 1;
////			for (wordNum = 0; CN16CHAR[wordNum].Index != NULL; wordNum++)
//			while(CN16CHAR[wordNum].Index != NULL){
//				if ((*cn == CN16CHAR[wordNum].Index[0])
//						&& (*(cn + 1) == CN16CHAR[wordNum].Index[1])) {
//					flag = 1;
//					OLED_Set_Pos(x, y);
//					for (t = 0; t < 16; t++) {
//						OLED_WR_Byte(CN16CHAR[wordNum].Msk[t], OLED_DATA);
//						adder += 1;
//					}
//					OLED_Set_Pos(x, y + 1);
//					for (t = 0; t < 16; t++) {
//						OLED_WR_Byte(CN16CHAR[wordNum].Msk[16 + t], OLED_DATA);
//						adder += 1;
//					}
//					x += 16;
////					break;
//				}	 //if查到字结束
//				wordNum++;
//			}
//		} else
//		{
//			flag = 0;
//			for (wordNum = 0; wordNum < 95; wordNum++) {
//				if ((F8X16[wordNum].Index1[0] == *cn)) {
//					OLED_Set_Pos(x, y);
//					for (t = 0; t < 8; t++)
//						OLED_WR_Byte(F8X16[wordNum].Msk1[t], OLED_DATA);
//					OLED_Set_Pos(x, y + 1);
//					for (t = 0; t < 8; t++)
//						OLED_WR_Byte(F8X16[wordNum].Msk1[t + 8], OLED_DATA);
//					x += 8;
//					break;
//				}
//			}
//		}
//	cn += (flag + 1);
//	}	//while结束
//	return 1;
//}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1,
		unsigned char y1, unsigned char BMP[]) {
	unsigned int j = 0;
	unsigned char x, y;

	if (y1 % 8 == 0)
		y = y1 / 8;
	else
		y = y1 / 8 + 1;
	for (y = y0; y < y1; y++) {
		OLED_Set_Pos(x0, y);
		for (x = x0; x < x1; x++) {
			OLED_WR_Byte(BMP[j++], OLED_DATA);
		}
	}
}

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */
void OLED_Init(void) {
	OLED_WR_Byte(0xAE, OLED_CMD);	//--display off
	OLED_WR_Byte(0x20, OLED_CMD);	//---set low column address //00
	OLED_WR_Byte(0x10, OLED_CMD);	//---set high column address
	OLED_WR_Byte(0x40, OLED_CMD);	//--set start line address
	OLED_WR_Byte(0xB0, OLED_CMD);	//--set page address
	OLED_WR_Byte(0x81, OLED_CMD); // contract control
	OLED_WR_Byte(0xFF, OLED_CMD); //--128
	OLED_WR_Byte(0xA1, OLED_CMD); //set segment remap
	OLED_WR_Byte(0xA6, OLED_CMD); //--normal / reverse
	OLED_WR_Byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F, OLED_CMD); //--1/32 duty
	OLED_WR_Byte(0xC8, OLED_CMD); //Com scan direction
	OLED_WR_Byte(0xD3, OLED_CMD); //-set display offset
	OLED_WR_Byte(0x00, OLED_CMD); //

	OLED_WR_Byte(0xD5, OLED_CMD); //set osc division
	OLED_WR_Byte(0x80, OLED_CMD); //

	OLED_WR_Byte(0xD8, OLED_CMD); //set area color mode off
	OLED_WR_Byte(0x05, OLED_CMD); //

	OLED_WR_Byte(0xD9, OLED_CMD); //Set Pre-Charge Period
	OLED_WR_Byte(0xF1, OLED_CMD); //

	OLED_WR_Byte(0xDA, OLED_CMD); //set com pin configuartion
	OLED_WR_Byte(0x12, OLED_CMD); //

	OLED_WR_Byte(0xDB, OLED_CMD); //set Vcomh
	OLED_WR_Byte(0x30, OLED_CMD); //

	OLED_WR_Byte(0x8D, OLED_CMD); //set charge pump enable
	OLED_WR_Byte(0x14, OLED_CMD); //

	OLED_WR_Byte(0xAF, OLED_CMD); //--turn on oled panel

	ESP_LOGI(TAG_oled, "OLED_init OK!!!");
}
/* USER CODE END PFP */

