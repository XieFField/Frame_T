## RC10_LIB FrameWork�û��ֲ�

RC10_LIB���ṩ����Ԥ�Ʋˣ�ּ���öԵײ���������Ϥ���û�Ҳ�ܳ�����дӦ�ò���롣�����û��ֲ�Ҳ��Ԥ�Ʋ˵�һ����ּ�����û����Ը�������ʹ��RC10_LIB

### ������Ŀǰִ�е������淶
1. �����еı���ͳһ�� _ �ĺ�׺����`rpm_`
2. �����еĳ�Ա��Сд��ĸ��ͷ
3. ������Ҫ��Сд��ĸ�ʹ�д��ĸ
4. RC10_LIB���е�ͷ�ļ���Դ�ļ����������֧��ǰ׺����"Motor_","BSP_"

### ��������
1. ��дע�ͣ��������д����������һ����vscode�Դ���ai��ȫע��
2. �����ڿ���û��ͷ��ʱ�򣬿��Իع˿����ֲ�
3. ��Ҫ����API����RC10_LIB

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
### Motor��֧



### Module��֧