## RC10_LIB FrameWork�û��ֲ�

RC10_LIB���ṩ����Ԥ�Ʋˣ�ּ���öԵײ���������Ϥ���û�Ҳ�ܳ�����дӦ�ò���롣�����û��ֲ�Ҳ��Ԥ�Ʋ˵�һ����ּ�����û����Ը�������ʹ��RC10_LIB

attention: ����ֲ�ܴ�̶���AI���ɵģ�����ֻ�����޸����в��֣����������©���뼰ʱ�����ң���ָ�л��

### ������Ŀǰִ�е������淶
1. �����еı���ͳһ�� _ �ĺ�׺����`rpm_`
2. �����еĳ�Ա��Сд��ĸ��ͷ
3. ������Ҫ��Сд��ĸ�ʹ�д��ĸ
4. RC10_LIB���е�ͷ�ļ���Դ�ļ����������֧��ǰ׺����"Motor_","BSP_"

### ��������
1. ��дע�ͣ��������д����������һ����vscode�Դ���ai��ȫע��
2. �����ڿ���û��ͷ��ʱ�򣬿��Իع˿����ֲ�
3. ��Ҫ����API����RC10_LIB
4. ��ֹһ�ж�̬�ڴ����

### BSP��֧
#### FreeRTOS��ʹ��
��BSP_RTOS.h�ļ��У���װ�˻�����RTOSʹ�ã�Ŀǰ�л���������Ͷ���

1. ĿǰRtosTask����������ӵ������ģʽ
   1. ����Ԥ�Ʋ�ģʽ���û��ڳ�ʼ��ʱ��ֻ��������������Լ���дһ����ʼ���������ڷ���start��������.(**ע�⣺��������start��������ע�����񣬶���start����������osKernelStart();֮ǰ����,main.cpp�е�**)
        ```cpp
        /*����*/
        /*
            1.�û���Ҫ���ģ�ʹ��RtosTaskʵ��������
            2.����ĳ�ʼ�������У���˴���init()��д��start������ָ����������ȼ���ջ��С
            3.�ڳ���Ԥ�Ʋ�ģʽ�£�ֻ��Ҫ��дloop��д������ִ�е����񼴿�
        */
        class FrameDemo : public RtosTask
        {
        public:
            FrameDemo() : RtosTask("FrameDemo") {}
            void init();
            void loop() override; 
            volatile int counter = 0;
        };

        void FrameDemo::loop()
        {
            counter++;
        }

        void FrameDemo::init()
        {
            start(osPriorityNormal, 256);
        }

        ```
    2. �Զ���ģʽ
        ```cpp
        /*����*/
            /*
                1.�û���Ҫ���ģ�ʹ��RtosTaskʵ��������ͬʱ���빹�캯���ĵڶ����������ӳ�ʱ��(Ĭ����1)����������0��������Զ���ģʽ
                2.����ĳ�ʼ�������У���˴���init()��д��start������ָ����������ȼ���ջ��С
                3.���Զ���ģʽ�£�����Ҫ�����������ĹǼܣ���дrun()��Ա
            */
            class FrameDemo : public RtosTask
            {
            public:
                FrameDemo() : RtosTask("FrameDemo",0) {}
                void init();
                void loop() override; 
                volatile int counter = 0;
            };

            void FrameDemo::run()
            {
                static int i;
                for(;;)
                {
                    i++;
                    if(i > 10)
                    {
                        counter++;
                        i = 0;
                    }
                    
                    osDelay(1);
                }
                
            }

            void FrameDemo::init()
            {
                start(osPriorityNormal, 256);
            }
        ```
### APP��֧

#### APP_tool
������

#### APP_PID
1. λ��ʽPID
   1. ���������λ��֡�΢�����С����ַ���
      1. ΢�����У���ͳPID��target_setͻ��ʱ��΢���������弤��΢���������ڲ�����error�ı仯�ʣ�����ֱ�Ӽ���feedback�ı仯��
        ```cpp
        // ��ͳ D ��: D = kd * (error - last_error) / dt
        // ΢������ D ��: D = kd * (last_feedback - current_feedback) / dt
        ```
      2. ���λ��֣� �ȼ򵥵ľ��λ��֣�I += ki * error * dt������ȷ���������ڲ���ʱ�� dt ���ȶ������仯�Ͽ�ʱ����������ǵ�ǰ�����ϴ����ɵ����������
        ```cpp
        integral_term += ki * (error + last_error) / 2.0f * dt;
        ```
      3. ���ַ��룺 �����ܴ�ʱ����ʱ���û����ۼӣ���ֹ��������챥�ͣ�����ϵͳ�������ء�ֻ�е�������һ���ɽ��ܵķ�Χ�󣬲ſ�ʼ�ۼӻ��֡�
        ```cpp
        if (abs(error) < I_SeparaThreshold_) 
        {
            // ֻ��������Сʱ���ۼӻ���
            integral_term += ...;
        }
        ```

