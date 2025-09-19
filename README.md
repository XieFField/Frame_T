## RC10_R1_FRAME_TEST ˵����

### ���뷽ʽ
    ͳһʹ��GB2312
### �����淶
    �����еı���ͳһ��_�ĺ�׺���ββ�����׺

### �ļ��ܹ�
#### ���
    ���ĵ����ڼ�¼RC10_LIB�����˼·���������������RC10_LIB�����Ʋ��û��ֲ�
1. BSP_Driver
    �����ڴ����ײ���������fdCAN, UASRT, SPI IIC, TIM RTOS��������
    ǰ׺Ϊ==BSP_==
2. Motor
    �����ڴ�ŵ������.
    ǰ׺Ϊ==Motor==
3. APP
    �����ڴ�ſ��������˲�����һЩ���ߣ������������������ǿ���㷨֮��ġ�
    ǰ׺Ϊ==APP==
4. Module
    ������һЩ������ǿ��ģ��ķ�װ���缤����ģ�顢�ƴ��ȵ�
    ��ǰ׺Ϊ==Module==

##### Ŀǰ�����˼·
��ͳһ��FdCanBus ��װ����fdCANӲ����������RX�ַ�����Motor���ඨ��ͳһ�ӿڣ���motor�����������������Ҫ���ࣺ`DJIMotor`��`ExtendedMotor`�������������ֵ���ı��ķ��ͻ��Ʋ�ͬ��ʹ��FreeRTOS(����\����)��CAN���շ��������ƽ��ʹ��IDӳ�����ʽ�����ձ��ķַ�����ȷ�ĵ������
1. fdCanBus�������
   ��Ϊͨ��ͨ����������ֱ�ӷ���������RC9�Ĳ�ͬ�㣩
   1. ��·CAN�ܹ�����׼֡����չ֡
   2. ʹ��FIFO����CAN֡��ISR�򻯣�ֻ���˱��ģ��������������ŵ�RTOS�����н���
   3. fdCanBus����������Զ����ɶ�Ӧ����
   4. �ô���fdCAN ��Զ�Ǵ�ͨ�Ų㣬����߼��仯������Ⱦ CAN ������
   5. ==����ʵ��==
      1. fdCAN�ṩ���ͽӿڸ�����࣬�ṩ sendFrame(const CanFrame&) �ӿڣ�����಻��ֱ�ӵ��� HAL��
      2. fdCAN����ISR�е����ݰ��������У��õ���������
      3. ʵ��CAN����Ƶ��Ϊ1kHz����ش�Ƶ��һ�¡��ڲ���һ�� 1kHz ����������ͳһ���ȹ��صĵ����schedulerTask()��1ms ����һ�Σ��������ص�����ռ� packCommand() �Ľ����ͳһ sendFrame()��
         1. 1 ms ��ʱ �� ���� motorList[]��
         2. ��ÿ�� motor ���� motor->packCommand()��
         3. ���ࣨ���� DJIMotor���� packCommand() �У��ѵ�ǰ target ת���ɶ�ӦЭ��ı��ģ�����д�� group �Ļ��棩��
         4. ����� DJI ϵ�У����ͬ��� 4 ������ϲ���һ֡������������������ֱ�ӷ���һ֡��
      4. ��Ա������FDCAN_HandleTypeDef* hfdcan��bus_id����̬���������ָ��
      5. 

        

