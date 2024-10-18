#include "W25Q128.h"

/*
    W25Q128 flash存储器地址由3个字节(24位)构成, 第一个字节(前8位)是块地址, 
            第二个字节前4位是段首地址, 后4位页首地址, 第三个字节(后8位)是页内地址
*/

void W25Q128_Init(void)
{
    mySPI_Init();           // SPI通信接口初始化
}

void W25Q128_readID(uint8_t* makeID, uint16_t* devID)
{
    mySPI_Start();

    // 发送指令 9fh 使能从机发来ID
    mySPI_SwapByte(0x9f);

    // 接收ID信息 (为了读取数据, 发送什么不重要, 但不可发送指令)
    *makeID = mySPI_SwapByte(0xff);

    *devID = 0;
    *devID |= mySPI_SwapByte(0xff);        // 函数返回的是一个字节数据, 且从机发送数据是从高位开始传输
    *devID <<= 8;                          // 所以接收到数据后, 需要左移8位将数据移至高8位

    *devID |= mySPI_SwapByte(0xff);        // 接收低8位数据

    mySPI_Stop();
}

// 开启写使能
void W25Q128_WriteEnable(void)
{
    mySPI_Start();
    mySPI_SwapByte(0x06);   // 发送写使能指令
    mySPI_Stop();
}

// 关闭写使能
void W25Q128_WriteDisable(void)
{
    mySPI_Start();  
    mySPI_SwapByte(0x04);   // 发送写失能指令
    mySPI_Stop();
}

// 等待忙信号结束
void W25Q128_wait_NotBusy(void)
{
    mySPI_Start();  
    mySPI_SwapByte(0x05);   // 发送读取状态寄存器指令

    while (mySPI_SwapByte(0xff) & 0x01)
    {
        /* 等待忙信号位(最低位)置0 */
    }

    mySPI_Stop();
}

// 擦除段, 给出块和段地址
void W25Q128_EraseSector(uint8_t blockAddr, uint8_t secAddr)
{
    // 首先等待状态不为忙
    W25Q128_wait_NotBusy();

    // 开启写使能
    W25Q128_WriteEnable();

    // 计算要发送的地址（至段首地址） 这里地址的第二个字节前4位为段地址, 故左移12位
    uint32_t addr = (blockAddr << 16) | (secAddr << 12);

    mySPI_Start();

    mySPI_SwapByte(0x20);       // 发送段擦除指令

    // 发送要擦除的24位地址
    mySPI_SwapByte((addr >> 16) & 0xff);        // 指定要擦除的块地址
    mySPI_SwapByte((addr >> 8) & 0xff);         // 指定要擦除的段地址+页地址(页地址为0)
    mySPI_SwapByte((addr >> 0) & 0xff);         // 页内地址为0

    mySPI_Stop();

    // 关闭写使能
    W25Q128_WriteDisable();
}


/**
 * @description: 按页写入数据
 * @param {uint8_t} blockAddr   块地址
 * @param {uint8_t} secAddr     段地址
 * @param {uint8_t} pageAddr    页地址
 * @param {uint8_t} data        要写入数据的首地址
 * @param {uint16_t} len        写入多少个字节数据
 */
void W25Q128_pageWrite(uint8_t blockAddr, uint8_t secAddr, uint8_t pageAddr, uint8_t* data, uint16_t len)
{
    // 首先等待状态不为忙
    W25Q128_wait_NotBusy();

    // 开启写使能
    W25Q128_WriteEnable();

    // 计算要发送的地址（至页首地址）
    uint32_t addr = (blockAddr << 16) | (secAddr << 12) | (pageAddr << 8);

    mySPI_Start();

    mySPI_SwapByte(0x02);       // 发送页写指令

    // 发送要写入的24位地址
    mySPI_SwapByte(addr >> 16 & 0xff);           // 指定要写入的块地址
    mySPI_SwapByte(addr >> 8 & 0xff);            // 指定要写入的段地址和页地址
    mySPI_SwapByte(addr >> 0 & 0xff);            // 指定要写入的页内地址

    // 依次发送数据
    for (uint16_t i = 0; i < len; i++)
    {
        mySPI_SwapByte(data[i]);
    }
    
    mySPI_Stop();

    // 关闭写使能
    W25Q128_WriteDisable();
}


/**
 * @description: 读取数据
 * @param {uint8_t} blockAddr   块地址
 * @param {uint8_t} secAddr     段地址
 * @param {uint8_t} pageAddr    页地址
 * @param {uint8_t} InPageAddr  具体要读取数据的地址（页内地址）
 * @param {uint8_t} buffer      将数据读取存至缓冲区
 * @param {uint16_t} len        读取多少个字节的数据
 */
void W25Q128_Read(uint8_t blockAddr, uint8_t secAddr, uint8_t pageAddr, uint8_t InPageAddr, uint8_t* buffer, uint16_t len)
{
    // 首先等待状态不为忙
    W25Q128_wait_NotBusy();

    // 计算要发送的地址（至页内地址）
    uint32_t addr = (blockAddr << 16) | (secAddr << 12) | (pageAddr << 8) | InPageAddr;

    mySPI_Start();

    mySPI_SwapByte(0x03);       // 发送读数据指令

    // 发送要读的24位地址
    mySPI_SwapByte(addr >> 16);         // 指定要读的块地址
    mySPI_SwapByte(addr >> 8);          // 指定要读的段首和页首地址
    mySPI_SwapByte(addr >> 0);          // 指定要读的具体页内地址

    // 依次读取数据
    for (uint16_t i = 0; i < len; i++)
    {
        buffer[i] = mySPI_SwapByte(0xff);
    }

    mySPI_Stop();
}
