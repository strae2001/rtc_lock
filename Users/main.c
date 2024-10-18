#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "keyboard.h"
#include "OLED.h"
#include "oled_show.h"
#include "W25Q128.h"
#include "myRTC.h"
#include "pwd.h"

void myRTC_Time_Init(void);			// 初始化RTC时钟
void OLED_Show_RTC_Time(void);		// 实时显示RTC时钟

void unlock_State(void);			// 解锁状态执行代码
void locked_State(void);			// 锁定状态执行代码

extern uint8_t pwd_cost;			// 密码输错机会

uint8_t lockState = YES;			// 锁状态标志位  默认上锁
uint8_t time_data[] = {24, 10, 13, 16, 16, 0};

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
	delay_init(72);						// 延时初始化 依据时钟频率为72mhz
	
	led_init();
    serial_Init();
	keyboard_init();
	OLED_Init();
	W25Q128_Init();
	myRTC_Init();
	
	// W25Q128_EraseSector(0,0);		// 擦除密码
	myRTC_Time_Init();
	
    while(1)
    { 
		// 检查密码是否已设定
		if(check_pwd_isExist() == YES)			// 设备密码已设定
		{
			if(lockState == YES)		// 锁定状态
			{
				locked_State();
			}
			else if(lockState == NO)	// 解锁状态
			{
				unlock_State();
			}
		}
		else									// 设备无密码
		{
			OLED_Show_RTC_Time();
			oled_noPwd(ON);
			delay_ms(360);
			oled_noPwd(OFF);
			delay_ms(360);
			
			if(keyboard_get_value())					// 键盘按下任意键 开始设定密码
			{
				oled_input_pass();		
				while(1)
				{
					while(get_input_Pwd() != '#')		// 等待输入密码按#号结束
					{
						OLED_Line_clean(2);
					}
					if(save_pwd() == YES)
					{
						break;							// 密码设定成功 跳转至锁定界面
					}
				}
			}
		}
    }
}


// 锁定状态下代码
void locked_State(void)
{
	OLED_Clear();
	pwd_cost = 3;			// 密码输错机会次数重置
	while(1)
	{
		OLED_Show_RTC_Time();
		oled_islock_Img(YES);
		
		switch(keyboard_get_value())
		{
			case '#':
				oled_input_pass();
				while(get_input_Pwd() != '#')		// 等待输入密码按#号结束
				{
					OLED_Line_clean(2);				
				}
				if(PWD_cmp() == YES)
				{
					input_pwd_True_action();
					lockState = NO;					// 状态转为解锁
				}
				else 
					input_pwd_False_action();
				break;
				
			case '*':
				pwd_upate();
				break;
		}
		if(lockState == NO)
		{
			oled_islocked(NO, ON);
			delay_ms(500);
			break;					// 跳出锁定状态
		}
	}
}

// 解锁状态下代码
void unlock_State(void)
{
	OLED_Clear();
	pwd_cost = 3;			// 密码输错机会次数重置
	while(1)
	{
		OLED_Show_RTC_Time();
		oled_islock_Img(NO); 
		switch(keyboard_get_value())
		{
			case '#':
				lockState = YES;		// 状态转为锁定
				break;
			
			case '*':
				pwd_upate();
				break;
		}
		if(lockState == YES)
		{
			oled_islocked(YES, ON);
			delay_ms(500);
			break;						// 跳出解锁状态
		}
	}
}

void myRTC_Time_Init(void)
{
	myRTC_setTime(time_data);
}

void OLED_Show_RTC_Time(void)
{
	myRTC_getTime(time_data);		// 不断获取RTC时间
	
	// 第一行: 日期
	OLED_ShowNum(1, 2, time_data[YEAR] + 2000, 4);
	OLED_ShowChinese_TimeWord(1, 6, YEAR);		
	OLED_ShowNum(1, 8, time_data[MONTH], 2);
	OLED_ShowChinese_TimeWord(1, 10, MONTH);			
	OLED_ShowNum(1, 12, time_data[MDAY], 2);
	OLED_ShowChinese_TimeWord(1, 14, MDAY);	

	// 第二行: 时间
	OLED_ShowNum(2, 4, time_data[HOUR], 2);
	OLED_ShowChar(2,6,':');
	OLED_ShowNum(2, 7, time_data[MINUTE], 2);
	OLED_ShowChar(2,9,':');			
	OLED_ShowNum(2, 10, time_data[SECONDS], 2);
}
