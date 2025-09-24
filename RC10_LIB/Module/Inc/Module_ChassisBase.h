/**
 * @file    Module_ChassisBase.h
 * @author  XieFField
 * @brief   ����ģ�����
 *          ����һ��������˶�ѧģ�ͣ������ٶȵķֽ⣨��⣩�ͺϳɣ����⣩��
 *          - �������κε��ʵ����ֻ�������������Ŀ�ꡣ
 *          - ����ϵ����ѭ���ֶ���yaw��ʱ��Ϊ����
 * @version 1.0
 */

/*
   ______   __                                _            ____                         
  / ____/  / /_     ____ _   _____   _____   (_)  _____   / __ )   ____ _   _____   ___ 
 / /      / __ \   / __ `/  / ___/  / ___/  / /  / ___/  / __  |  / __ `/  / ___/  / _ \
/ /___   / / / /  / /_/ /  (__  )  (__  )  / /  (__  )  / /_/ /  / /_/ /  (__  )  /  __/
\____/  /_/ /_/   \__,_/  /____/  /____/  /_/  /____/  /_____/   \__,_/  /____/   \___/ 
                                                                                        
*/

#ifndef __MODULE_CHASSISBASE_H
#define __MODULE_CHASSISBASE_H

#ifdef __cplusplus
extern "C" {
#endif
#include "arm_math.h"
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include <cstdint>
#include "APP_tool.h"
#include "APP_CoordConvert.h"
#include "Motor_Base.h"
#include "BSP_TimeStamp.h"
#endif// __cplusplus

#ifdef __cplusplus

/**
 * @brief ����ģ�����
 * @details
 *          �����κε����඼��̳д��ࡣ         
 */
template <std::size_t WheelCount>
class Chassis_Base{
public:
    Chassis_Base(float wheel_radius, float max_wheel_rpm);
    ~Chassis_Base(){}

    void setRobotSpeed(const Robot_Twist& twist); // ���û������ٶȣ�����������ϵ��

    void setWorldSpeed(const Robot_Twist& twist); // ���������ٶȣ���������ϵ��

    void update(); // ��������Ӧ�õ����

    virtual void updateKinematics() = 0; // �����˶�ѧ��������������

    virtual void inverseKinematics(const Robot_Twist& twist) = 0; // ��⣬����Ŀ���ٶȼ�������

    virtual void forwardKinematics() = 0; // ���⣬�������ټ���������ٶ�


    void updateAngleData(const Angle_Twist& angle_twist) { angle_twist_ = angle_twist; } // ���½��ٶ�����

    float getWheelTargetRPM(uint8_t wheel_index) const
    {
        if(wheel_index >= WheelCount)
            return 0.0f;
        return wheele_target_rpm_[wheel_index];
    }

    Robot_Twist getRobotSpeed() const { return robot_twist_; } // ��ȡ�������ٶȣ�����������ϵ��
    Robot_Twist getWorldSpeed() const { return world_twist_; } // ��ȡ�������ٶȣ���������ϵ��
    float getdt() const { return dt_; } // ��ȡʱ���
 
    bool registerWheelMotor(uint8_t wheel_index, Motor_Base* motor) // ע�����ӵ��
    {
        if(wheel_index >= WheelCount) 
            return false;
        wheels_[wheel_index] = motor;
        return true;
    }

    void reset_AccLimitStatus(bool reset) { accel_Limit_ = reset; } // ���õ����߼��ٶ��޷���
    void reset_AccValue(float reset) {accel_value_ = reset;}; // ���õ����߼��ٶ�ֵ
protected:

    Robot_Twist robot_twist_ = {0}; // ����������ϵ��ǰ�ٶ�
    Robot_Twist world_twist_ = {0}; // ��������ϵ��ǰ�ٶ�

    Robot_Twist robot_target_twist_ = {0}; // ����������ϵĿ���ٶ�
    Robot_Twist world_target_twist_ = {0}; // ��������ϵĿ���ٶ�

    Angle_Twist angle_twist_ = {0}; // �Ӵ������õ��Ľ��ٶȡ��Ƕ�����

    

    bool accel_Limit_ = false; // �Ƿ����ü��ٶ��޷�
    float accel_value_ = 0.0f; // ��ǰ�߼��ٶ�ֵ

    const float wheel_radius_;    // ���Ӱ뾶 (m)
    const float max_wheel_rpm_;   // �������RPM
    const float max_wheel_speed_; // ����������ٶ� (m/s)

    // ʱ��������ڼ��ٶ�б��
    float last_update_time_s_ = 0.0f; //��λ����

    float wheele_target_rpm_[WheelCount] = {0}; // �洢�������ĸ���Ŀ��RPM

    Motor_Base* wheels_[WheelCount] = {nullptr}; // ���ӵ��ָ������
    float dt_ = 0.0f; //����ʱ���
};

#endif // __cplusplus

#endif // __MODULE_CHASSISBASE_H