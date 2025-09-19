/**
 * @file Motor_Base.h
 * @author XieFField
 * @brief 电机基类定义
 * @version 1.0
 * @date 2025-09-16
 */
#ifndef MOTOR_BASE_H
#define MOTOR_BASE_H

#pragma once
#ifdef __cplusplus

#endif // __cplusplus
#include "BSP_CanFrame.h"
#include <cstdint>
#include <cstddef>
class fdCANbus; // 前置声明

class Motor_Base {
public:
    Motor_Base(uint32_t id, bool isExt, fdCANbus* bus);
    virtual ~Motor_Base() = default;

    // 目标设定
    virtual void setTargetRPM(float rpm_set);
    virtual void setTargetCurrent(float current_set);
    virtual void setTargetAngle(float angle_set);
    virtual void setTargetTotalAngle(float totalAngle_set);

    // 反馈读取
    virtual float getRPM() const;
    virtual float getCurrent() const;
    virtual float getAngle() const;
    virtual float getTotalAngle() const;

    // 抽象：打包命令 -> 返回需要发送的帧数
    virtual std::size_t packCommand(CanFrame outFrames[], std::size_t maxFrames) = 0;

    // 抽象：解析反馈帧
    virtual void updateFeedback(const CanFrame& cf) = 0;

    // 帧匹配（默认简单 ID/Ext 匹配）
    virtual bool matchesFrame(const CanFrame& cf) const;

    fdCANbus* bus() const { return bus_; }
protected:
    uint32_t id_;
    bool isExtended_;
    fdCANbus* bus_;

    // 目标/状态量
    float target_rpm_ = 0.0f;
    float target_current_= 0.0f;
    float target_angle_ = 0.0f;
    float target_totalAngle_ = 0.0f;

    float rpm_ = 0.0f;
    float current_ = 0.0f;
    float angle_ = 0.0f;
    float totalAngle_ = 0.0f;

};




#endif // MOTOR_BASE_H
