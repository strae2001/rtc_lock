#include "myRTC.h"
#include "usart.h"
#include "led.h"
#include "OLED.h"
#include "stm32f1xx_hal.h"

// 闹钟计数器复位值
#define RTC_ALARM_RESETVALUE          0xFFFFFFFFU

RTC_HandleTypeDef hrtc;

void myRTC_Init(void)
{
    // 使能电源时钟 BKP时钟
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_RCC_BKP_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();         // 使能访问bkp和rtc寄存器

    hrtc.Instance = RTC;
    hrtc.Init.AsynchPrediv = 40000 - 1;             // 分频
    hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;       // 忽略RTC入侵功能
    HAL_RTC_Init(&hrtc);
	
}

void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
    __HAL_RCC_RTC_ENABLE();

    RCC_OscInitTypeDef  RCC_OscInitStruct;
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;      // 振荡器选择LSI
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;                        // LSI状态开启
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;                  // 不使用PLL
    HAL_RCC_OscConfig(&RCC_OscInitStruct);        // 初始化振荡器

    RCC_PeriphCLKInitTypeDef  PeriphClkInit;
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;        // 外设时钟配置为RTC
    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;        // 选择RTC时钟源为LSI
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);       

	// 为闹钟设置中断
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);

}

// RTC闹钟中断服务函数
void RTC_Alarm_IRQHandler(void)
{
	if (__HAL_RTC_ALARM_GET_IT_SOURCE(&hrtc, RTC_IT_ALRA))
	{
		if(__HAL_RTC_ALARM_GET_FLAG(&hrtc, RTC_CRL_ALRF) != (uint32_t)RESET)
		{
			led_on(LED1);
			
			/* 清闹钟溢出中断标志位 */
			__HAL_RTC_ALARM_CLEAR_FLAG(&hrtc, RTC_CRL_ALRF);
		}    
	}
	
	/* 清闹钟外部触发中断标志位 */
	__HAL_RTC_ALARM_EXTI_CLEAR_FLAG();
}

uint16_t myRTC_read_BKP(uint32_t BackupRegister)
{
    return HAL_RTCEx_BKUPRead(&hrtc, BackupRegister);
}

void myRTC_write_BKP(uint32_t BackupRegister, uint16_t data)
{
    HAL_RTCEx_BKUPWrite(&hrtc, BackupRegister, data);
}

void myRTC_setTime(uint16_t* time_data)
{
	uint32_t set_tim_stamp;
    struct tm set_tim_data;
	
	set_tim_data.tm_year = time_data[0] - 1900;
	set_tim_data.tm_mon  = time_data[1] - 1;
	set_tim_data.tm_mday = time_data[2];
	set_tim_data.tm_hour = time_data[3];
	set_tim_data.tm_min  = time_data[4];
	set_tim_data.tm_sec  = time_data[5];
	
	set_tim_stamp = mktime(&set_tim_data);
	
	// 等待上一次写操作完成
	while(!(hrtc.Instance->CRL & RTC_CRL_RTOFF));
	
	// 失能写保护
	__HAL_RTC_WRITEPROTECTION_DISABLE(&hrtc);
	
	// 将时间戳写入RTC_CNT寄存器
	WRITE_REG(hrtc.Instance->CNTH, (set_tim_stamp >> 16U));
    WRITE_REG(hrtc.Instance->CNTL, (set_tim_stamp & RTC_CNTL_RTC_CNT));
	
	// 写入完毕开启写保护
	__HAL_RTC_WRITEPROTECTION_ENABLE(&hrtc);
	
	// 等待写操作完成
	while(!(hrtc.Instance->CRL & RTC_CRL_RTOFF));
}

void myRTC_getTime(uint16_t* get_time_datas)
{
	uint32_t get_tim_stamp;
    struct tm get_tim_data = {0};
	
	// 从RTC_CNT寄存器获取RTC时间戳
    get_tim_stamp = RTC->CNTH; 	// 获取高16位
    get_tim_stamp <<= 16U;
    get_tim_stamp |= RTC->CNTL; // 获取低16位

	get_tim_data = *localtime(&get_tim_stamp);
	
    get_time_datas[0] = get_tim_data.tm_year + 1900;
    get_time_datas[1] = get_tim_data.tm_mon  + 1;
    get_time_datas[2] = get_tim_data.tm_mday;

    get_time_datas[3] = get_tim_data.tm_hour;
    get_time_datas[4] = get_tim_data.tm_min;
    get_time_datas[5] = get_tim_data.tm_sec;
}


