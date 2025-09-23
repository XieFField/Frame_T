
#include "APP_CoordConvert.h"



/* ==================================================================================
 *                            2D��α任 - ʵ��
 * ==================================================================================*/

// ���캯������ʼ������ʵ������Ϊ��λ����
HomogeneousTransform2D::HomogeneousTransform2D() : m_matrix{3, 3, m_data} 
{
    arm_set_identity_f32(&m_matrix); //�˺�����APP_tool����
}

// ���캯������ʼ������ʵ�������ó�ʼ�任
HomogeneousTransform2D::HomogeneousTransform2D(const Point2D& pose) : m_matrix{3, 3, m_data} 
{
    setTransform(pose);
}


// ���ñ任����
void HomogeneousTransform2D::setTransform(const Point2D& pose) 
{
    float32_t c, s;
    arm_sin_cos_f32(pose.theta, &s, &c); 
    
    // ��������������������
    m_data[0] = c;  m_data[1] = -s; m_data[2] = pose.x;
    m_data[3] = s;  m_data[4] = c;  m_data[5] = pose.y;
    m_data[6] = 0;  m_data[7] = 0;  m_data[8] = 1;
}

// ������ƽ�Ʋ���
void HomogeneousTransform2D::setTranslation(const Point2D& translation) 
{
    m_data[2] = translation.x;
    m_data[5] = translation.y;
}

// ��������ת����
void HomogeneousTransform2D::setRotation(float theta_rad) 
{
    float32_t tx = m_data[2];
    float32_t ty = m_data[5];
    float32_t c, s;
    arm_sin_cos_f32(theta_rad, &s, &c);
    m_data[0] = c; m_data[1] = -s;
    m_data[3] = s; m_data[4] = c;
    // ����ƽ�Ʋ��ֲ���
    m_data[2] = tx;
    m_data[5] = ty;
}

// Ӧ�ñ任��2D��
Point2D HomogeneousTransform2D::apply(const Point2D& point) const 
{
    // ��2D����չΪ������� [x, y, 1]'
    float32_t p_in[3] = {point.x, point.y, 1.0f};
    float32_t p_out[3];
    arm_matrix_instance_f32 p_in_mat = {3, 1, p_in};
    arm_matrix_instance_f32 p_out_mat = {3, 1, p_out};

    // ִ�о���˷�: p_out = M * p_in
    arm_mat_mult_f32(&m_matrix, &p_in_mat, &p_out_mat);
    
    return Point2D(p_out[0], p_out[1]);
}

// ����˷�
HomogeneousTransform2D HomogeneousTransform2D::multiply(const HomogeneousTransform2D& other) const 
{
    HomogeneousTransform2D result;
    // ִ�о���˷�: result = this * other
    arm_mat_mult_f32(&m_matrix, &other.m_matrix, &result.m_matrix);
    return result;
}

// ������任����Ը���任���Ż�������
HomogeneousTransform2D HomogeneousTransform2D::inverse() const 
{
    HomogeneousTransform2D result;
    // ����任���棬��ת���ֵ���ԭ��ת�����ת��
    // R' = R^T
    result.m_data[0] = m_data[0]; result.m_data[1] = m_data[3];
    result.m_data[3] = m_data[1]; result.m_data[4] = m_data[4];

    // ƽ�Ʋ��ֵ��� -R^T * t
    float32_t tx = m_data[2];
    float32_t ty = m_data[5];
    result.m_data[2] = -(result.m_data[0] * tx + result.m_data[1] * ty);
    result.m_data[5] = -(result.m_data[3] * tx + result.m_data[4] * ty);
    
    // ���������겿��
    result.m_data[6] = 0; result.m_data[7] = 0; result.m_data[8] = 1;
    return result;
}


/* ==================================================================================
 *                            3D��α任 - ʵ��
 * ==================================================================================*/

// ���캯������ʼ������ʵ������Ϊ��λ����
HomogeneousTransform3D::HomogeneousTransform3D() : m_matrix{4, 4, m_data} 
{
    arm_set_identity_f32(&m_matrix); //�˺�����APP_tool���� 
}

HomogeneousTransform3D::HomogeneousTransform3D(const Point3D& pose) : m_matrix{4, 4, m_data} 
{
    setTransform(pose);
}



void HomogeneousTransform3D::setTransform(const Point3D& pose) 
{
    setRotation(pose.roll, pose.pitch, pose.yaw);
    setTranslation(pose);
}

