#include "oled_show.h"
	
void oled_input_pass(void)
{
	OLED_Clear();
	// 提示文字: 请输入密码
	OLED_ShowChineseTips(1, 3, 0);
	OLED_ShowChineseTips(1, 5, 1);
	OLED_ShowChineseTips(1, 7, 2);
	OLED_ShowChineseTips(1, 9, 3);
	OLED_ShowChineseTips(1, 11, 4);
	OLED_ShowChar(1, 13, ':');
}

void oled_islocked(uint8_t judge, uint8_t display)
{
	if(display == ON)
	{
		if(judge == YES)
		{
			// 提示文字: 已锁定
			OLED_ShowChineseTips(3, 5, 5);
			OLED_ShowChineseTips(3, 7, 7);
			OLED_ShowChineseTips(3, 9, 8);
		}
		else
		{
			// 提示文字: 已解锁
			OLED_ShowChineseTips(2, 5, 5);
			OLED_ShowChineseTips(2, 7, 6);
			OLED_ShowChineseTips(2, 9, 7);
		}
	}
	else OLED_ShowString(3, 1, "                ");
}
	
void oled_islock_Img(uint8_t judge)
{
	if(judge == YES)
	{
		OLED_ShowImg(4, 7, YES);
	}
	else
	{
		OLED_ShowImg(4, 7, NO);
	}
}	

void oled_noPwd(uint8_t display)
{
	if(display == ON)
	{
		// 提示文字: 请设定密码
		OLED_ShowChineseTips(4, 3, 0);
		OLED_ShowChineseTips(4, 5, 12);
		OLED_ShowChineseTips(4, 7, 8);
		OLED_ShowChineseTips(4, 9, 3);
		OLED_ShowChineseTips(4, 11, 4);
		OLED_ShowChar(4, 13, '!');
	}
	else 
	{
		OLED_ShowString(4, 1, "                ");
	}
}
	
void oled_set_done(uint8_t display)
{
	OLED_Clear();
	if(display == ON)
	{
		// 提示文字: 设定成功！
		OLED_ShowChineseTips(2, 4, 12);
		OLED_ShowChineseTips(2, 6, 8);
		OLED_ShowChineseTips(2, 8, 13);
		OLED_ShowChineseTips(2, 10, 14);
		OLED_ShowChar(2, 12, '!');
	}
	else
		OLED_ShowString(2, 1, "                ");
}

void oled_isCorrent_Pwd(uint8_t judge, uint8_t display)
{
	if(display == ON)
	{
		if(judge == YES)
		{
			// 提示文字: 密码正确
			OLED_ShowChineseTips(2, 4, 3);
			OLED_ShowChineseTips(2, 6, 4);
			OLED_ShowChineseTips(2, 8, 17);
			OLED_ShowChineseTips(2, 10, 18);
			OLED_ShowChar(2, 12, '!');
		}
		else
		{
			// 提示文字: 密码错误
			OLED_ShowChineseTips(2, 4, 3);
			OLED_ShowChineseTips(2, 6, 4);
			OLED_ShowChineseTips(2, 8, 19);
			OLED_ShowChineseTips(2, 10, 20);
			OLED_ShowChar(2, 12, '!');
		}
	}
	else
		OLED_ShowString(2, 1, "                ");
	
}

void oled_input_isNew_Pwd(uint8_t judge)
{
	OLED_Clear();
	if(judge == YES)
	{
		// 提示文字: 请输入新密码 
		OLED_ShowChineseTips(1, 2, 0);
		OLED_ShowChineseTips(1, 4, 1);
		OLED_ShowChineseTips(1, 6, 2);
		OLED_ShowChineseTips(1, 8, 16);
		OLED_ShowChineseTips(1, 10, 3);
		OLED_ShowChineseTips(1, 12, 4);
		OLED_ShowChar(1, 14, ':');
	}
	else
	{
		// 提示文字: 请输入旧密码 
		OLED_ShowChineseTips(1, 2, 0);
		OLED_ShowChineseTips(1, 4, 1);
		OLED_ShowChineseTips(1, 6, 2);
		OLED_ShowChineseTips(1, 8, 15);
		OLED_ShowChineseTips(1, 10, 3);
		OLED_ShowChineseTips(1, 12, 4);
		OLED_ShowChar(1, 14, ':');
	}	
}

void fobid_pass(void)
{
	OLED_Clear();
	// 提示文字: 密码已禁用
	OLED_ShowChineseTips(2, 3, 3);
	OLED_ShowChineseTips(2, 5, 4);
	OLED_ShowChineseTips(2, 7, 5);
	OLED_ShowChineseTips(2, 9, 21);
	OLED_ShowChineseTips(2, 11, 22);
	OLED_ShowChar(2, 13, '!');
}
