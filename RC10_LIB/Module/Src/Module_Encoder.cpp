#include "Module_Encoder.h"

void Encoder::update(uint16_t raw_value)
{
    if (!is_init_) 
    {
        offset_ = raw_value;
        last_ = raw_value;
        is_init_ = true;
        return;
    }

    int32_t delta = static_cast<int32_t>(raw_value) - static_cast<int32_t>(last_);

    if (delta > range_ / 2) 
        round_cnt_--;

    else if (delta < -static_cast<int32_t>(range_) / 2) 
        round_cnt_++;
    
    last_ = raw_value;
    int32_t total_encoder = round_cnt_ * static_cast<int32_t>(range_) +
                            static_cast<int32_t>(raw_value) - static_cast<int32_t>(offset_);
    total_angle_ = static_cast<float>(total_encoder) / (static_cast<float>(range_) / 360.0f);
    angle_ = total_angle_ - static_cast<float>(round_cnt_) * 360.0f;
}