void HomogeneousTransform3D::setTranslation(const Point3D& translation) 
{
    m_data[3] = translation.x;
    m_data[7] = translation.y;
    m_data[11] = translation.z;
}

// ����ŷ����������ת����
void HomogeneousTransform3D::setRotation(float roll_rad, float pitch_rad, float yaw_rad) 
{
    float32_t sr, cr, sp, cp, sy, cy;
    arm_sin_cos_f32(roll_rad, &sr, &cr);
    arm_sin_cos_f32(pitch_rad, &sp, &cp);
    arm_sin_cos_f32(yaw_rad, &sy, &cy);

    float32_t tx = m_data[3];
    float32_t ty = m_data[7];
    float32_t tz = m_data[11];

    // ���� Z-Y-X ŷ����˳�򹹽���ת����
    m_data[0] = cy * cp;
    m_data[1] = cy * sp * sr - sy * cr;
    m_data[2] = cy * sp * cr + sy * sr;

    m_data[4] = sy * cp;
    m_data[5] = sy * sp * sr + cy * cr;
    m_data[6] = sy * sp * cr - cy * sr;

    m_data[3] = tx;
    m_data[7] = ty;
    m_data[11] = tz;

    m_data[8]  = -sp;
    m_data[9]  = cp * sr;
    m_data[10] = cp * cr;
    
    // ȷ��������겿����ȷ
    m_data[12] = 0; m_data[13] = 0; m_data[14] = 0; m_data[15] = 1;
}

// Ӧ�ñ任��3D��
Point3D HomogeneousTransform3D::apply(const Point3D& point) const 
{
    // ��3D����չΪ������� [x, y, z, 1]'
    float32_t p_in[4] = {point.x, point.y, point.z, 1.0f};
    float32_t p_out[4];
    arm_matrix_instance_f32 p_in_mat = {4, 1, p_in};
    arm_matrix_instance_f32 p_out_mat = {4, 1, p_out};

    // ִ�о���˷�: p_out = M * p_in
    arm_mat_mult_f32(&m_matrix, &p_in_mat, &p_out_mat);
    
    return Point3D(p_out[0], p_out[1], p_out[2]);
}

// ����˷�
HomogeneousTransform3D HomogeneousTransform3D::multiply(const HomogeneousTransform3D& other) const 
{
    HomogeneousTransform3D result;
    arm_mat_mult_f32(&m_matrix, &other.m_matrix, &result.m_matrix);
    return result;
}

// ������任����Ը���任���Ż�������
HomogeneousTransform3D HomogeneousTransform3D::inverse() const 
{
    HomogeneousTransform3D result;
    
    // ����һ����ʱ��3x3����������ת���ֵļ���
    float32_t rot_data[9];
    arm_matrix_instance_f32 rot = {3, 3, rot_data};

    // ��ȡԭ��ת�����ת�ã����棩
    rot_data[0] = m_data[0]; rot_data[1] = m_data[4]; rot_data[2] = m_data[8];
    rot_data[3] = m_data[1]; rot_data[4] = m_data[5]; rot_data[5] = m_data[9];
    rot_data[6] = m_data[2]; rot_data[7] = m_data[6]; rot_data[8] = m_data[10];
    
    // ��ת�ú����ת�����Ƶ������
    result.m_data[0] = rot_data[0]; result.m_data[1] = rot_data[1]; result.m_data[2] = rot_data[2];
    result.m_data[4] = rot_data[3]; result.m_data[5] = rot_data[4]; result.m_data[6] = rot_data[5];
    result.m_data[8] = rot_data[6]; result.m_data[9] = rot_data[7]; result.m_data[10] = rot_data[8];

    // �����µ�ƽ�Ʋ���: t' = -R^T * t
    float32_t t_in[3] = {m_data[3], m_data[7], m_data[11]};
    float32_t t_out[3];
    arm_matrix_instance_f32 t_in_mat = {3, 1, t_in};
    arm_matrix_instance_f32 t_out_mat = {3, 1, t_out};
    
    arm_mat_mult_f32(&rot, &t_in_mat, &t_out_mat);

    result.m_data[3] = -t_out[0];
    result.m_data[7] = -t_out[1];
    result.m_data[11] = -t_out[2];

    // ����������겿��
    result.m_data[12] = 0; result.m_data[13] = 0; result.m_data[14] = 0; result.m_data[15] = 1;

    return result;
}


