#include "my_SPI.h"

void mySPI_Init()
{
    // 时钟使能
    mySPI_CLK_ENABLE;

    // GPIO初始化
    GPIO_InitTypeDef GPIO_Init;
    GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;           // 通用推挽
    GPIO_Init.Pin = CS_PIN;
    GPIO_Init.Pull = GPIO_NOPULL;                   // 引脚默认浮空
    GPIO_Init.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(CS_PORT, &GPIO_Init);
    GPIO_Init.Pin = MOSI_PIN;
    HAL_GPIO_Init(MOSI_PORT, &GPIO_Init);
    GPIO_Init.Pin = SCK_PIN;
    HAL_GPIO_Init(SCK_PORT, &GPIO_Init);

    GPIO_Init.Mode = GPIO_MODE_INPUT;           // 输入模式
    GPIO_Init.Pull = GPIO_NOPULL;               // 引脚默认浮空
    GPIO_Init.Pin = MISO_PIN;
    HAL_GPIO_Init(MISO_PORT, &GPIO_Init);

    // 选择SPI工作模式为mode0, SCK空闲时低电平
    SCK_LOW;

    // 片选不使能
    CS_HIGH;

    SPI_DELAY;
}

// 数据传输开始和结束
void mySPI_Start()
{
    CS_LOW;       // 片选信号拉低开始传输数据
}

void mySPI_Stop()
{
    CS_HIGH;      // 片选信号拉高结束传输数据
}

uint8_t mySPI_SwapByte(uint8_t byte)
{
    uint8_t rbyte = 0;
    
    // 依次发送和接收8个字节
    for(uint8_t i = 0; i < 8; i++)
    {
        // 准备好数据放在MOSI线上
        if(byte & 0x80)         // 数据从高位开始传输
        {
            MOSI_HIGH;
        }
        else
        {
            MOSI_LOW;
        }
        byte <<= 1;

        SCK_HIGH;       // SCK上升沿触发数据采样
        SPI_DELAY;

        rbyte <<= 1;
        if(MISO_READ)           // 在MISO线上读取从机发来的数据
        {
            rbyte |= 0x01;
        }

        SCK_LOW;        // 拉低时钟, 为下一位数据传输作准备
        SPI_DELAY;  
    }
    return rbyte;       // 返回接收到的字节数据
}