2. ����ʽPID
    1. ����ʽPID������΢�ָ�����(Track_D)����Ϊһ���ź�Ԥ����ģ��
       1. ��ͳPID��Ŀ��ֵ������Ծʱ������ͻ�䣬ʹϵͳ�����𵴡�
       2. ԭ��
          1. ����һ��Ŀ��ֵtarget��Track_D
          2. Track_D�������̰�target����PID���������ڲ�ģ��һ�����׶�̬ϵͳ������һ��ƽ���������Ĺ�������V1��ʹ��V1ƽ���ƽ�target��
          3. Track_D��V1��ΪPID��ʵ��Ŀ��ֵ
       3. Track_D��ʵ�ʳ��������ã�
            ����ʹ��PID�������û����˴�A��B��
          1. ��Track_D: �����˻��͵�������Ȼ��ɲ����B�㣬���ҿ��ܳ��������Ǻ��ȶ���
          2. ��Track_D�������˻�滮һ��ƽ���ļ������ߣ�Ȼ�����ȵ������١����١����٣�Ȼ��׼ͣ��B��
    2. 
3. �û������ʹ�ã�
   1. λ��ʽPID
        α����
        ```cpp
        //init
        PID_Param_Config param_init = {������};// ���� Kp, Ki, Kd �Ȳ���
        PID_Position  pid(param_init);
        float target_pos = 100.0f;
        motor.set_pos(pid.calc(target_pos, motor.get_pos()));
        ```
   2. ����ʽPID
        α����
        ```cpp
        // ��ʼ��
        PID_Param_Config param_init = { ... }; // ���� Kp, Ki, Kd �Ȳ���
        float td_ratio = 0.8f; // ���ø���΢�����ٶȣ�0Ϊ��ʹ��
        PID_Incremental pid_speed(param_init, td_ratio);

        
        float target_speed = 5000.0f; 
        float current_speed = motor.get_speed(); // ��ȡ��ǰ����ٶ�

        // pid_calc���ص��ǡ���ǰ�������������ֱ��ʹ��
        float motor_output = pid_speed.pid_calc(target_speed, current_speed);

        // �����������͸����
        motor.set_current(motor_output); // �����ǵ���������
        ```


### Module��֧

---

### DJI ���ʹ��ָ��

��ָ�Ͻ���������ɴ�Ӳ����ʼ������ RTOS �����п��ƴ�ϵ�е����M3508, M2006, GM6020�����������̡�

#### �������ع�

�ڿ�ʼ֮ǰ�����ס RC10_LIB �ĺ�����ƣ�

1.  **����ԭ��**: `fdCANbus` ��һ�������ͨ��ͨ������������п����߼���PID���㣩��Э�������ڵ�������� (`M3508`, `DJI_Group` ��) ����ɡ�
2.  **�Զ�������**: �� **����Ҫ** �ֶ����� PID ����� CAN ���ͺ�����`fdCANbus` �ڲ��� `schedulerTask` ���� 1kHz ��Ƶ���Զ�������¹�����
    *   ����������ע�����������飩�� `update()` ������ִ�� PID ���ƻ�·��
    *   ���� `packCommand()` ��������������Ŀ���ָ������ CAN ֡��
    *   �� CAN ֡���ͳ�ȥ��
3.  **�û�ְ��**: �����Ҫ��������һ�������Ŀ��������У�������Ҫ���� `setTargetCurrent()`, `setTargetRPM()`, `setTargetAngle()` ���趨Ŀ��ֵ���ɡ�

#### ��һ����ϵͳ��ʼ��

����Ӳ���Ͷ���ĳ�ʼ����Ӧ�������� RTOS ������ (`osKernelStart()`) ֮ǰ��ɡ��Ƽ��� `main.cpp` �� `USER CODE BEGIN 2` �� `USER CODE END 2` ֮�䣬����һ��ר�ŵ� `user_setup.cpp` �ļ��н��С�

������һ�������ĳ�ʼ��ʾ����

```cpp
/* main.cpp �� user_setup.cpp */

#include "fdcan.h" // ��CubeMX����
#include "BSP_fdCAN_Driver.h"
#include "Motor_DJI.h"

// 1. ����ȫ�ֵ�CAN���ߺ͵������ָ��
//    ʹ��ָ����Ϊ�˷����ڲ�ͬ�ļ��з��ʣ���Ҳ����ѡ��������ʽ�������
fdCANbus* can1_bus_p;
M3508* m3508_p;
DJI_Group* group1_4_p;

// 2. ����һ����ʼ������
void user_setup()
{
    // --- CAN ���߳�ʼ�� ---
    // ����1: FDCAN��� (����fdcan.h)
    // ����2: ����ID (1, 2, �� 3)�������ж�·��
    fdCANbus can1_bus_p(&hfdcan1, 1);

    // --- ����͵�����ʼ�� ---
    // ����һ�� M3508 ���ʵ��
    // ����1: ���ID (1-4 ��Ӧ 0x201-0x204, 5-8 ��Ӧ 0x1FF)
    // ����2: ������CAN����ָ��
    M3508 m3508_p(1, can1_bus_p);

    // ����һ�� DJI ����� (����4��1���)
    // ����1: ��������ID (0x200 �� 0x1FF)
    // ����2: ������CAN����ָ��
    DJI_Group group1_4_p(0x200, can1_bus_p);

    // --- ע�������� ---
    // �������ӵ������
    group1_4_p.addMotor(m3508_p);
    // ����Լ�����Ӹ������������...
    // group1_4_p->addMotor(another_motor_p);

    // ����Ҫ���������ע�ᵽCAN����
    // ע�⣺����ע����� group1_4_p�������� m3508_p��
    // fdCANbus ���Զ����� group1_4_p �� update �� packCommand ������
    can1_bus_p.registerMotor(group1_4_p);

    // --- PID ������ʼ�� ---
    // Ϊ m3508_p ����PID���� (AI���ɵĲ�������Ҫֱ������)
    PID_Param_Config speed_pid_params = {
        .kp = 10.0f, 
        .ki = 1.0f, 
        .kd = 0.1f,
        .I_Outlimit = 5000.0f,
        .isIOutlimit = true,
        .output_limit = 16384.0f, // M3508 ������Χ
        .deadband = 0.0f
    };
    PID_Param_Config angle_pid_params = {
        .kp = 100.0f, 
        .ki = 0.5f, 
        .kd = 0.0f,
        .I_Outlimit = 300.0f, // λ�û�����������ƣ�ͨ��������������ٶ�
        .isIOutlimit = true,
        .output_limit = 1000.0f, // λ�û�������ƣ��������Ŀ���ٶ�
        .deadband = 0.1f // ��������ֹ��Ŀ�긽��΢С����
    };
    // ����pid_init���г�ʼ��
    m3508_p.pid_init(speed_pid_params, 0.1f, angle_pid_params, 5.0f);

    // --- ����CAN���� ---
    // �������CANӲ���������˲����������ڲ��� rxTask �� schedulerTask
    can1_bus_p.init();
}

// �� main() �����е���
int main(void)
{
    // ... HAL_Init(), SystemClock_Config(), MX_GPIO_Init(), MX_FDCAN1_Init() ...
    
    /* USER CODE BEGIN 2 */
    user_setup(); // �������ǵĳ�ʼ������
    /* USER CODE END 2 */

    // ... osKernelInitialize(), ������Ŀ������� ...
    
    osKernelStart(); // ����������

    // ...
}
```

#### �ڶ������������п��Ƶ��

���ڣ�����Դ���һ���������������Ϳ���ָ�

1.  **��������������**

    �̳� `RtosTask` ��ʵ�� `loop()` ��������Ϊ����һ�����������񣩡�

    ```cpp
    /* ������û�Ӧ�ò�, e.g., User/Control/control_task.h */
    #include "BSP_RTOS.h"
    #include "Motor_DJI.h"

    // �����ڳ�ʼ�������д�����ȫ�ֶ���
    extern M3508 m3508_p;

    class MyControlTask : public RtosTask
    {
    public:
        // ���캯�������������������� (e.g., 10ms -> 100Hz)
        MyControlTask() : RtosTask("MyCtrl", 10) {}

    protected:
        // ������ִ�е�ѭ����
        void loop() override;
    };
    ```

