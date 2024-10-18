#ifndef __PWD_H
#define __PWD_H

#include "sys.h"

// 清空密码存放缓冲区
void pwdBuf_clear(void);

// 保存密码
uint8_t save_pwd(void);

// 获取键盘输入
uint8_t get_input_Pwd(void);

// 密码比对
uint8_t PWD_cmp(void);

// 检查密码是否存在
uint8_t check_pwd_isExist(void);

// 密码输入正确后的操作
void input_pwd_True_action(void);

// 密码输入错误后的操作
void input_pwd_False_action(void);

// 密码修改
void pwd_upate(void);

#endif
