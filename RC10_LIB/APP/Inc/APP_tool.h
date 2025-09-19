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

// 模板函数：将一个值限制在最小和最大值之间
/**
 * @param value 要限制的值
 * @param min 最小值
 * @param max 最大值
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
