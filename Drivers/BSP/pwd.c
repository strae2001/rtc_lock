#include "pwd.h"
#include "oled_show.h"
#include "W25Q128.h"
#include "string.h"
#include "keyboard.h"
#include "usart.h"
#include "led.h"

#define PWD_LEN  6          	// 密码长度

uint8_t pwd_cost = 3;			// 密码输错机会

uint8_t pwd[PWD_LEN] = {0};		// 密码存放缓冲区
uint8_t pwd_cnt = 0;			// 密码缓冲区索引

// 清空密码存放缓冲区
void pwdBuf_clear(void)
{
    memset(pwd, 0, sizeof(pwd));
    pwd_cnt = 0;
}

// 保存密码
uint8_t save_pwd(void)
{
	OLED_Clear();		// 清屏oled

	W25Q128_EraseSector(0, 0);
	W25Q128_pageWrite(0,0,0,pwd, PWD_LEN);

    uint8_t tempbuf[PWD_LEN]={0};
    W25Q128_Read(0,0,0,0, tempbuf, PWD_LEN);

    if(!memcmp(tempbuf, pwd, PWD_LEN))         // 判断是否保存成功
    {
        uint8_t cnt = 2;
        while (cnt--)
        {
            oled_set_done(ON);
            delay_ms(300);
            oled_set_done(OFF);
            delay_ms(300);
        }
        return YES;
    }

	return NO;
}

// 获取键盘输入密码
uint8_t get_input_Pwd(void)
{
	pwdBuf_clear();			// 输入密码前 清空密码存放缓冲区
	uint8_t btn = 0;
	while(1)
	{
		btn = keyboard_get_value();
		if(btn == '#' || btn == '*')
		{
			return btn;
		}
		else if(btn != 0)				// btn初始化即为0 故不能等于0 
		{
			if (pwd_cnt < PWD_LEN)
			{
				pwd[pwd_cnt++] = btn;
				
				if(pwd_cnt == 1) OLED_ShowChar(2, 5, '*');
				else if(pwd_cnt == 2) OLED_ShowChar(2, 6, '*');
				else if(pwd_cnt == 3) OLED_ShowChar(2, 7, '*');
				else if(pwd_cnt == 4) OLED_ShowChar(2, 8, '*');
				else if(pwd_cnt == 5) OLED_ShowChar(2, 9, '*');
				else if(pwd_cnt == 6) OLED_ShowChar(2, 10, '*');
			}
		}
	}
}

// 密码比对
uint8_t PWD_cmp(void)
{
	uint8_t temp[PWD_LEN] = {0};
	W25Q128_Read(0,0,0,0, temp, PWD_LEN);
	if(!memcmp(temp, pwd, PWD_LEN))
	{
		return YES;
	}
	return NO;
}

// 检查密码是否存在
uint8_t check_pwd_isExist(void)
{   
    uint8_t temp[PWD_LEN] = {0};
    W25Q128_Read(0,0,0,0, temp, PWD_LEN);
    if(temp[0] == 0xFF || temp[0] == '\0')			// W25Q128未写入时默认0xFF
        return NO;
    return YES;
}


// 密码输对情况的操作
void input_pwd_True_action(void)
{
	OLED_Clear();
	oled_isCorrent_Pwd(YES, ON);
	delay_ms(500);
	oled_isCorrent_Pwd(YES, OFF);
}

// 密码输错情况的操作
void input_pwd_False_action(void)
{
	OLED_Clear();
	uint8_t i = 2;
	while(i--)
	{
		oled_isCorrent_Pwd(NO, ON);
		delay_ms(300);
		oled_isCorrent_Pwd(NO, OFF);
		delay_ms(300);
	}
	
	if(--pwd_cost == 0)					// 输错密码3次 亮灯警示
	{
		fobid_pass();
		led_on(LED1 | LED2);
		delay_ms(3000);
		led_off(LED1 | LED2);
		OLED_Clear();
		pwd_cost = 3;
	}
}



// 密码修改
void pwd_upate(void)
{
	oled_input_isNew_Pwd(NO);			// 请输入旧密码
	while(get_input_Pwd() != '#')
	{
		OLED_Line_clean(2);
	}
	if(PWD_cmp() == YES)
	{
		input_pwd_True_action();
		oled_input_isNew_Pwd(YES);			
		while(get_input_Pwd() != '#')		// 等待输入新密码完成
		{
			OLED_Line_clean(2);
		}
		save_pwd();
	}
	else
		input_pwd_False_action();
}
