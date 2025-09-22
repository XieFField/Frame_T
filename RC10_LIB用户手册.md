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

### User
1. �������������Control
2. ����debug/demo�����debug
3. Setup���ڷų�ʼ���ļ�

### RC10_LIB�ĺ������ԭ��
1. �ϸ�ֲ㣬ְ��һ
   ��ܷ�ΪӲ�������㡢�豸Э��㡢�㷨���Ӧ�ò㡣��������¹���ʱ��������ȷ�������

   Ӳ������ֻ��������������ͨ�š�
   �豸Э��ֻ��������ʹ���ض��豸�ı��ġ�
   �㷨�Ǵ������ѧ���ߡ�
   Ӧ��ֻ�����´�߲�ָ� ԭ�� ��ֹ�����ã����ָ��㴿���ԡ�

2. �����Զ������ȣ������������
   1. ����: fdCANbus ����ṩһ����Ƶ�ʵ�����������������Զ���������ע���豸�� update() �� packCommand()��

   update(): ֻ���ڼ��㡣ִ����PID���������㷨�������ڲ�״̬��
   packCommand(): ֻ���ڴ������ȡ update() �ļ���������������װ�ɴ����͵�CAN���ġ�
   setTarget...(): ֻ���ڽ���ָ�������������ṩ��Ӧ�ò�Ľӿڣ��������ø߼�Ŀ�ꡣ ԭ�� ��Զ��Ҫ�� packCommand() �н��м��㣬Ҳ��Ҫ�� update() ����װ���ġ����ŵ������ᰴ��ȷ��˳��������ǡ�

3. �̳�ͳһ�ӿڣ����ö�̬ʵ��������
   ���ͨ������ӿڱ��ʵ����չ�ԡ������豸����������̳���һ����ͬ�Ļ��ࣨ�� Motor_Base����

   ͳһ����: ������ֻ�����ӿڽ������������ľ�����ʲô�豸��
   �麯��ʵ�ֶ�̬: ʹ�� virtual �������� get_GearRatio()������ÿ�������ṩ�Լ����ص���Ϣ����Ϊ�� ԭ�� ������豸��������ʵ�ֻ�������д��麯�����������麯����д��override����ʵ�����ض�Э��͹��ܡ�

4. �û�ʹ�ýӿڵļ�
   ��һ�е��ظ��Թ���������ķ�װ��ʵ�֣�ʹ���û��ڿ���Ӧ�ò��ʱ������д̫�������ظ��Ĵ��룬����Ч���п�����

### BSP��֧
#### FreeRTOS��ʹ��
��`BSP_RTOS.h`�ļ��У���װ�˻�����RTOSʹ�ã�Ŀǰ�л���������Ͷ��С�

1.  **RtosTask �����װ**
    `RtosTask` ���ṩ����������ģʽ��ͨ�����캯���� `period` �������֣�
    *   **���������� (`period > 0`)**: ������� `period` ָ����Tick����Զ�ѭ��ִ�� `loop()` ��������������Ҫ�̶�Ƶ�����еļ��߼���
        ```cpp
        class MyPeriodicTask : public RtosTask {
        public:
            MyPeriodicTask() : RtosTask("MyTask", 1000) {} // 1000ms����
        protected:
            void loop() override 
            {
                // ����Ĵ���ÿ1000msִ��һ��
            }
        };
        ```
    *   **�¼��������� (`period = 0`)**: ���񴴽����ִ��һ�� `run()` ������`run()` �����������һ����ѭ�� `for(;;)` ��һ���������ã��� `vTaskDelay`, `xSemaphoreTake`�������ڵȴ��ⲿ�¼�����������Ҫ���������ĸ�����������CAN���ߵĵ��Ⱥͽ�������
        ```cpp
        class MyEventTask : public RtosTask {
        public:
            MyEventTask() : RtosTask("EventTask", 0) {} // �¼�����
        protected:
            void run() override 
            {
                for(;;) 
                {
                    // �ȴ��ź����������¼�
                    xSemaphoreTake(mySemaphore, portMAX_DELAY); 
                    // �����¼�...
                }
            }
        };
        ```

2.  **RtosQueue ���з�װ**
    ����һ��ģ���࣬���Է���ش�����ʹ���̰߳�ȫ�Ķ��С�
    ```cpp
    // ����һ��������8��int�Ķ���
    RtosQueue<int> myQueue(8);

    // ��һ�������з�������
    myQueue.send(123);

    // ����һ�������н�������
    int received_value;
    if (myQueue.recv(received_value, 100)) { // �ȴ�100ms
        // �ɹ����յ�����
    }
    ```

