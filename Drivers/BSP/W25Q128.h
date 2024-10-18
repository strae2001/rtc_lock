#ifndef __W25Q128_H
#define __W25Q128_H

#include "my_SPI.h"

void W25Q128_Init(void);


/**
 * @description:    读取W25Q128存储器ID
 * @param  makeID   制造商ID
 * @param  devID    设备ID
 * @return {*}
 */
void W25Q128_readID(uint8_t* makeID, uint16_t* devID);

//// 开启写使能
//void W25Q128_WriteEnable(void);

//// 关闭写使能
//void W25Q128_WriteDisable(void);

//// 等待忙信号结束
//void W25Q128_wait_NotBusy(void);

// 擦除段, 给出块和段地址
void W25Q128_EraseSector(uint8_t blockAddr, uint8_t secAddr);


/**
 * @description: 按页写入数据
 * @param {uint8_t} blockAddr   块地址
 * @param {uint8_t} secAddr     段地址
 * @param {uint8_t} pageAddr    页地址
 * @param {uint8_t} data        要写入数据的首地址
 * @param {uint16_t} len        写入多少个字节数据
 * @return {*}
 */
void W25Q128_pageWrite(uint8_t blockAddr, uint8_t secAddr, uint8_t pageAddr, uint8_t* data, uint16_t len);


/**
 * @description: 读取数据
 * @param {uint8_t} blockAddr   块地址
 * @param {uint8_t} secAddr     段地址
 * @param {uint8_t} pageAddr    页地址
 * @param {uint8_t} InPageAddr  具体要读取数据的地址（页内地址）
 * @param {uint8_t} buffer      将数据读取存至缓冲区, buffer为缓冲区首地址
 * @param {uint16_t} len        读取多少个字节的数据
 * @return {*}
 */
void W25Q128_Read(uint8_t blockAddr, uint8_t secAddr, uint8_t pageAddr, uint8_t InPageAddr, uint8_t* buffer, uint16_t len);

#endif
