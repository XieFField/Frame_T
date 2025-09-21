/**
 * @file   Module_Encoder.h
 * @author XieFField
 * @brief  ����������ת�ӽǶȡ���·��
 * @version 1.0
 */

#ifndef __ENCODER_H
#define __ENCODER_H

#pragma once
#ifdef __cplusplus
extern "C"{}
#endif

#ifdef __cplusplus

#include <cstdint>
#include <cmath>
#include <cstddef>
#include "APP_tool.h"

/*����ֻ����еת�ӽǶȼ��㣬�ǵ����ʵ�Ƕ�*/
class Encoder{
public:
    Encoder(uint16_t range = 8192): range_(range){}

    /**
     * @brief ���±�����ԭʼֵ�����㵱ǰ�ǶȺ���·��
     * @param raw_value ������ԭʼֵ
     */
    void update(uint16_t raw_value);

    float getAngle() const { return angle_; }

    float getTotalAngle() const { return total_angle_; }

    float getAngle_redian() const { return angle_ * (PI / 180.0f); }

    float getTotalAngle_redian() const { return total_angle_ * (PI / 180.0f); }
private:

    float angle_ = 0.0f; //��ǰ�Ƕ�
    float total_angle_ = 0.0f; //�ܽǶ�

    bool is_init_ = false;
    uint16_t offset_ = 0;
    uint16_t last_ = 0;
    int32_t round_cnt_ = 0;

    uint16_t range_;
};

#endif

#endif