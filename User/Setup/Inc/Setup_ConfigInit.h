/**
 * @file Setup_ConfigInit.h
 * @brief �������ó�ʼ��
 */

#ifndef SETUP_CONFIGINIT_H
#define SETUP_CONFIGINIT_H

#ifdef __cplusplus
#pragma once

extern "C" {
    #include "stm32h7xx_hal.h"
    #include "cmsis_os.h"
    #include "FreeRTOS.h"
    #include "task.h"
    #include "queue.h"
    #include "semphr.h"
    void ALL_Setup_ConfigInit(void);
}

#ifdef __cplusplus

#include <cstdint>
#include "BSP_CANFrame.h"
#include "BSP_RTOS.h"
#include "BSP_fdCAN_Driver.h"
#include "Motor_DJI.h"
#include "APP_tool.h"

fdCANbus CAN1_Bus;
DJI_Group group;



#endif // __cplusplus

#endif


#endif