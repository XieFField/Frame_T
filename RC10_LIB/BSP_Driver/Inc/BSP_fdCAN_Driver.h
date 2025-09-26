/**
 * @file    BSP_Driver/Src/BSP_fdCAN_Driver.h
 * @brief   BSP Driver for fdCAN communication
 * @author  XieFField
 * @version 1.0
 * 
 */

 /* |(*^��^*)/ ����ͨ���� */

#ifndef __BSP_FDCAN_DRIVER_H
#define __BSP_FDCAN_DRIVER_H

/*
    __________     _________    _   __   __              
   / ____/ __ \   / ____/   |  / | / /  / /_  __  _______
  / /_  / / / /  / /   / /| | /  |/ /  / __ \/ / / / ___/
 / __/ / /_/ /  / /___/ ___ |/ /|  /  / /_/ / /_/ (__  ) 
/_/   /_____/   \____/_/  |_/_/ |_/  /_.___/\__,_/____/  
                                                         
*/

#pragma once

#ifdef __cplusplus
extern "C" 
{
    #include "fdcan.h"
    #include<cstdint>
    #include "stm32h7xx_hal.h"
    #include "cmsis_os.h"
    #include "BSP_CanFrame.h"
}
#endif

#include "APP_tool.h"
#include "BSP_RTOS.h"
#include "Motor_Base.h" 
#include <cstring>

    class Motor_Base; // ǰ������

/** 
  * fdCANbus������һ��fdCAN����
  * - ÿһ·CAN����һ��ʵ��
  * - ����̬��motorList ���8��
  * - ISR����ֻ����push ԭʼ�� CanFrame �� rxQueue_
  * - rxTask_ ����� rxQueue_ pop ���ַ��� motor -> motor->updateFeedback()
  * - schedulerTask_ ÿ 1ms ���� motorList���ռ� packCommand() ������ sendFrame()���Ӷ�ʵ��1kHz�ķ���Ƶ��
  * - ������
  * @attention ���಻���κξ���ı��Ľ�����ȫ�����������
 */
class fdCANbus;

extern "C" void fdcan_global_rx_isr(FDCAN_HandleTypeDef* hfdcan);
extern "C" void fdcan_global_scheduler_tick_isr();

class fdCANbus{
public:
    // ���������ÿ·��
    static constexpr size_t MAX_MOTORS = 8;

    fdCANbus(FDCAN_HandleTypeDef* hfdcan, uint32_t bus_id): hfdcan_(hfdcan)
    , bus_id_(bus_id)
    , rxQueue_(64)                // ���г��ȿɵ���
    , rxTask_(this)
    , schedulerTask_(this)
    {
        for (std::size_t i = 0; i < MAX_MOTORS; ++i) 
            motorList_[i] = nullptr;

        tx_mutex_ = xSemaphoreCreateMutex(); //����������
        schedSem_ = xSemaphoreCreateBinary();
    }

    ~fdCANbus()
    {
        // ����ɾ��ͨ����ϵͳͣ���������������Զ�ɾ��
    }

    /**
     * @brief ��ʼ�� (�˲�/�ж�/��������)
     */
    void init();

    /**
     * @brief ��̬ע����
     */
    bool registerMotor(Motor_Base* m);

    /**
     * @brief ����
     * @param cf Ҫ���͵�֡
     * @return true ���ͳɹ���false ����ʧ��
     * @attention ��scheduler����
     */
    bool sendFrame(const CanFrame& cf);

    /**
     * @brief ��ISR�н������ݲ�������ն���
     * @param cf Ҫ���յ�֡
     */
    bool pushRxFromISR(const CanFrame& cf, BaseType_t* pxHigherPriorityTaskWoken);

    uint8_t getbusID() const{return bus_id_;}

    // ���ж�/ISR �е��ã����Ѹ� fdCANbus �� scheduler task
    // pxHigherPriorityTaskWoken ���Դ� ISR ���벢���� portYIELD_FROM_ISR
    //���˱�ķ�ʽʵ�֣�����Ѿ������ˣ��պ���Ҫȡ����ʱ�жϻ��ѣ����Դ�����
    //static void notifySchedulerFromISR(BaseType_t* pxHigherPriorityTaskWoken);

    FDCAN_HandleTypeDef* getFDCANHandle() const { return hfdcan_; }
    
    SemaphoreHandle_t tx_mutex_; // ���ͻ�����
    SemaphoreHandle_t schedSem_; // ���������ź���
protected:
    
    FDCAN_HandleTypeDef* hfdcan_; //protected character
    uint32_t bus_id_;

    /**
     * @brief Rx��������
     */
    void rxTaskbody();

    /**
     * @brief 1kHz ������������
     */
    void schedulerTaskbody();

    // Ĭ��ƥ�亯��������� motor �� override motor.matchesFrame��
    static bool matchesFrameDefault(const CanFrame& cf, uint32_t targetId, bool isExt);

    Motor_Base * motorList_[MAX_MOTORS];// ����б�

    RtosQueue<CanFrame> rxQueue_;

    /**
     * @brief ����������
     */
    class RxTask : public RtosTask{
    public:
        explicit RxTask(fdCANbus* parent);
    protected:
       virtual void run() override;
    private:
        fdCANbus* parent_;
    };

    /**
     * @brief ������������
     */
    class SchedTask : public RtosTask {
    public:
        explicit SchedTask(fdCANbus* parent);
    protected:
       virtual void run() override;
    private:
        fdCANbus* parent_;
    };

    // ��Աʵ��
    RxTask rxTask_;
    SchedTask schedulerTask_;

    int HAL_FDCAN_Start_ERROR = 0; // ��¼ HAL_FDCAN_Start �Ƿ�ɹ�

    int HAL_FDCAN_ActivateNotification_ERROR = 0; // ��¼ HAL_FDCAN_ActivateNotification �Ƿ�ɹ�
};



#endif /* __BSP_FDCAN_DRIVER_H */