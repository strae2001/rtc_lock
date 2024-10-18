#ifndef __OLEDSHOW_H
#define __OLEDSHOW_H

#define ON  1
#define OFF 0

#include "OLED.h"
	
void oled_input_pass(void);

void oled_islocked(uint8_t judge, uint8_t display);
	
void oled_islock_Img(uint8_t judge);	

void oled_noPwd(uint8_t display);

// 密码设置完毕
void oled_set_done(uint8_t display);

void oled_isCorrent_Pwd(uint8_t judge, uint8_t display);

void oled_input_isNew_Pwd(uint8_t judge);

void fobid_pass(void);

#endif