void myRTC_set_Alarm(uint8_t* alarm_time)
{
//	RTC_AlarmTypeDef sAlarm;
//	//sAlarm.Alarm = RTC_ALARM_A;         // 指定alarm ID
// 
//	// 指定告警具体时间
//	sAlarm.AlarmTime.Hours = alarm_time[0];
//	sAlarm.AlarmTime.Minutes = alarm_time[1];
//	sAlarm.AlarmTime.Seconds = alarm_time[2];
//	HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN);
	
	uint32_t alarm_counter = 0, rtc_tim_stamp = 0;		
    uint32_t hour_s = 0, minute_s = 0, sec = 0;
	uint32_t days_gone = 0;			// 已过去的天数
	
	// 获取当前RTC时间戳
    rtc_tim_stamp = RTC->CNTH; 	// 获取高16位
    rtc_tim_stamp <<= 16U;
    rtc_tim_stamp |= RTC->CNTL; // 获取低16位
	
	uint32_t tmp_hours =  rtc_tim_stamp / 3600U;	// 计算一共过了多少小时
	
	if(tmp_hours >= 24U)		// 当小时数大于等于24, 即跨天
	{
		days_gone = (tmp_hours / 24U);				// 得到已过去的天数
		rtc_tim_stamp =  days_gone * 24U * 3600U;	// 将过去的天数转换为秒数
	}
	else
	{
		rtc_tim_stamp = 0;		
	}
	
	// 将闹钟时间转换为秒数
	hour_s   = alarm_time[0] * 3600U;		// 每小时3600秒
	minute_s = alarm_time[1] * 60U;
	sec	     = alarm_time[2];
	
	// 总秒数(年月日需要和RTC时间同步): 已过去的天数总时间+设置的闹钟秒数
	alarm_counter = hour_s + minute_s + sec + rtc_tim_stamp;	
	
	// 等待上次写操作结束
	while ((hrtc.Instance->CRL & RTC_CRL_RTOFF) == (uint32_t)RESET);
	
	// 失能写保护
	__HAL_RTC_WRITEPROTECTION_DISABLE(&hrtc);
	
	// 将闹钟数据写入RTC闹钟寄存器
	WRITE_REG(hrtc.Instance->ALRH, (alarm_counter >> 16U));
	WRITE_REG(hrtc.Instance->ALRL, (alarm_counter & RTC_ALRL_RTC_ALR));
	
	// 写入完毕，打开写保护
	__HAL_RTC_WRITEPROTECTION_ENABLE(&hrtc);
	
	// 等待本次写操作结束
	while ((hrtc.Instance->CRL & RTC_CRL_RTOFF) == (uint32_t)RESET);
	
	// 同时将alarm_counter写入备份寄存器备份
	myRTC_write_BKP(RTC_BKP_DR2, alarm_counter >> 16U);		// DR2备份高16位
	myRTC_write_BKP(RTC_BKP_DR3, alarm_counter);			// DR3备份低16位
	
	/* 清闹钟中断标志位 */
	RTC->CRL &= ~RTC_CRL_ALRF;
	
	// 开启闹钟中断
	RTC->CRH |= RTC_CRH_ALRIE;
}

void myRTC_get_Alarm(uint8_t* alarm_time)
{	
	uint32_t read_alarm_counter = 0U;
	
	// 读出闹钟数据
	read_alarm_counter = RTC->ALRH; 	// 获取高16位
    read_alarm_counter <<= 16U;
    read_alarm_counter |= RTC->ALRL; 	// 获取低16位
	
	// 若闹钟计数器为复位值则读取备份寄存器中的闹钟数据
	if(read_alarm_counter == RTC_ALARM_RESETVALUE)
	{
		read_alarm_counter = 0U;	// 清空源闹钟数据
		
		// 从备份寄存器读取闹钟值
		read_alarm_counter = myRTC_read_BKP(RTC_BKP_DR2);	// 高16位
		read_alarm_counter <<= 16U;
		read_alarm_counter |= myRTC_read_BKP(RTC_BKP_DR3);	// 低16位
	}
	
	// 转换为标准格式（时分秒）
	alarm_time[0] = (uint8_t)((read_alarm_counter / 3600U) % 24U);
	if(read_alarm_counter % 3600U == 0)
	{
		alarm_time[1] = 0;
		alarm_time[2] = 0;
	}
	else
	{
		alarm_time[1] = (uint8_t)((read_alarm_counter % 3600U) / 60U);
		alarm_time[2] = (uint8_t)((read_alarm_counter % 3600U) % 60U);
	}
}