### APP��֧

#### APP_tool
�����࣬�ṩ�� `constrain`���޷�����ͨ�ú�����

#### APP_debugTool
�ṩ���Թ��ߣ��紮�ڴ�ӡ���ݡ�

#### APP_PID
�ṩ��λ��ʽ������ʽ����PID��������

1.  **�������**
    *   **λ��ʽPID**: ���������λ��֡�΢�����С����ַ���ȸĽ��㷨�������ڴ󲿷���Ҫ��ȷλ�ÿ��Ƶĳ�����
    *   **����ʽPID**: ������΢�ָ�����(Track_D)������Чƽ��Ŀ��ֵ�Ľ�Ծ�仯������ϵͳ�𵴣��������ٶȿ��Ƶȳ�����
    *   **�̶�����ʱ��**: PID�������ڲ��� `dt` ��Ӳ����Ϊ `0.001f` (1ms)������һ��**�������**����ǿ�����ڵ��� `pid_calc` �� `update()` ������һ����ȷ��1kHz���������� `fdCANbus::schedulerTaskbody`�������á������ῼ�ǰ���������ñ���ֵ����ʱ��Ĵ���������������dt���Ӿ�ȷ��

2.  **�û������ʹ�ã�**
    �ڵ���ࣨ�� `M3508`���� `pid_init` �����г�ʼ��PID������Ȼ���� `update` �����е��� `pid_calc` ���ɡ��û�������� `dt` �ļ��㡣
    ```cpp
    // �� M3508::update() ��
    case SPEED_CONTROL:
    {
        // target_rpm_ �� this->rpm_ ���������ת�٣��߶�ͳһ
        target_current_ = speed_pid_.pid_calc(target_rpm_, this->rpm_);
        break;
    }
    ```

### Module��֧
�˷�֧��Ҫ�������ض�Ӳ��ģ����صĴ��룬���� `Module_Encoder.cpp`�������𽫱�������ԭʼֵ����0-8191��ת��Ϊ�����ĽǶȣ�-��, +�ޣ��͵�Ȧ�Ƕȣ�[0, 360)����

---

### fdCANbus��ι����ģ�

`fdCANbus` ������������ƿ�������ࡣ��������ײ��CANͨ�ţ����Ծ�ȷ��Ƶ���Զ��������е���������񣬽��û��ӷ�����ʵʱ���ƺ�Ӳ�������н�ų�����

#### ��������빤������

`fdCANbus` �ڲ���Ҫ���������е�RTOS����������

1.  **�������� (`rxTask_`)**:
    *   **����**: ����һ���¼������������������������ȴ� `rxQueue_` �����е�����Ϣ��
    *   **������**:
        1.  ��CANӲ�����յ�һ������֡��`HAL_FDCAN_RxFifo0Callback` �жϷ������ISR����������
        2.  ISR���� `fdcan_global_rx_isr`���ú�����Ӳ����������ȡԭʼCAN���ġ�
        3.  ԭʼ���ı���װ�� `CanFrame` ���󣬲����������� `rxQueue_` ���С�
        4.  `rxTask_` �����ѣ��Ӷ�����ȡ�� `CanFrame`��
        5.  `rxTask_` ����������ע��ĵ����`motorList_`��������ÿ������� `matchesFrame()` ������Ѱ�Ҹñ��ĵġ����ˡ���
        6.  һ���ҵ�ƥ��ĵ�����͵����� `updateFeedback()` �����������Ľ��ɵ�����н�����

2.  **�������� (`schedulerTask_`)**:
    *   **����**: ����һ�������ȼ��ġ��ɶ�ʱ����ȷ����������������Ƶ��Ϊ1kHz��
    *   **������**:
        1.  һ��1kHz��Ӳ����ʱ���жϴ��� `fdcan_global_scheduler_tick_isr()`��
        2.  ��ISR�ͷţ�Give��һ����Ϊ `schedSem_` ���ź�����Ȼ�������˳���
        3.  `schedulerTask_` ���������һֱ�����ȴ���Take������ź�����һ����ȡ���ź��������ͻᱻ���ѡ�
        4.  **����**: �������ȱ�������ע��ĵ���������飩�����������ǵ� `update()` ��������ᴥ��PID����ȿ����߼���
        5.  **���**: ���ţ������ٴα������ж��󣬵��� `packCommand()` �������ռ���Ҫ���͵�CANָ��֡��
        6.  **����**: ������������ռ�����ָ��֡ͨ�� `sendFrame()` �������ͳ�ȥ��`sendFrame` �ڲ�ʹ�û����� `tx_mutex_` ��ȷ�����������CANӲ�����̰߳�ȫ��
        7.  ���һ�ֵ��Ⱥ�`schedulerTask_` ����ѭ���Ŀ�ʼ���ٴ������ȴ���һ�ε��ź������Ӷ�ʵ�־�ȷ��1ms���ڡ�

