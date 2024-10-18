#ifndef __MYRTC_H
#define __MYRTC_H

#include "sys.h"
#include <time.h>

enum tim_index{
	YEAR=0,
	MONTH,
	MDAY ,
	HOUR ,
	MINUTE ,
    SECONDS,
};

void myRTC_Init(void);
void myRTC_setTime(uint8_t* time_data);
void myRTC_getTime(uint8_t* time_data);

/// @brief 读备份寄存器
/// @param BackupRegister 指定备份寄存器地址
/// @return 返回读出的数据
uint16_t myRTC_read_BKP(uint32_t BackupRegister);

/// @brief 写备份寄存器
/// @param BackupRegister 指定备份寄存器地址
/// @param data           指定要写的数据
/// @return 
void myRTC_write_BKP(uint32_t BackupRegister, uint16_t data);

/// @brief 设置闹钟
/// @param alarm_time 闹钟时间
void myRTC_set_Alarm(uint8_t* alarm_time);

// 获取闹钟时间
void myRTC_get_Alarm(uint8_t* alarm_time);

#endif
