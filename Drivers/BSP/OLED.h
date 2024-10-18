#ifndef __OLED_H
#define __OLED_H

#define YES 1
#define NO  0

#include "sys.h"

void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

// 清空整行
void OLED_Line_clean(uint8_t Line);

// 显示中文汉字
// N: 字库对应字符索引
void OLED_ShowChinese_TimeWord(uint8_t Line, uint8_t Column, uint8_t N);

void OLED_ShowChineseTips(uint8_t Line, uint8_t Column, uint8_t N);

/// @brief 显示图片
/// @param islock   是否上锁（上锁:YES 显示锁图; 不锁:NO 显示解锁图）
void OLED_ShowImg(uint8_t Line, uint8_t Column, uint8_t islock);

#endif
