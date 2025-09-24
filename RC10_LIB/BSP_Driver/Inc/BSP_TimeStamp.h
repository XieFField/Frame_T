/**
 * @file BSP_TimeStamp.h
 * @author XieFField
 * @brief ʱ����񣬿����ṩ΢�뼶ʱ���
 *        ��Ҫ���ǲο������룬����C/C++��϶Ƚϸߣ�ȫ�ֱ�������
 *        �ڴ���������
 * @version 1.0
 * 
 * @version 2.0
 *       ������64λ������µ���������bug
 *       �Ż����ٽ����Ĵ�����Ϊԭ�Ӳ���
 */

/*
  _______               ___________                    
 /_  __(_)___ ___  ___ / ___/_  __/___ _____ ___  ____ 
  / / / / __ `__ \/ _ \\__ \ / / / __ `/ __ `__ \/ __ \
 / / / / / / / / /  __/__/ // / / /_/ / / / / / / /_/ /
/_/ /_/_/ /_/ /_/\___/____//_/  \__,_/_/ /_/ /_/ .___/ 
                                              /_/      
*/

#ifndef __BSP_TIMESTAMP_H
#define __BSP_TIMESTAMP_H

#ifdef __cplusplus
extern "C" {
#endif
#include "stm32h7xx_hal.h"
#include "tim.h"

#ifdef __cplusplus
}
#endif // __cplusplus

#ifdef __cplusplus

#include <cstdint>


class TimeStamp{
public:
    static TimeStamp& getInstance(); //��ȡΨһʵ��

    /**
     * @brief ��ʼ��ʱ�������
     * @param htim ָ��������Ϊ����������ģʽ�Ķ�ʱ 1us �ľ��
     */
    void init(TIM_HandleTypeDef* htim);

    /**
     * @brief ��ȡ��ǰʱ���(��ϵͳ��������)����λ΢��
     */
    uint64_t getMicroseconds() const;

    /**
     * @brief ��ȡ��ǰʱ���(��ϵͳ��������)����λ����
     */
    uint64_t getMilliseconds() const;

    /**
     * @brief ��ȡ��ǰʱ���(��ϵͳ��������)����λ��
     */
    float getSeconds() const;

    static void overflowCallback(); // ��ʱ������ص�����
private:
    //���ֵ���
    TimeStamp();

    ~TimeStamp() = default;

    TimeStamp(const TimeStamp&) = delete;

    TimeStamp& operator = (const TimeStamp&) = delete;

    static TIM_HandleTypeDef* s_htim_; // ��ʱ�����

    static volatile uint64_t s_overflow_count_; // �������
};

#endif // __cplusplus

#endif // __BSP_TIMESTAMP_H