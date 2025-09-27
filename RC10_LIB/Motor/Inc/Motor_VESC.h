/**
 * @file Motor_VESC.h
 * @author XieFField
 * @brief VESC�����
 * @version 1.0
 */

#ifndef __MOTOR_VESC_H
#define __MOTOR_VESC_H

#pragma once

#ifdef __cplusplus
extern "C"{
#endif
#include <stdint.h>
#ifdef __cplusplus
}
#endif



#ifdef __cplusplus

#include "Motor_Base.h"
#include "BSP_CanFrame.h"
#include "APP_tool.h"
#include "BSP_fdCAN_Driver.h"
#include "BSP_RTOS.h"

// VESC CAN��������
typedef enum {
    CAN_CMD_SET_DUTY = 0,
    CAN_CMD_SET_CURRENT = 1,
    CAN_CMD_SET_CURRENT_BRAKE = 2,
    CAN_CMD_SET_ERPM = 3,
    CAN_CMD_SET_POS = 4,
    CAN_CMD_SET_POS_SPD_LIM = 5,
    CAN_CMD_SET_CURRENT_REL = 6,
    CAN_CMD_SET_CURRENT_BRAKE_REL = 7,
    CAN_CMD_SET_CURRENT_HANDBRAKE = 8,
    CAN_CMD_SET_CURRENT_HANDBRAKE_REL = 9
} VESC_CAN_CMD;

// VESC CAN������������
typedef enum {
    CAN_PACKET_STATUS_1 = 9,
    CAN_PACKET_STATUS_2 = 14,
    CAN_PACKET_STATUS_3 = 15,
    CAN_PACKET_STATUS_4 = 16,
    CAN_PACKET_STATUS_5 = 27,
    CAN_PACKET_STATUS_6 = 28
} VESC_CAN_PACKET_ID;

typedef enum{
    SET_NULL,   //��
    SET_eRPM,   //����ת�ٱջ�ģʽ
    SET_CURRENT,//�����ջ�ģʽ
    SET_DUTY,   //ռ�ձ�ģʽ�����Ƽ�ʹ��
    SET_POS,    //λ��ģʽ�����Ƽ�ʹ��
    SET_BRAKE,  //ɲ��
}VESC_MODE;

class VESC_Motor : public Motor_Base {
public:
    /**
     * @param id ���CAN ID
     * @param bus ָ��fdCANbusʵ����ָ��
     * @param poles �����������Ĭ��21(u8)
     */
    VESC_Motor(uint32_t id, fdCANbus* bus, float poles);
    ~VESC_Motor(){};

    void update() override
    {
        //do nothing 
    }

    void setTargetCurrent(float current_set) override;

    /**
     * @brief ����Ŀ��ת�٣���λRPM (ע�ⲻ��eRPM����RPM)
     */
    void setTargetRPM(float rpm_set) override;
    void setTargetAngle(float angle_set) override{};
    void setTargetTotalAngle(float totalAngle_set) override;

    void setBrake(float brake_current);

    void setDuty(float duty);


    std::size_t packCommand(CanFrame outFrames[], std::size_t maxFrames) override;

    void updateFeedback(const CanFrame& cf) override;

    bool matchesFrame(const CanFrame& cf) const override
    {
        if (!cf.isextended || (cf.ID & 0xFF) != motor_id_)
            return false;

        uint8_t packet_type = (cf.ID >> 8) & 0xFF;
        return packet_type == CAN_PACKET_STATUS_1; // ��ǰֻ����STATUS_1
    }

    float get_GearRatio() const override { return GEAR_RATIO; }

    void reset_GearRatio(float reset_value){GEAR_RATIO = reset_value;}

    int32_t RPM_to_eRPM(float rpm) const
    {
        return static_cast<int32_t>(rpm * poles_);
    }

    float eRPM_to_RPM(int32_t eRPM) const
    {
        return static_cast<float>(eRPM) / poles_;
    }

    /**
     * @brief ���ݵ�ǰ����ģʽ�������������Ʋ����������ͻ
     */
    void reset_otherParam()
    {
        switch(mode_)
        {
            case SET_NULL:
                target_duty_ = 0.0f;
                target_totalAngle_ = 0.0f;
                target_current_ = 0.0f;
                target_rpm_ = 0.0f;
                target_brake_current_ = 0;
                break;
            case SET_eRPM:
                target_duty_ = 0.0f;
                target_brake_current_ = 0;
                target_current_ = 0.0f;
                target_totalAngle_ = 0.0f;

                break;
            case SET_CURRENT:
                target_duty_ = 0.0f;
                target_brake_current_ = 0;
                target_rpm_ = 0.0f;
                target_totalAngle_ = 0.0f;
                break;
            case SET_POS:
                target_duty_ = 0.0f;
                target_brake_current_ = 0;
                target_current_ = 0.0f;
                target_rpm_ = 0.0f;
                break;
            case SET_BRAKE:
                target_duty_ = 0.0f;
                target_current_ = 0.0f;
                target_rpm_ = 0.0f;
                target_totalAngle_ = 0.0f;
                break;
            default:
                break;
        }
    }
private:
    int32_t target_eRPM_ = 0; //����ת��
    float GEAR_RATIO = 1.0f; // VESCһ���޼�����
    VESC_MODE mode_ = SET_NULL;
    float poles_; // ��������Ĭ��21
    float target_duty_ = 0.0f; //ռ�ձ�  -1.0~1.0
    float duty_ = 0.0f; //��ǰռ�ձ�
    int32_t eRPM_ = 0; 
    int16_t target_brake_current_ = 0; //ɲ������
    uint8_t id_check_; //�ش�id�����ڸ��û��ֱ�motor_id_�͵��id�Ƿ�һ��
};


#endif // __cplusplus


#endif// __MOTOR_VESC_H