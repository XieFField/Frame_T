#include "frame_demo.h"

fdCANbus CAN1_Bus(&hfdcan1, 1); // CAN1
DJI_Group DJI_Group_1(send_idLow(), &CAN1_Bus); // 低片 0x200

M3508 m3508_1(1, &CAN1_Bus);

PID_Param_Config m3508_speed_pid_params = {
    .kp = 32.0f,
    .ki = 0.0f,
    .kd = 0.0f,
    .I_Outlimit = 0.0f, 
    .isIOutlimit = true, 
    .output_limit = 20000.0f,   
    .deadband = 5.0f 
};

PID_Param_Config m3508_angle_pid_params = {
    .kp = 100.0f,
    .ki = 0.0f,
    .kd = 0.005f,
    .I_Outlimit = 8000.0f, 
    .isIOutlimit = true, 
    .output_limit = 20000.0f,   
    .deadband = 5.0f // 
};


// 使用 volatile 防止编译器优化，确保在调试时可以观察到值的变化
volatile int counter = 0;
volatile uint8_t start_signal = 0;
void FrameDemo::loop()
{

}

void FrameDemo::init()
{
    start(osPriorityNormal, 256);
}


void DJI_MotorDemo::loop()
{
    debug_uart.printf_DMA("%f,%f\r\n",m3508_1.getRPM(), m3508_1.getTargetRPM());
    //HAL_UART_Transmit(&huart1, (uint8_t*)"Tick\r\n", 6, HAL_MAX_DELAY);
    if(start_signal == 1)
    {
        m3508_1.setTargetCurrent(500.0f);
    }
    else if(start_signal == 0)
    {
        m3508_1.setTargetCurrent(0.0f);
    }
    else if(start_signal == 2)
    {
        m3508_1.setTargetCurrent(-500.0f);
    }
    else if (start_signal == 3)
    {
        /* code */
        m3508_1.setTargetRPM(60.0f);
    }
    else if (start_signal == 4)
    {
        /* code */
        m3508_1.setTargetRPM(-60.0f);
    }
    else if (start_signal == 5)
    {
        /* code */
        m3508_1.setTargetAngle(180.0f);
    }
    else if (start_signal == 6)
    {
        /* code */
        m3508_1.setTargetAngle(-180.0f);
    }
    else if (start_signal == 7)
    {
        /* code */
        m3508_1.setTargetAngle(720.0f);
    }
    else if (start_signal == 8)
    {
        /* code */
        m3508_1.setTargetAngle(-720.0f);
    }
    else
    {
        m3508_1.setTargetCurrent(0.0f);
    }
}

void DJI_MotorDemo::init()
{
    DJI_Group_1.addMotor(&m3508_1);
    CAN1_Bus.registerMotor(&m3508_1); // 注册电机本身
    CAN1_Bus.registerMotor(&DJI_Group_1); // 同时注册Group用于发送
    m3508_1.pid_init(m3508_speed_pid_params, 0.0f, m3508_angle_pid_params, 0.0f);
    CAN1_Bus.init();
    start(osPriorityNormal, 256);
    
    const char *msg = "Hello UART1 on PB6/PB7\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
    
}