``` mermaid
flowchart TD

    subgraph Scheduler["fdCANbus �������� (1 kHz, ÿ�� CAN һ����)"]
        T1["���ڴ��� (1ms)"]
        T2["���� motorList[]"]
        T3["���� packCommand()"]
        T4["��֡ Frame"]
        T5["���� sendFrame()"]
        T1 --> T2 --> T3 --> T4 --> T5
    end

    subgraph MotorList["����б� (��̬����, ���8��)"]
        M1["DJI Motor 1..4"] 
        M2["DJI Motor 5..8"]
        M3["Other Motor 1"]
        M4["Other Motor 2"]
        T2 --> M1
        T2 --> M2
        T2 --> M3
        T2 --> M4
    end

    subgraph fdCAN["fdCANbus ����"]
        C1["sendFrame()<br/>�ײ� HAL ����"]
        C2["�����ж� ISR ���Ͷ���"]
        C3["��������������ַ������"]
        T5 --> C1
        C2 --> C3
    end

    subgraph MotorUpdate["�����������"]
        U1["�����<br/>updateFeedback(frame)"]
        C3 --> U1
    end
```
1. FreeRTOS�������
   1. �����Ӧ�ĸ��࣬�ⲿ������ʱû���̫��
   2. ����ϵͳ�࣬�ṩͳһ�ӿ��������͹��������ƹ�CubeMX���������ɡ�
      1. ����ROS�ڵ��е�`spin()`,�̳�����ϵͳ������ֻ��Ҫ����`run`����`loop`
      2. ��ҪĿ���ǰ�RTOS���������Ϊһ�����ܵ�Ԫ
   3. ͨ�ų����࣬��һ����Ҫ��RTOSʵ�֣�һЩ������ͳһ�ĺ���ʵ�ֲ������������廹���е�����ROS�е�pub/sub����service��
      1. Publisher/Subscriber��һ������/�������ĳ�����⣨���У�������Ϣ����һ���ඩ�ĺ��������д���
      2. Service/Client�����ڡ�����/��Ӧ��ģʽ������������á�һ�������
   4. �ô����Ժ�ֻ�� CAN�������Խ� UART��SPI���������ȣ����ܹ������ RTOS ͨ�ſ���
   5. ����ʵ��
      1. ������ȣ������ࣩ����װ FreeRTOS TaskHandle_t��ͳһ�������񴴽��������������߼���
      2. ͨ�Ż��ƣ���Ϣ/�����ࣩ����һ������ ROS topic/service �ĸ��࣬������һ������ȫʹ��FreeRTOS��queue֮������ͨ�š�
         1. ģ�� ROS �� pub/sub��
            1. publish(msg)
            2. subscribe(callback)








   
2. �����װ��ʵ��
   1. ������һ��Motor_Base�����࣬��Ϊ���࣬ͳһ�������Ҫ��ͨ�ýӿڱ���������������д��
   2. ��Motor_Base��DJI�������һ��ר��DJI�࣬��ΪDJI������ĵ������ԣ�һ֡CAN���ͱ��ĵ�����֡�����ĸ�����������ݣ�
   3. ������ͱ��ĵ����ɺͻ��ձ��ĵĽ����ڵ������ʵ��
   4. ����ʵ��
      1. �ṩͨ�ýӿ�(Motor_Base�����)��

        setTargetRPM() / setTargetCurrent() / setTargetAngle()/setTargetTotalAngle()

        getRPM() / getPosition() / getCurrent() / getTotalAngle()

        packCommand()����Ŀ����ת�� CAN ���ģ�

        unpackFeedback()������������ر��ģ�
      2. DJI_Motor ����

        ���� DJI ������õĴ��Э�飨4 �����֡����

        �����ͺţ�M3508��M2006��GM6020���̳�����࣬������巴��������

      3. ExtMotor ����

        �������֡�Ŀ��Ʒ�ʽ���� VESC��GO-M8010�������

        ���ͺ�ʵ���Լ��� packCommand() & unpackFeedback()��
    


