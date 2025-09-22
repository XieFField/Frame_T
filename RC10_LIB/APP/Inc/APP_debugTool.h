/**
 * @file    APP_debugTool.h
 * @author  XieFField
 * @brief   ������ʱ�����õ� UART printf ��װ
 *          ֮������debug����Ҳ���ԷŽ�������     
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
 * @brief ���ڴ�ӡ��������
 */
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
            HAL_UART_Transmit(huart_, Sendbuf, len,100);  // ͨ�������ַ���
        }
    }
private:
    UART_HandleTypeDef *huart_;
    uint8_t Sendbuf[SEND_BUF_SIZE];
};

#endif

#endif // __DEBUG_UART_H