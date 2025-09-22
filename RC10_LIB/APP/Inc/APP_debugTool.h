/**
 * @file    APP_debugTool.h
 * @author  XieFField
 * @brief   用于临时调试用的 UART printf 封装
 *          之后其他debug工具也可以放进这里面     
 * @version 1.0
 */

#ifndef __APP_DEBUG_TOOL_H
#define __APP_DEBUG_TOOL_H

#pragma once

#ifdef __cplusplus
extern "C" {
    #include "stm32h7xx_hal.h"
    #include <stdarg.h> 
    #include <string.h>
    #include "usart.h"
}
#endif

#ifdef __cplusplus

#include <cstdint>
#include <cstdio>
#define SEND_BUF_SIZE 100
/**
 * @brief 串口打印调试助手
 */
class Debug_Printf {
public:
    Debug_Printf(UART_HandleTypeDef *huart) : huart_(huart) {}
    ~Debug_Printf() {}

    void printf_DMA(char *fmt, ...)
    {
        memset(Sendbuf, 0, SEND_BUF_SIZE);  // 清空发送缓冲区
        
        va_list arg;
        va_start(arg, fmt);
        vsnprintf((char*)Sendbuf, SEND_BUF_SIZE, fmt, arg);  // 安全的格式化输出，防止缓冲区溢出
        va_end(arg);
        
        uint8_t len = strlen((char*)Sendbuf);  // 计算实际字符串长度
        if(len > 0)
        {
            HAL_UART_Transmit(huart_, Sendbuf, len,100);  // 通过发送字符串
        }
    }
private:
    UART_HandleTypeDef *huart_;
    uint8_t Sendbuf[SEND_BUF_SIZE];
};

#endif

#endif // __DEBUG_UART_H