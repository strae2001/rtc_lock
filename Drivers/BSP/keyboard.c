#include "keyboard.h"
#include "delay.h"
#include "usart.h"

uint8_t key_val = 0;

void keyboard_init(void)
{
	// 打开时钟
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	// 初始化GPIO
	GPIO_InitTypeDef gpio_initStruct;
	gpio_initStruct.Pin = ROW2 | ROW1 | ROW3 | ROW4;
	gpio_initStruct.Mode = GPIO_MODE_IT_FALLING;			// 外部中断下降沿触发
	gpio_initStruct.Pull = GPIO_PULLUP;
	gpio_initStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &gpio_initStruct);

	gpio_initStruct.Pin = COL2 | COL1 | COL3;
	gpio_initStruct.Mode = GPIO_MODE_INPUT;					// 输入模式
	gpio_initStruct.Pull = GPIO_PULLDOWN;					// 默认下拉
	HAL_GPIO_Init(GPIOB, &gpio_initStruct);

	// 开启行中断
	HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

	HAL_NVIC_SetPriority(EXTI1_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);
	HAL_NVIC_SetPriority(EXTI2_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	
}

void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(ROW1);
}

void EXTI1_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(ROW2);
}

void EXTI2_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(ROW3);
}

void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(ROW4);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	
	uint8_t row = 0, col = 0;

	// 防止按键多次按下
	if(key_val != 0) return;		

	// 确定行
	if(GPIO_Pin == ROW1) row = 0x10;
	if(GPIO_Pin == ROW2) row = 0x20;
	if(GPIO_Pin == ROW3) row = 0x30;
	if(GPIO_Pin == ROW4) row = 0x40;
	
	// 确定列
	if(HAL_GPIO_ReadPin(GPIOB, COL1) == GPIO_PIN_SET)
	{
		delay_ms(10);
		while (HAL_GPIO_ReadPin(GPIOB, COL1) == SET);
		col = 0x01;
	} 
	else if(HAL_GPIO_ReadPin(GPIOB, COL2) == GPIO_PIN_SET)
	{
		delay_ms(10);
		while (HAL_GPIO_ReadPin(GPIOB, COL2));
		col = 0x02;
	}
	else if(HAL_GPIO_ReadPin(GPIOB, COL3) == GPIO_PIN_SET)
	{
		delay_ms(10);
		while (HAL_GPIO_ReadPin(GPIOB, COL3));
		col = 0x03;
	}

	if(row && col)
	{
		key_val = row | col;
	}
}

uint8_t keyboard_get_value(void)
{
	uint8_t ch = 0;

	if(key_val)
	{
		if(key_val == 0x11) ch = '1';
		else if(key_val == 0x12) ch = '2';
		else if(key_val == 0x13) ch = '3';
		else if(key_val == 0x21) ch = '4';
		else if(key_val == 0x22) ch = '5';
		else if(key_val == 0x23) ch = '6';
		else if(key_val == 0x31) ch = '7';
		else if(key_val == 0x32) ch = '8';
		else if(key_val == 0x33) ch = '9';
		else if(key_val == 0x41) ch = '*';
		else if(key_val == 0x42) ch = '0';
		else if(key_val == 0x43) ch = '#';
		
		// printf("按下了:%c键\r\n", ch);
		
		delay_ms(200);
		key_val = 0;
	}
	return ch;
}
