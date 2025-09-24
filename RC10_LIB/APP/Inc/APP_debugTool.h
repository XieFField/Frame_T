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
            HAL_UART_Transmit_DMA(huart_, Sendbuf, len);  // ͨ�������ַ���
        }
    }

    void printf_UART(char *fmt, ...) 
    {  // �������޸������ַ�DMA�汾
        if (fmt == NULL) 
            return;  // ��ֹ��ָ�����
        
        va_list arg;
        va_start(arg, fmt);
        
        // 1. ��仺��������ȡʵ����Ҫ�ĳ��ȣ�������ֹ����
        int ret = vsnprintf((char*)Sendbuf, SEND_BUF_SIZE, fmt, arg);
        va_end(arg);
        
        // 2. У���������������Ч���
        if (ret <= 0 || ret >= SEND_BUF_SIZE) 
            return;  // ���ʧ�ܻ����ݳ���
        
        uint16_t send_len = (uint16_t)ret;  // ��Ч���ͳ���
        
        // 3. ���UART״̬��ȷ������
        if (HAL_UART_GetState(huart_) != HAL_UART_STATE_READY) 
            // ����ʽ��������ȴ�DMA��ֱ�ӳ�������UART
            HAL_UART_Abort(huart_);
        
        
        // 4. ����ʽ���ͣ��ȴ�������ɣ�
        if (send_len > 0) 
            // ʹ��HAL_UART_Transmit������ʽ������ʱʱ����Ϊ100ms
            HAL_UART_Transmit(huart_, Sendbuf, send_len, time_out);
    }

    void reset_timeout(uint32_t timeout_ms) 
    {
        time_out = timeout_ms;
    }

private:
    uint32_t time_out = 100;
    UART_HandleTypeDef *huart_;
    uint8_t Sendbuf[SEND_BUF_SIZE];
};

#endif

#endif // __DEBUG_UART_H