2.  **ʵ�ֿ����߼�**

    �� `loop()` �У�����Ը���ϵͳ״̬�л�����Ŀ���ģʽ���趨Ŀ�ꡣ

    ```cpp
    /* ������û�Ӧ�ò�, e.g., User/Control/control_task.cpp */
    #include "control_task.h"
    #include <cmath> // for sin

    void MyControlTask::loop()
    {
        // ��ȡ��ǰʱ��������ڲ����仯��Ŀ��ֵ
        uint32_t tick = osKernelGetTickCount();

        // --- ʾ��1��λ�ÿ��� ---
        // �õ���� -90 �� 90 ��֮�����ذڶ�
        float target_angle = 90.0f * sin(tick * 0.002f);
        m3508_p.setTargetAngle(target_angle);

        /*
        // --- ʾ��2���ٶȿ��� ---
        // ����һ���㶨��Ŀ��ת�� (RPM)
        // m3508_p->setTargetRPM(500.0f);
        */

        /*
        // --- ʾ��3���������� ---
        // ����һ���㶨��Ŀ����� (Ampere)
        // m3508_p->setTargetCurrent(0.5f);
        */
    }
    ```

3.  **��������������**

    ����� `main.cpp` �� `osKernelStart()` ֮ǰ����������ʵ������������

    ```cpp
    /* main.cpp */

    // ...
    #include "control_task.h"

    MyControlTask my_control_task; // ��������ʵ��

    int main(void)
    {
        // ...
        user_setup();
        /* USER CODE BEGIN 2 */

        // ������������
        my_control_task.start(osPriorityNormal);

        /* USER CODE END 2 */
        
        osKernelStart();
        // ...
    }
    ```

#### �����������Ӷ�ʱ���ж� (��ֲʱע��)

**ע�⣺�ڵ�ǰ��Ŀ�У��˲����Ѿ��ɿ�������ɡ�����������Ҫ��������ά���߻�ϣ�����˿���ֲ��������Ŀ�Ŀ�����˵���乤��ԭ��ͱ�Ҫ���á�**

`fdCANbus` �� `schedulerTask` ����һ�� 1kHz ���ź���������������ź�����һ��ȫ���жϺ��� `fdcan_global_scheduler_tick_isr()` ������

��ˣ��κ�ʹ�� `RC10_LIB` ����Ŀ�� **����** ��һ��Ӳ����ʱ������ TIM6, TIM7������Ϊ 1ms ����һ���жϣ��������жϻص������е��� `fdcan_global_scheduler_tick_isr()`��

##### 1. ʹ�� CubeMX ����һ��������ʱ��
    *   ѡ��һ����ʱ�� (e.g., TIM7)��
    *   ���� `Prescaler` �� `Counter Period` ʹ��ÿ 1ms ����һ�θ����¼� (Update Event)��
        *   ���磬��� TIM CLK �� 120MHz���������� Prescaler = 120-1, Counter Period = 1000-1��
    *   �� "NVIC Settings" ��ǩҳ�У�**���øö�ʱ���ĸ����ж�**��

##### 2. ���жϻص�����ӵ���

    �� `stm32h7xx_it.c` �ļ��У��ҵ���ʱ�����жϷ����� `TIMx_IRQHandler`��HAL ����ڴ˺����е���һ��������Ļص����� `HAL_TIM_PeriodElapsedCallback`������Ҫ��д����ص���

    ```c

    // �������ǵ�ȫ�ֵ��Ⱥ���
    extern void fdcan_global_scheduler_tick_isr(void);

    /**
      * @brief  Period elapsed callback in non-blocking mode
      * @param  htim TIM handle
      * @retval None
      */
    void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
    {
      /* USER CODE BEGIN Callback 0 */

      /* USER CODE END Callback 0 */
      if (htim->Instance == TIM1) { // TIM1 �� HAL ��Ĭ�ϵ� SysTick ��ʱ��
        HAL_IncTick();
      }
      /* USER CODE BEGIN Callback 1 */

      // ���������1kHz���ȵĶ�ʱ�� (�ڵ�ǰ��Ŀ����TIM6)
      if (htim->Instance == TIM6)
      {
        fdcan_global_scheduler_tick_isr();
      }

      /* USER CODE END Callback 1 */
    }
    ```

������ϲ������ĵ������ϵͳ���������������ˡ����벢���س��򣬵��Ӧ�ûᰴ�� `MyControlTask` ���趨���߼���ʼ�˶���