#ifndef __USART_H
#define __USART_H

#include "sys.h"
#include <stdio.h>

#define TX1 GPIO_PIN_9
#define RX1 GPIO_PIN_10


void serial_Init(void);
void serial_sendByte(uint8_t Byte);
void serial_sendArrary(uint8_t* Arr, uint8_t len);
void serial_sendString(uint8_t* str);
void serial_sendNum(uint32_t Num, uint8_t width);

#endif
