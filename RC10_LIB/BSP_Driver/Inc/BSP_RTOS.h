/***************************************************************************
 *                                                                         *
 *               RRRRR    TTTTT    OOOOO    SSSS                           *
 *               R    R     T      O    O   S                              *
 *               R    R     T      O    O   SSSS                           *
 *               RRRRR      T      O    O       S                          *
 *               R   R      T      O    O       S                          *
 *               R    R     T      OOOOO    SSSS                           *
 *                                                                         *
 ***************************************************************************/

/**
 * @file BSP_RTOS.h
 * @author XieFField
 * @version 1.0
 * @brief freeRTOS������ȸ���
 * @attention hpp�ļ�ֻ����C++�ļ�����
 */
//111




#ifndef BSP_RTOS_H
#define BSP_RTOS_H

#pragma once


#ifdef __cplusplus

extern "C" 
{
    #include "stm32h7xx_hal.h"
    #include "cmsis_os.h"
    #include "FreeRTOS.h"
    #include "task.h"
    #include "queue.h"
    #include "semphr.h"
    #include <cstdint>
}

enum class TaskPrio {
    LOW = tskIDLE_PRIORITY+1,
    NORMAL = tskIDLE_PRIORITY+3,
    HIGH = tskIDLE_PRIORITY+5
};

/**
 * @brief FreeRTOS�����װ��
 *       �̳и��ಢʵ�� run() ���������ɴ���һ�� FreeRTOS ����
 *       �û��ڴ���������������Ҫʹ�� start() ����ע�Ტ��������
*/

class RtosTask{
public:
    /**
     * @brief ���캯��
     * @param name ������
     * @param period �������� (Ticks)��
     *               - period > 0 (Ĭ�� = 1): ����һ�������������û���ʵ�� loop()��
     *               - period = 0           : ����һ���¼����������û�����д run()��
     */
    explicit RtosTask(const char* name, TickType_t period = 1) 
        : name_(name), handle_(nullptr), period_(period) {}

    virtual ~RtosTask() 
    {
        // ���ǲ��ڴ˴���������Զ�ɾ�����ˡ�ϵͳֹͣ���Զ�ɾ��
    }

    /**
     * @brief ������������ע������ FreeRTOS
     * @param priority �������ȼ�
     * @param stackWords ����ջ��С����λΪ�֣�4�ֽڣ�
     * @param arg ���ݸ�����Ĳ���
     * @return true �����ɹ���false ����ʧ��
     *         �����û�����ͨ��bool����ֵ�ж������Ƿ񴴽��ɹ�
     *         ������ʧ�ܣ�handle_������Ϊnullptr
     */
    bool start(UBaseType_t priority, uint32_t stackWords = 128, void* arg = nullptr) 
    {
        BaseType_t res = xTaskCreate(&RtosTask::taskEntry,
                                     name_,
                                     (configSTACK_DEPTH_TYPE)stackWords,
                                     this,
                                     priority,
                                     &handle_);
        return res == pdPASS;
    }

    /**
     * @brief ��ȡ������
     * @return ������
     */
    TaskHandle_t handle() const { return handle_; }

protected:
    
    /**
     * @brief [�¼���������] ����������
     *        ����������¼������� (period=0)������д�˺�����
     *        �������ڲ����� for(;;) ���������á�
     */
    virtual void run() 
    { 
        // �ṩһ��Ĭ�ϵĿ�ʵ��
        for(;;)
        {
            osDelay(1000); // Ĭ��ÿ������һ�Σ���ֹ��ѭ��
        }
    }

    /**
     * @brief [����������] ��ѭ���塣
     *        ��������������Ե� (period>0)������д�˺�����
     */
    virtual void loop() 
    {
        // �ṩһ��Ĭ�ϵĿ�ʵ��
    }

private:

    /**
     * @brief ������ں���
     */
    static void taskEntry(void* pvParameters) 
    {
        RtosTask* self = static_cast<RtosTask*>(pvParameters);
        if (self->period_ > 0) 
        {
            // ����������ģʽ
            for (;;) 
            {
                self->loop();
                osDelay(self->period_);
            }
        } 
        else  // �¼�����/һ��������ģʽ
            self->run();
         // ��� run() ���ػ�ѭ���˳���ɾ������
        vTaskDelete(nullptr);
    }

    const char* name_;
    TaskHandle_t handle_;
    TickType_t period_;
};

/**
 * @brief FreeRTOS���з�װ��
 *        �ڹ��캯��
 * @attention ���Ҫ���ж��з��Ͷ��У���ʹ�� sendFromISR() ����
 */
/*==============================================================
 * RtosQueue: ���з�װ��
 * �÷���(��'��'��)
 *   RtosQueue<int> queue(8); // ����һ������Ϊ8��int���Ͷ���
 *   queue.send(123);         // ��������
 *   int value; 
 *   queue.recv(value); // ��������
 *      (*^��^*)

*/
template<typename T>
class RtosQueue{
public:
    /**
     * @brief ���캯��������һ������
     * @param len ���г��ȣ�Ĭ��8
     */
    explicit RtosQueue(size_t len = 8) {queue_ = xQueueCreate((UBaseType_t)len, sizeof(T));}

    ~RtosQueue() 
    {
        // ɾ������
        if (queue_) 
            vQueueDelete(queue_);
    }

    /**
     * @brief �������ݵ�����
     * @param item Ҫ���͵�����
     * @param ticks �ȴ�ʱ�䣬Ĭ�ϲ��ȴ�
     * @return true ���ͳɹ���false ����ʧ��
     */
    bool send(const T& item, TickType_t ticks = 0) 
    {
        if (!queue_) 
            return false;
        return xQueueSend(queue_, &item, ticks) == pdTRUE;
    }

    /**
     * @brief ��ISR�з������ݵ�����
     * @param item Ҫ���͵�����
     * @param pxHigherPriorityTaskWoken ����������ݺ��и������ȼ������񱻻��ѣ���ΪpdTRUE
     * @return true ���ͳɹ���false ����ʧ��
     */
    bool sendFromISR(const T& item, BaseType_t* pxHigherPriorityTaskWoken) 
    {
        if (!queue_) 
            return false;
        return xQueueSendFromISR(queue_, &item, pxHigherPriorityTaskWoken) == pdTRUE;
    }

    /**
     * @brief �Ӷ��н�������
     * @param out �������ݵĴ��λ��
     * @param ticks �ȴ�ʱ�䣬Ĭ�����޵ȴ�
     * @return true ���ճɹ���false ����ʧ��
     *       ������Ϊ���ҵȴ�ʱ�䵽�����false
     */
    bool recv(T& out, TickType_t ticks = portMAX_DELAY) 
    {
        if (!queue_) 
            return false;
        return xQueueReceive(queue_, &out, ticks) == pdTRUE;
    }

    /**
     * @brief ��ȡ�ײ���о��
     * @return ���о��
     */
    QueueHandle_t raw() const { return queue_; }

private:
    QueueHandle_t queue_;
};


#endif


#endif