��˼ά��ͼ(���/���ϵ)
```mermaid
flowchart TB
  subgraph RTOS_Wrapper["RTOS ��װ"]
    RT_Task["RtosTask\n(�������)"]
    RT_Topic["RtosTopic\n(Pub/Sub ����)"]
  end

  subgraph fdCANbus_layer["fdCANbus �� (ÿ· CAN һ��ʵ��)"]
    fdCAN["fdCANbus\n- hfdcan\n- bus_id\n- motorList[��8]\n- rxQueue\n- schedulerTask(1kHz)"]
    DJIGroup["DJIMotorGroup\n(���� 4-in-1 ���/���)"]
  end

  subgraph Motor_layer["�����"]
    Motor["Motor (����)\n- packCommand()\n- updateFeedback()\n- targets/status\n- ���� fdCANbus* (���)"]
    DJIMotor["DJIMotor : Motor\n- ����ĳ�� (group_id)\n- Э�飺4 �� 1"]
    OtherMotor["OtherMotor : Motor\n- VESC / Damiao / GO-M8010 Adapter\n- Э�飺1 ��� = 1 ֡"]
  end

  %% ��ϵ
  RT_Task --- RT_Topic
  fdCAN -->|����/����| Motor
  Motor -->|ʹ�� (has-a)| fdCAN
  DJIMotor -->|����| DJIGroup
  fdCAN -->|�ɺ�| DJIGroup

  %% multi-bus hint
  subgraph BUSES["Ӳ��: ��· FDCAN��bus1..bus3��"]
    bus1["fdCANbus (bus1)"]
    bus2["fdCANbus (bus2)"]
    bus3["fdCANbus (bus3)"]
  end
  bus1 --> fdCAN
  bus2 --> fdCAN
  bus3 --> fdCAN

```

����ʱ��ͼ
```mermaid
flowchart TD
    subgraph SCHED["fdCANbus Scheduler (per CAN, 1kHz)"]
      Tick["��ʱ���� 1ms"]
      ForLoop["���� motorList[]"]
      Pack["���� motor.packCommand()"]
      Batch["DJI: group �ϲ� -> 1 ֡\nOthers: ��֡"]
      send["fdCAN.sendFrame(frame) -> HAL ����"]
      Tick --> ForLoop --> Pack --> Batch --> send
    end

    subgraph BUS["CAN ���� & Ӳ��"]
      CANBUS["���� CAN ����"]
      HAL["HAL/FDCAN Ӳ����"]
    end

    subgraph RX["����·��"]
      ISR["FDCAN Rx ISR\n(������)"]
      ISR_Queue["RX ԭʼ֡���� (rtos topic/queue)"]
      RX_Task["fdCAN RX Task\n�Ӷ��� pop -> publish"]
      Dispatch["�� ID/����ַ������� motor\n���� motor.updateFeedback()"]
      ISR --> ISR_Queue --> RX_Task --> Dispatch
    end

    %% ���� send -> bus -> isr
    send --> HAL --> CANBUS --> ISR

    %% motor update interaction
    Dispatch --> MotorUpdate["Motor ����״̬\n(�Ƕ�/�ٶ�/����)"]

    %% note: motor may update targets via other control tasks

```
```
                +-----------------------------+
                |        TIMx ��ʱ��          |
                | (1kHz Update Event / IRQ)   |
                +-------------+---------------+
                              |
                              v
                   +---------------------+
                   |  Scheduler Task     |
                   | (�����ȴ��ź���)     |
                   +----------+----------+
                              |
                              v
          ------------------------------------------------
          |                      |                       |
  +-------+------+      +--------+-------+      +--------+-------+
  |  DJI Group   |      |  DJI Group    |      |  Other Motor   |
  | (4 Motors)   |      | (4 Motors)    |      | (Single Frame) |
  +--------------+      +----------------      +-----------------+
          |                       |                       |
          v                       v                       v
    +-----------+           +-----------+           +-----------+
    | CAN Frame |           | CAN Frame |           | CAN Frame |
    |  (4x cmd) |           |  (4x cmd) |           | (1x cmd ) |
    +-----+-----+           +-----+-----+           +-----+-----+
          \------------------------|-----------------------/
                                   |
                            +------+------+
                            |   fdCANbus   |
                            | sendFrame()  |
                            +--------------+


```

```

```


#### User��
    ���ڴ�Ż���RC10_LIB��д��Ӧ�ò㣬����������࣬Debug�࣬demo�ࡣ
    �Լ�ʵ������Ҫ����������������

### ��������Э���涨
1. �����о���д����ע�ͣ�����Լ�����д����ʹ��vscode�Դ���ai���в�ȫ�����ߵ�ע��Ҳ��������aiд�ġ�
2. 


