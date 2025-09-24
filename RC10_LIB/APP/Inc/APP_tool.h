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

//б�º���
void ramp(float target, float& current, float max_change_rate, float dt);

//����ת��Ϊ�ǶȺ���
float rad_to_deg(float rad);

//�Ƕ�ת��Ϊ���Ⱥ���
float deg_to_rad(float deg);

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

struct Robot_Twist{
    float vx;
    float vy;
    float vz;

    float yaw_rate;
    float pitch_rate;
    float roll_rate;

    Robot_Twist(float vx = 0.0f, float vy = 0.0f, float vz = 0.0f, 
                 float yaw_rate = 0.0f, float pitch_rate = 0.0f, float roll_rate = 0.0f)
    : vx(vx), vy(vy), vz(vz), yaw_rate(yaw_rate), pitch_rate(pitch_rate), roll_rate(roll_rate) {}
};

struct Angle_Twist
{
    float yaw_rate;
    float pitch_rate;
    float roll_rate;

    float yaw_angle;
    float pitch_angle;
    float roll_angle;

    Angle_Twist(float yaw_rate = 0.0f, float pitch_rate = 0.0f, float roll_rate = 0.0f,
                 float yaw_angle = 0.0f, float pitch_angle = 0.0f, float roll_angle = 0.0f)
    : yaw_rate(yaw_rate), pitch_rate(pitch_rate), roll_rate(roll_rate),
      yaw_angle(yaw_angle), pitch_angle(pitch_angle), roll_angle(roll_angle) {}
};


#ifdef __cplusplus


#endif

#endif /* __APP_TOOL_H */
