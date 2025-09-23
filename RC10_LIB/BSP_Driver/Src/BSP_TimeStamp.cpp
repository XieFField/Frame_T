#include "BSP_TimeStamp.h"

// ��ʼ����̬��Ա����
TIM_HandleTypeDef* TimeStamp::s_htim_ = nullptr;
volatile uint64_t TimeStamp::s_overflow_count_ = 0;

// ˽�й��캯��
TimeStamp::TimeStamp() 
{
    // do nothing
}

// ��ȡ����ʵ��
TimeStamp& TimeStamp::getInstance() 
{
    static TimeStamp instance;
    return instance;
}

// ��ʼ��
void TimeStamp::init(TIM_HandleTypeDef* htim) 
{
    if (s_htim_ == nullptr && htim != nullptr)
    {
        s_htim_ = htim;
        // ������ʱ����ʹ�ܸ����ж�
        HAL_TIM_Base_Start_IT(s_htim_);
    }
}

// ����жϻص�
void TimeStamp::overflowCallback() 
{
    // ��鶨ʱ���Ƿ��ѳ�ʼ�����жϱ�־λ������
    if (s_htim_ != nullptr && __HAL_TIM_GET_FLAG(s_htim_, TIM_FLAG_UPDATE) != RESET) 
    {
        if (__HAL_TIM_GET_IT_SOURCE(s_htim_, TIM_IT_UPDATE) != RESET) 
        {
            __HAL_TIM_CLEAR_IT(s_htim_, TIM_IT_UPDATE);
            s_overflow_count_++;
        }
    }
}

// ��ȡ΢��
uint64_t TimeStamp::getMicroseconds() const 
{
    if (!s_htim_) return 0;

    uint64_t overflow_val;
    uint32_t counter_val;

    // --- �����ٽ��� ---
    uint32_t primask = __get_PRIMASK(); // ���浱ǰ���ж�״̬
    __disable_irq();                    // ��ֹ�����ж�

    // ���жϱ���ֹ������£���ȫ�ض�ȡ
    overflow_val = s_overflow_count_;
    counter_val = s_htim_->Instance->CNT;

    // ����ڶ�ȡCNT���Ƿ�պ÷���������жϣ���־λ��Ӳ����λ��
    // ����Ϊ�˴����ڶ�ȡCNT��˲�䣬����жϼ�����������δ����������
    if (__HAL_TIM_GET_FLAG(s_htim_, TIM_FLAG_UPDATE) != RESET)
    {
        // �����־λΪ1��˵��CNT�Ѿ���ת����overflow_val��û���ü�+1
        //֮ǰ������Ϊ����64λ��������»��������������
        // ���������ٴζ�ȡһ�Σ�ȷ���õ����µ�ֵ
        overflow_val = s_overflow_count_ + 1;
    }

    // --- �˳��ٽ��� ---
    if (!primask) // ���֮ǰ�ж��ǿ����ģ��ͻָ���
    {
        __enable_irq();
    }

    const uint64_t period = s_htim_->Instance->ARR + 1;
    return (overflow_val * period) + counter_val;
}

// ��ȡ����
uint64_t TimeStamp::getMilliseconds() const 
{
    return getMicroseconds() / 1000;
}

// ��ȡ��
float TimeStamp::getSeconds() const 
{
    return static_cast<float>(getMicroseconds()) * 1e-6f;
}


