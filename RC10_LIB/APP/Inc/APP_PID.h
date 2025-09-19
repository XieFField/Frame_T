/**
 * @file APP_PID.h
 * @author XieFField
 * @brief PID ������
 * @version 1.0
 * @date 2025-09-17
 */

/*
    �˴������㷨�Ϻܴ�̶Ƚ���˽����Ĵ��룬�ڴ˱�ʾ��л
    ֮����û��ֱ��ʹ�ã�����Ϊ�������ѧ�߼���Ȼ���ʣ�����
    ������ͽṹ������C++�����ǰ��൱���˽ṹ����ʹ�ã�
    ����Ϊ�ƻ��˷�װ�ԣ�������׳��
    =================================================
    �������㷨�ϣ�����Ϊ��Щ�ط����ԸĽ�����Ҫ�������ڲ���
    ʱ��ļ����ϣ��Լ����ֿ����͵��߼������������ֱ����
    ��������е���ڼ�������������ϵͳ�𵴡�
    =================================================
*/

#ifndef __APP_PID_H
#define __APP_PID_H

#pragma once

#ifdef __cplusplus

extern "C" {

#include "stm32h7xx_hal.h" 

}
#include <cstdint>
#include <stdbool.h>
#include <cmath>
#include "APP_tool.h"
/*
    ______ _            __  _____  ____________     ______
   /  _/ /( )_____     /  |/  /\ \/ / ____/ __ \   / / / /
   / // __/// ___/    / /|_/ /  \  / / __/ / / /  / / / / 
 _/ // /_  (__  )    / /  / /   / / /_/ / /_/ /  /_/_/_/  
/___/\__/ /____/    /_/  /_/   /_/\____/\____/  (_|_|_)   
*/                                                          

typedef struct {
    float kp;
    float ki;
    float kd;
    float I_Outlimit; // �����޷�
    bool isIOutlimit; // �Ƿ����û����޷�
    float output_limit;   // ����޷�
    float deadband;      // ����
} PID_Param_Config;

// λ��ʽ PID �����������������λ��֣�΢�����У����ַ���ĸĽ�ʽPID
class PID_Position {
public:
    /**
     * @brief ���캯��
     * @param pid_Param_Config PID �������ýṹ��
     * @param I_SeparaThreshold ���ַ�����ֵ����������ֵ���ڸ�ֵʱ��������ۼ�
     */
    PID_Position(PID_Param_Config params = {0.0f, 0.0f, 0.0f, 0.0f, false, 0.0f, 0.0f}, 
        float I_SeparaThreshold = 0.0f)
        : params_(params), I_SeparaThreshold_(I_SeparaThreshold) 
        {
            reset();
        }

    bool max_output = false; //���ֿ�����

    /**
     * @brief ����PID���
     * @param target Ŀ��ֵ
     * @param feedback ��ǰ����ֵ
     * @return PID�������
     */
    float pid_calc(float target, float feedback);
    

    /**
     * @brief ����PID���������ڲ�״̬
     *        ��Ҫ�������û��������ʷֵ��
     */
    void reset()
    {
        I_Term = 0.0f;
        P_Term = 0.0f;
        D_Term = 0.0f;
        output_ = 0.0f;
        error_last_ = 0.0f;
        feedback_last_ = 0.0f;
        
    }

    /**
     * @brief ��̬�޸�PID����
     * @param params �µ�PID����
     */
    void set_params(const PID_Param_Config& params, float I_SeparaThreshold);

    float get_dt() const { return dt; } // ��ȡ����ʱ��

private:
    float I_Term = 0;			/* ��������� */
    float P_Term = 0;			/* ��������� */
    float D_Term = 0;			/* ΢������� */
    float output_ = 0.0f;     // ���ֵ
    float dt = 0.0f;         // ����ʱ�䣬��λ��
    PID_Param_Config params_;
    float I_SeparaThreshold_;

    float error_last_ = 0.0f;       // �ϴ����
    float feedback_last_ = 0.0f;    // �ϴη���ֵ

    uint32_t last_tick_ = 0;
    bool isFirst_ = true; // �Ƿ�Ϊ��һ�μ���
};

// ����ʽ PID ������ ����΢�ָ�����
class PID_Incremental {
public:
    /** 
     * @brief ���캯��
     * @param params PID �������ýṹ��
     * @param td_ratio ΢�ָ������������ӣ���Χ0.0~1.0��0��ʾ������΢�ָ�����.td_ratioԽ�����Խ�졣
     *
    */
    PID_Incremental(PID_Param_Config params = {0.0f, 0.0f, 0.0f, 0.0f, false, 0.0f, 0.0f}, float td_ratio = 0.0f)
        : params_(params), td_ratio_(td_ratio) 
    {
        reset();
    }

        /**
         * @brief ��������ʽPID���
         * @param target Ŀ��ֵ
         * @param feedback ��ǰ����ֵ
         * @return PID�������
         */
        float pid_calc(float target, float feedback);

        /**
         * @brief ����PID���������ڲ�״̬
         *        ��Ҫ�������û��������ʷֵ��
         */
        void reset()
        {
            error_ = 0.0f;
            error_last_ = 0.0f;
            error_earlier_ = 0.0f;
            output_last_ = 0.0f;
            td_v1_ = 0.0f;
            td_v2_ = 0.0f;
            
        }

        /**
         * @brief ��̬�޸�PID����
         * @param params �µ�PID����
         */
        void set_params(const PID_Param_Config& params, float td_ratio)
        {
            params_ = params;
            td_ratio_ = td_ratio;
        }

        
private:
    void calc_track_D(float expect); //΢�ָ�����
    bool isFirst_ = true; // �Ƿ�Ϊ��һ�μ���

    float error_ = 0.0f, error_last_ = 0.0f, error_earlier_ = 0.0f; // ��ǰ���ϴ������ϴ����
    float output_ = 0.0f;       // ��ǰ�����
    float output_last_ = 0.0f;  // �ϴ������

    PID_Param_Config params_;

    float I_Term = 0.0f; // ���������
    float P_Term = 0.0f; // ���������
    float D_Term = 0.0f; // ΢�������

    float td_ratio_ = 0.0f; // track_D��������

    float td_v1_ = 0.0f; //���ٵ�Ŀ��λ��
    float td_v2_ = 0.0f; //���ٵ�Ŀ���ٶ�


    uint32_t last_tick_ = 0;

    float dt = 0.0f; // ����ʱ�䣬��λ��
};


#endif

#endif