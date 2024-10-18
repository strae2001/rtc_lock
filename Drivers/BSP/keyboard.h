#ifndef __KEYBOART_H
#define __KEYBOART_H

#define ROW1 GPIO_PIN_0
#define ROW2 GPIO_PIN_1
#define ROW3 GPIO_PIN_2
#define ROW4 GPIO_PIN_10

#define COL1 GPIO_PIN_13
#define COL2 GPIO_PIN_12
#define COL3 GPIO_PIN_11

#include "sys.h"

void keyboard_init(void);
uint8_t keyboard_get_value(void);

#endif