#### �ؼ���ƾ���

*   **�жϷ������ISR����С��**: ISRֻ�����ٵĹ���������ȡ���ݲ�����������С����к�ʱ�Ĳ������������ƥ�䡢��������ת�Ƶ����ȼ��ϵ͵� `rxTask_` ��ִ�У���ȷ����ϵͳ��ʵʱ��Ӧ������
*   **��������շ���**: ��������ȫ�첽���¼������ģ�����������ͬ���������Եġ�������Ʒ��Ͽ���ϵͳ�ĵ���ģʽ���������շ��������Թ̶���Ƶ���������ָ�
*   **ȫ���ж�·��**: ͨ��һ��ȫ�ֵ� `g_fdcan_bus_map` ���飬���Խ�����HAL��ġ������־������ߵ�C����жϻص�����ȷ��·�ɵ���Ӧ�� `fdCANbus` C++����ʵ���ϡ���ʹ�ô����������֧�ֶ��CAN���ߡ�
*   **�̰߳�ȫ**: ͨ��ʹ��RTOS���У�`RtosQueue`���ͻ�������`tx_mutex_`����`fdCANbus` ȷ�����ڶ����񻷾������ݽ�����Ӳ�����ʵİ�ȫ�ԡ�

### �������������ʹ��ָ��

��ָ�Ͻ���������ɴ�Ӳ����ʼ������ RTOS �����п��Ƶ�����������̡�

#### �������˼��

1.  **����ת��ǰ��**: �� `DJI_Motor::updateFeedback` �����У���CAN���߽��յ���**���ת��ԭʼ����**��ת�١�������ֵ����**����**ͨ���麯�� `get_GearRatio()` ��ȡ��ȷ�ļ��ٱȣ�����ת��Ϊ**���ٺ�����������**��

2.  **�ڲ�״̬ͳһ**: ת����ɺ����д洢�ڻ��� `Motor_Base` �еĳ�Ա������`rpm_`, `angle_`, `totalAngle_`���ĺ��嶼ͳһΪ**������״̬**��

3.  **�����뷴���߶�ͳһ**: PID���ƻ�·���� `update()` �����У���**Ŀ��ֵ**���� `target_rpm_`����**����ֵ**���� `this->rpm_`��������**�����ĳ߶�**���м��㣬��֤�˿��Ƶ���ȷ�ԡ�

4.  **�����Զ���**: �� **����Ҫ** �ֶ����� PID ����� CAN ���ͺ�����`fdCANbus` �ڲ��� `schedulerTask` ���� 1kHz ��Ƶ���Զ����������ע�����������飩�� `update()` �� `packCommand()` ���á�

5.  **�û�ְ��**: ��Ĺ����ǳ��򵥣�ֻ����һ�������Ŀ��������У�������Ҫ���� `setTargetRPM()`, `setTargetAngle()` �Ⱥ������趨**������Ŀ��ֵ**���ɡ�

#### ��һ����ϵͳ��ʼ��

����Ӳ���Ͷ���ĳ�ʼ����Ӧ�������� RTOS ������ (`osKernelStart()`) ֮ǰ��ɡ��Ƽ��� `main.cpp` �� `USER CODE BEGIN 2` �� `USER CODE END 2` ֮�䣬����һ��ר�ŵ� `user_setup.cpp` �ļ��н��С�

