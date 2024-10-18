#ifndef __MYSPI_H
#define __MYSPI_H

#include "sys.h"
#include "delay.h"

// CS SCK MOSI MISO 时钟使能定义
#define mySPI_CLK_ENABLE  __HAL_RCC_GPIOA_CLK_ENABLE()

// CS SCK MOSI MISO 引脚定义
#define CS_PORT    GPIOA                // CS 为从设备片选信号
#define CS_PIN     GPIO_PIN_3

#define SCK_PORT   GPIOA                // SCK  SPI通信 时钟线
#define SCK_PIN    GPIO_PIN_5

#define MOSI_PORT  GPIOA                // MOSI SPI通信 主设备输出从设备输入线
#define MOSI_PIN   GPIO_PIN_6

#define MISO_PORT  GPIOA                // MISO 作为stm32主设备接收线 输入模式
#define MISO_PIN   GPIO_PIN_7

// CS SCK MOSI 输出高低电平
#define CS_HIGH    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET)
#define CS_LOW     HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET)

#define SCK_HIGH   HAL_GPIO_WritePin(SCK_PORT, SCK_PIN, GPIO_PIN_SET)
#define SCK_LOW    HAL_GPIO_WritePin(SCK_PORT, SCK_PIN, GPIO_PIN_RESET)

#define MOSI_HIGH  HAL_GPIO_WritePin(MOSI_PORT, MOSI_PIN, GPIO_PIN_SET)
#define MOSI_LOW   HAL_GPIO_WritePin(MOSI_PORT, MOSI_PIN, GPIO_PIN_RESET)

// 读取 MISO 引脚信号
#define MISO_READ  HAL_GPIO_ReadPin(MISO_PORT, MISO_PIN)

// SPI标准延迟时间
#define SPI_DELAY  delay_us(5)


/*   函数定义   */

// SPI通信协议初始化
void mySPI_Init(void);

//数据传输开始
void mySPI_Start(void);

//数据传输结束
void mySPI_Stop(void);

/**
 * @description: 主从设备交换一个字节数据(主从根据时钟信号同时收发)
 * @param byte   要发送的字节数据
 * @return {uint8_t}  收到的字节数据
 */
uint8_t mySPI_SwapByte(uint8_t byte);


#endif
