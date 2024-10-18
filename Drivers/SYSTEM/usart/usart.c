#include "usart.h"


uint8_t RxData;         // 接收数据缓冲区
extern uint8_t RxStr[];
extern uint8_t Rx_flag;

UART_HandleTypeDef huart1;

void serial_Init(void)
{
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 9600;
    huart1.Init.Mode = UART_MODE_TX_RX;        // 收发模式
    huart1.Init.Parity = UART_PARITY_NONE;     // 不需要校验位
    huart1.Init.StopBits = UART_STOPBITS_1;    // 1位停止位
    huart1.Init.WordLength = UART_WORDLENGTH_8B;       // 每次数据传输8位
    HAL_UART_Init(&huart1);

    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);     // 使能UART 并开启UART_IT_RXNE触发中断
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart1)
{
    if(huart1->Instance == USART1)
    {
        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        GPIO_InitTypeDef GPIO_Init;
        GPIO_Init.Pin = TX1;
        GPIO_Init.Mode = GPIO_MODE_AF_PP;
        GPIO_Init.Pull = GPIO_PULLUP;
        GPIO_Init.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIO_Init);

        GPIO_Init.Pin = RX1;
        GPIO_Init.Mode = GPIO_MODE_INPUT;
        HAL_GPIO_Init(GPIOA, &GPIO_Init);
        
        // 串口接收中断配置
        HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);        // 中断优先级分组
        HAL_NVIC_SetPriority(USART1_IRQn, 1, 1);                   // 中断优先级配置
        HAL_NVIC_EnableIRQ(USART1_IRQn);
		
    }
}

void USART1_IRQHandler(void)
{
    if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) != RESET)      // 判断接收数据寄存器空 标志位
    {
        HAL_UART_Receive(&huart1, &RxData, 1, 1000);
        serial_sendByte(RxData);
    }
}

void serial_sendByte(uint8_t Byte)
{
    HAL_UART_Transmit(&huart1, &Byte, 1, 1000);
    while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TXE) == RESET);    
}

void serial_sendArrary(uint8_t* Arr, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++)
    {
        serial_sendByte(Arr[i]);
    }
    
}

void serial_sendString(uint8_t* str)
{
    for (uint8_t i = 0; str[i]!='\0'; i++)
    {
        serial_sendByte(str[i]);
    }
}

uint32_t serial_pow(uint32_t x, uint32_t y)
{
    uint32_t ret = 1;
    while (y--)
    {
        ret *= x;
    }
    return ret;
}

void serial_sendNum(uint32_t Num, uint8_t width)
{
    uint8_t Numbit;
    for (uint8_t i = 0; i < width; i++)
    {
        Numbit = Num / serial_pow(10, width - i - 1) % 10 + '0';
        serial_sendByte(Numbit);
    } 
    
}

int fputc(int ch,  FILE* stream)
{
	serial_sendByte(ch);
	return ch;
}	
