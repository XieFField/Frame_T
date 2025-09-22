/**
 * @file APP_CoordConvert.cpp
 * @author Zhan Hongli (Original), XieFField (Refactored)
 * @brief ʹ�� CMSIS-DSP (arm_math.h) �Ż����������任�⡣
 * @version 2.0
 * @brief
 * �ÿ��ṩ��2D��3D����α任���ܣ����罫�����״������ϵת���������˵��̵���������ϵ��
 * ���о��������ʹ��ARM�ٷ���CMSIS-DSP����м��٣��Ի��������ܡ�
 * 
 * @note ���нǶȲ����ĵ�λ��Ϊ **���� (radians)**��
 */
#ifndef __APP_COORDCONVERT_H
#define __APP_COORDCONVERT_H

#include "arm_math.h"
#include "APP_tool.h" // ���� Point2D �� Point3D �ṹ�嶨��



/**
 * @brief ʹ�� CMSIS-DSP ��2D��α任�ࡣ
 * @details
 * ���ڴ����άƽ���ϵ�ƽ�ƺ���ת��
 * ���磺��һ����װ�ڻ������ϡ���һ��ƫ�ƺ���ת�ǶȵĴ����������꣬ת���������˵���������ϵ��
 */
class HomogeneousTransform2D {
public:
    /**
     * @brief Ĭ�Ϲ��캯��������һ����λ�任����ƽ�ơ�����ת����
     */
    HomogeneousTransform2D();

    /**
     * @brief ���캯�������ݸ�����λ�˴���һ���任��
     * @param pose ����ƽ�ƺ���ת��Ϣ��2D�㡣
     */
    HomogeneousTransform2D(const Point2D& pose);

    /**
     * @brief ����һ��ȫ�µı任�����ǵ�ǰֵ��
     * @param pose ����ƽ�ƺ���ת��Ϣ��2D�㡣
     */
    void setTransform(const Point2D& pose);

    /**
     * @brief �����ñ任��ƽ�Ʋ��֣���ת���ֱ��ֲ��䡣
     * @param translation ����x, yƽ������2D�㣨��������theta��Ա����
     */
    void setTranslation(const Point2D& translation);

    /**
     * @brief �����ñ任����ת���֣�ƽ�Ʋ��ֱ��ֲ��䡣
     * @param theta_rad ��Z�����ת�Ƕȣ���λ�����ȣ���
     */
    void setRotation(float theta_rad);

    /**
     * @brief ���˱任Ӧ����һ��2D�㡣
     * @param point Ҫ�任��ԭʼ�㡣
     * @return Point2D �任����µ㡣
     */
    Point2D apply(const Point2D& point) const;

    /**
     * @brief ����˷�������ǰ�任����һ���任��ˡ�
     * @details
     * ���൱�ڽ������任���ӡ����� T_A_to_C = T_A_to_B.multiply(T_B_to_C)��
     * @param other Ҫ�����ұߵı任����
     * @return HomogeneousTransform2D �����任��˺�Ľ����
     */
    HomogeneousTransform2D multiply(const HomogeneousTransform2D& other) const;

    /**
     * @brief ���㵱ǰ�任����任��
     * @details
     * ���һ���任�Ǵ�A����ϵ��B����ϵ (T_A_to_B)����ô������任���Ǵ�B����ϵ��A����ϵ (T_B_to_A)��
     * @return HomogeneousTransform2D ��任����
     */
    HomogeneousTransform2D inverse() const;

private:
    float32_t m_data[9]; // 3x3 �������� (������)
    arm_matrix_instance_f32 m_matrix; // CMSIS-DSP ����ʵ��
};

/**
 * @brief ʹ�� CMSIS-DSP ��3D��α任�ࡣ
 * @details
 * ���ڴ�����ά�ռ��е�ƽ�ƺ���ת��
 */
class HomogeneousTransform3D {
public:
    /**
     * @brief Ĭ�Ϲ��캯��������һ����λ�任��
     */
    HomogeneousTransform3D();

    /**
     * @brief ���캯�������ݸ�����λ�˴���һ���任��
     * @param pose ����ƽ�ƺ���ת��Ϣ��3D�㡣
     */
    HomogeneousTransform3D(const Point3D& pose);

    /**
     * @brief ����һ��ȫ�µı任�����ǵ�ǰֵ��
     * @param pose ����ƽ�ƺ���ת��Ϣ��3D�㡣
     */
    void setTransform(const Point3D& pose);

    /**
     * @brief �����ñ任��ƽ�Ʋ��֣���ת���ֱ��ֲ��䡣
     * @param translation ����x, y, zƽ������3D�㣨����������ת��Ա����
     */
    void setTranslation(const Point3D& translation);

    /**
     * @brief �����ñ任����ת���֣�����ŷ���ǣ���ƽ�Ʋ��ֱ��ֲ��䡣
     * @param roll_rad  ��X�����ת������ǣ���λ�����ȣ���
     * @param pitch_rad ��Y�����ת�������ǣ���λ�����ȣ���
     * @param yaw_rad   ��Z�����ת��ƫ���ǣ���λ�����ȣ���
     */
    void setRotation(float roll_rad, float pitch_rad, float yaw_rad);

    /**
     * @brief ���˱任Ӧ����һ��3D�㡣
     * @param point Ҫ�任��ԭʼ�㡣
     * @return Point3D �任����µ㡣
     */
    Point3D apply(const Point3D& point) const;

    /**
     * @brief ����˷�������ǰ�任����һ���任��ˡ�
     */
    HomogeneousTransform3D multiply(const HomogeneousTransform3D& other) const;

    /**
     * @brief ���㵱ǰ�任����任��
     */
    HomogeneousTransform3D inverse() const;

private:
    float32_t m_data[16]; // 4x4 �������� (������)
    arm_matrix_instance_f32 m_matrix; // CMSIS-DSP ����ʵ��
};



#endif // __APP_COORDCONVERT_H
