#include "APP_PID.h"



float PID_Position::pid_calc(float target, float feedback)
{
    uint32_t now = HAL_GetTick();
    if(last_tick_ != 0)
        dt = (now - last_tick_) / 1000.0f;
    else
        dt = 0.0f;
    last_tick_ = now;

    //仁王盾
    if(dt <= 0.0f || dt > 0.5f)
    {
        reset();
        return 0.0f;
    }

    // calc error
    float error = target - feedback;

    

    if(fabs(error) < params_.deadband)
        error = 0.0f;

    if(isFirst_)
    {
        error_last_ = error;
        feedback_last_ = feedback;
        isFirst_ = false;
    }


    // calc P
        P_Term = params_.kp * error;
    // calc I
    if(fabsf(error) < I_SeparaThreshold_)
    {
        I_Term += params_.ki * (error + error_last_) * dt / 2.0f; // 梯形积分
        if(params_.isIOutlimit == true)
            I_Term = constrain(I_Term, -params_.I_Outlimit, params_.I_Outlimit);
    }

    // calc D
    D_Term = -params_.kd * (feedback - feedback_last_) / dt; // 微分先行
    //D 项取的是输出微分而非误差微分

    //update history
    error_last_ = error;
    feedback_last_ = feedback;

    float output = P_Term + I_Term + D_Term;
    output = constrain(output, -params_.output_limit, params_.output_limit);

    output_ = output;

    return output_;
}

void PID_Position::set_params(const PID_Param_Config& params, float I_SeparaThreshold)
{
    params_ = params;
    I_SeparaThreshold_ = I_SeparaThreshold;
}


/* =================================================================================== */

//增量式

void PID_Incremental::calc_track_D(float expect)
{
    //二阶跟踪微分
    float fh = -td_ratio_ * td_ratio_ *(td_v1_ - expect) - 2.0f * td_v2_;

    td_v1_ += td_v2_ * dt;
    td_v2_ += fh * dt;
}

float PID_Incremental::pid_calc(float target, float feedback)
{
    uint32_t now = HAL_GetTick();
    if(last_tick_ != 0)
        dt = (now - last_tick_) / 1000.0f;
    else
        dt = 0.0f;
    

    //仁王盾
    if(dt <= 0.0f || dt > 0.5f)
    {
        reset();
        return 0.0f;
    }

    last_tick_ = now;

    //1.如果启用td
    float current_target = target;
    if(td_ratio_ > 0.0f)
    {
        calc_track_D(target);
        current_target = td_v1_;
    }

    //2.计算误差
    error_ = current_target - feedback;
    if(fabs(error_) < params_.deadband)
        error_ = 0.0f;

    if(isFirst_)
    {
        error_last_ = error_;
        error_earlier_ = error_;
        isFirst_ = false;
    }

    //3.计算PID



        //1.P
        P_Term = params_.kp * (error_ - error_last_);

        //2.I
        I_Term = params_.ki * error_;
        if(params_.isIOutlimit == true)
            I_Term = constrain(I_Term, -params_.I_Outlimit, params_.I_Outlimit);
        //3.D
        D_Term = params_.kd * (error_ - 2.0f * error_last_ + error_earlier_);




    output_ = P_Term + I_Term + D_Term + output_last_;
    output_ = constrain(output_, -params_.output_limit, params_.output_limit);

    error_earlier_ = error_last_;
    error_last_ = error_;
    output_last_ = output_;

    return output_;
}