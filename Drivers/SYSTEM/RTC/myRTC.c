#include "myRTC.h"
#include "usart.h"
#include "led.h"

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
    HAL_RTC_AlarmIRQHandler(&hrtc);         // RTC闹钟中断公共处理函数
}

// RTC闹钟回调函数, 由中断公共处理函数调用
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
    led_on(LED1);
}

uint16_t myRTC_read_BKP(uint32_t BackupRegister)
{
    return HAL_RTCEx_BKUPRead(&hrtc, BackupRegister);
}

void myRTC_write_BKP(uint32_t BackupRegister, uint16_t data)
{
    HAL_RTCEx_BKUPWrite(&hrtc, BackupRegister, data);
}

void myRTC_setTime(uint8_t* time_data)
{
    RTC_DateTypeDef RTC_date;
    RTC_date.Year = time_data[0];
    RTC_date.Month = time_data[1];
    RTC_date.Date = time_data[2];
    HAL_RTC_SetDate(&hrtc, &RTC_date, RTC_FORMAT_BIN);

    RTC_TimeTypeDef RTC_time;
    RTC_time.Hours = time_data[3];
    RTC_time.Minutes = time_data[4];
    RTC_time.Seconds = time_data[5];
    HAL_RTC_SetTime(&hrtc, &RTC_time, RTC_FORMAT_BIN);

    while (__HAL_RTC_ALARM_GET_FLAG(&hrtc, RTC_FLAG_RTOFF) == RESET)
    {
        /* 等待RTC上一次操作完成 */
    }
}

void myRTC_getTime(uint8_t* time_data)
{
    RTC_TimeTypeDef RTC_time;
    RTC_DateTypeDef RTC_date;
    HAL_RTC_GetDate(&hrtc, &RTC_date, RTC_FORMAT_BIN);
    HAL_RTC_GetTime(&hrtc, &RTC_time, RTC_FORMAT_BIN);

    time_data[0] = RTC_date.Year;
    time_data[1] = RTC_date.Month;
    time_data[2] = RTC_date.Date;

    time_data[3] = RTC_time.Hours;
    time_data[4] = RTC_time.Minutes;
    time_data[5] = RTC_time.Seconds;
}


void myRTC_set_Alarm(uint8_t* alarm_time)
{
    RTC_AlarmTypeDef sAlarm;
    sAlarm.Alarm = RTC_ALARM_A;         // 指定alarm ID

    // 指定告警具体时间
    sAlarm.AlarmTime.Hours = alarm_time[0];
    sAlarm.AlarmTime.Minutes = alarm_time[1];
    sAlarm.AlarmTime.Seconds = alarm_time[2];
    HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN);
}

void myRTC_get_Alarm(uint8_t* alarm_time)
{
    RTC_AlarmTypeDef sAlarm;
    HAL_RTC_GetAlarm(&hrtc, &sAlarm, RTC_ALARM_A, RTC_FORMAT_BIN);

    alarm_time[0] = sAlarm.AlarmTime.Hours;
    alarm_time[1] = sAlarm.AlarmTime.Minutes;
    alarm_time[2] = sAlarm.AlarmTime.Seconds;
}