```cpp
/* main.cpp �� user_setup.cpp */

#include "BSP_fdCAN_Driver.h"
#include "Motor_DJI.h"

// 1. ����ȫ�ֶ���
// ע�⣺����ֱ�Ӷ�����󣬶�����ָ�룬�Ա��⶯̬�ڴ����
fdCANbus CAN1_Bus(&hfdcan1, 1); // CAN����
M3508 m3508_1(1, &CAN1_Bus);     // M3508���, IDΪ1
DJI_Group DJI_Group_1(0x200, &CAN1_Bus); // DJI�����, ����IDΪ0x200

// 2. ����һ����ʼ������
void user_setup()
{
    // --- PID�������� ---��AI���ɵģ�����ͨ�ò�����
    PID_Param_Config speed_pid_params = 
    {
        .kp = 10.0f, .ki = 0.5f, .kd = 0.0f,
        .I_Outlimit = 5000.0f, .isIOutlimit = true,
        .output_limit = 16000.0f, .deadband = 0.0f
    };
    PID_Param_Config angle_pid_params = 
    {
        .kp = 0.5f, .ki = 0.0f, .kd = 0.0f,
        .I_Outlimit = 100.0f, .isIOutlimit = true,
        .output_limit = 500.0f, .deadband = 0.0f
    };
    m3508_1.pid_init(speed_pid_params, 0.0f, angle_pid_params, 30.0f);

    // --- ע�������� ---
    // �������ӵ������
    DJI_Group_1.addMotor(&m3508_1);
    // ����Լ�����Ӹ������������...
    // DJI_Group_1.addMotor(&another_motor);

    // ����Ҫ�����������͵���鶼ע�ᵽCAN����
    // 1. ע��������ʹ���ܽ��շ������Ĳ�����״̬
    CAN1_Bus.registerMotor(&m3508_1);
    // 2. ע�����飬ʹ���ܱ����������� packCommand() ��������͵���ָ��
    CAN1_Bus.registerMotor(&DJI_Group_1);

    // --- �������� ---
    // �������CAN�Ľ����жϺ�1kHz�ĵ�������
    CAN1_Bus.init();
}

// �� main() �����е���
int main(void)
{
    // ... HAL_Init(), SystemClock_Config(), MX_GPIO_Init(), MX_FDCAN1_Init() ...
    
    user_setup(); // �������ǵĳ�ʼ������
    
    osKernelInitialize();
    // ... ���������û����� ...
    osKernelStart();
    
    // ...
}
```

#### ���������չ�����
������Ҫ���һ����DJI�ġ����Լ�����CANЭ��ĵ�������� MyMotor��

1. ���� `Motor_MyMotor.h`

```cpp
#include "Motor_Base.h"
#include "APP_PID.h" // �����ҪPID

class MyMotor : public Motor_Base {
public:
    // 1. ���캯�������û��๹�캯��
    MyMotor(uint32_t id, fdCANbus* bus) 
        : Motor_Base(id, false, bus) // ����ʹ�ñ�׼֡
    {
        // ��ʼ���õ����˽�г�Ա
    }

    // 2. �����롿���� packCommand
    //    ���� target_current_ ��Ŀ��ֵ������ɸõ����CAN֡
    std::size_t packCommand(CanFrame outFrames[], std::size_t maxFrames) override;

    // 3. �����롿���� updateFeedback
    //    �����յ���CAN֡������ rpm_, angle_ �ȳ�Ա����
    void updateFeedback(const CanFrame& cf) override;

    // 4. �����롿���� matchesFrame
    //    �ж��յ���CAN֡�Ƿ�����������
    bool matchesFrame(const CanFrame& cf) const override;

    // 5. �����롿���� get_GearRatio
    //    ���ظõ������ʵ���ٱ�
    float get_GearRatio() const override { return 27.0f; } // ������ٱ���27

    // 6. ʵ�� update ����������ִ��PID����
    void update() override;

    // 7. ʵ�� setTarget... �ȿ��ƽӿ�
    void setTargetRPM(float rpm_set) override;

private:
    // �õ����˽�г�Ա����PID������
    PID_Incremental speed_pid_;
};
```

2. �� `Motor_MyMotor.cpp` ��ʵ�ֹ���
```cpp
#include "Motor_MyMotor.h"

std::size_t MyMotor::packCommand(CanFrame outFrames[], std::size_t maxFrames) {
    // ... ���� this->target_current_ ���CAN֡ ...
    // outFrames[0].ID = 0x123;
    // outFrames[0].data[0] = ...;
    return 1; // ���ش����֡��
}

void MyMotor::updateFeedback(const CanFrame& cf) {
    // ... ���� cf.data ...
    // float raw_rpm = ...;
    // this->rpm_ = raw_rpm / get_GearRatio(); // ת��Ϊ�����ת��
}

bool MyMotor::matchesFrame(const CanFrame& cf) const {
    // �ж��߼������磺
    return (cf.ID == (0x200 + this->motor_id_));
}

void MyMotor::update() {
    // ... ����PID���� ...
    // target_current_ = speed_pid_.pid_calc(target_rpm_, this->rpm_);
}

void MyMotor::setTargetRPM(float rpm_set) {
    // ... ����Ŀ��ֵ ...
    this->target_rpm_ = rpm_set;
}
```
3. ��Ӧ�ò�ʹ�� ��ʹ�� `M3508` һ�������� `MyMotor` ���󣬲�����ע�ᵽ `fdCANbus` ���ɡ����������Զ����������һ�С�


   