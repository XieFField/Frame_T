#ifndef __APP_TOOL_H
#define __APP_TOOL_H

#include <stdint.h>
#include <cmath>

#ifdef __cplusplus
extern "C" {

    #define PI 3.14159265358979323846f
}
    

#endif


int binarySearch(const uint32_t arr[], uint8_t count, uint32_t key);

// ģ�庯������һ��ֵ��������С�����ֵ֮��
/**
 * @param value Ҫ���Ƶ�ֵ
 * @param min ��Сֵ
 * @param max ���ֵ
 */
template <typename T>
static inline T constrain(T value, T min, T max) 
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

#ifdef __cplusplus


#endif

#endif /* __APP_TOOL_H */
