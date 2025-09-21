/**
 * @file frame_demo.h
 * @author XieFField
 * @brief º‹ππ≤‚ ‘
 */

#ifndef __FRAME_DEMO_H
#define __FRAME_DEMO_H

#pragma once

#ifdef __cplusplus


#include "BSP_RTOS.h"
#include "BSP_fdCAN_Driver.h"
#include "Setup_ConfigInit.h"

extern "C"
{
   
    #include "cmsis_os.h"
}

class FrameDemo : public RtosTask
{
public:
    FrameDemo() : RtosTask("FrameDemo", 1) {}
    void init();
    void loop() override;
    
};

class CanTest : public fdCANbus
{
public:
    CanTest(FDCAN_HandleTypeDef* hfdcan, uint32_t bus_id) : fdCANbus(hfdcan, bus_id) {}
    ~CanTest(){}
    
};

#endif // __cplusplus

#endif