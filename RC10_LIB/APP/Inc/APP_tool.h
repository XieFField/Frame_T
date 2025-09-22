#ifndef __APP_TOOL_H
#define __APP_TOOL_H

#include <stdint.h>
#include <cmath>

#ifdef __cplusplus
extern "C" {
    #include "arm_math.h"
    #define PI 3.14159265358979323846f
}
    

#endif

/**
 * @brief  ����������Ϊ��λ����
 * @param[in,out] M   ָ�����ʵ��
 * @note   Ҫ������Ƿ��� (numRows == numCols)
 */
void arm_set_identity_f32(arm_matrix_instance_f32 *M);

/**
 * @brief Perform binary search on a sorted array
 */
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

// 2D��ṹ��
struct Point2D {
    float x, y;
    float theta; // ��ת�Ƕȣ���λ����
    Point2D(float x = 0.0f, float y = 0.0f, float theta = 0.0f)
    : x(x), y(y), theta(theta) {} // ���캯��
};

// 3D��ṹ��
struct Point3D {
    float x, y, z;
    float roll, pitch, yaw; // ŷ���ǣ���λ����
    Point3D(float x = 0.0f, float y = 0.0f, float z = 0.0f, float roll = 0.0f, float pitch = 0.0f, float yaw = 0.0f)
    : x(x), y(y), z(z), roll(roll), pitch(pitch), yaw(yaw) {} // ���캯��
};


#ifdef __cplusplus


#endif

#endif /* __APP_TOOL_H */
