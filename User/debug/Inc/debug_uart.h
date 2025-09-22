/**
 * @file    debug_uart.h
 * @author  XieFField
 * @brief   ������ʱ�����õ� UART printf ��װ
 * @version 1.0
 */

#ifndef __DEBUG_UART_H
#define __DEBUG_UART_H

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
class Debug_Printf {
public:
    Debug_Printf(UART_HandleTypeDef *huart) : huart_(huart) {}
    ~Debug_Printf() {}

    void printf_DMA(char *fmt, ...)
    {
        memset(Sendbuf, 0, SEND_BUF_SIZE);  // ��շ��ͻ�����
        
        va_list arg;
        va_start(arg, fmt);
        vsnprintf((char*)Sendbuf, SEND_BUF_SIZE, fmt, arg);  // ��ȫ�ĸ�ʽ���������ֹ���������
        va_end(arg);
        
        uint8_t len = strlen((char*)Sendbuf);  // ����ʵ���ַ�������
        if(len > 0)
        {
            HAL_UART_Transmit_DMA(huart_, Sendbuf, len);  // ͨ��DMA�����ַ���
        }
    }
private:
    UART_HandleTypeDef *huart_;
    uint8_t Sendbuf[SEND_BUF_SIZE];
};

#endif

#endif // __DEBUG_UART_H