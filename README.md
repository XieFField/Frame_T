## RC10_R1_FRAME_TEST ˵����

### ���
    ���ĵ����ڼ�¼RC10_LIB�����˼·���������������RC10_LIB�����Ʋ��û��ֲᡣ
    ���ĵ�д�Ļ���������ң����ʱ��ֻ��������¼���ߵ��뷨��ʵ��

### ���뷽ʽ
    ͳһʹ��GB2312
### �����淶
    �����еı���ͳһ��_�ĺ�׺���ββ�����׺

### �ļ��ܹ�

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
   4. ��װ�˶�֡�����������Щ����Ƿֶ�֡���͵ģ���ȻĿǰ��û�õĸߣ���֪���Ժ�᲻��������
   5. �ô���fdCAN ��Զ�Ǵ�ͨ�Ų㣬����߼��仯������Ⱦ CAN ������
   6. ==����ʵ��==
      1. fdCAN�ṩ���ͽӿڸ�����࣬�ṩ sendFrame(const CanFrame&) �ӿڣ�����಻��ֱ�ӵ��� HAL��
      2. ��fdCANbus��ע����,ʹ��Motor_Baseָ�룬�������м̳�Motor_Base�����඼����ע��
      3. fdCAN����ISR�е����ݰ��������У��õ���������
      4. ʵ��CAN����Ƶ��Ϊ1kHz����ش�Ƶ��һ�¡��ڲ���һ�� 1kHz ����������ͳһ���ȹ��صĵ����schedulerTask()��1ms ����һ�Σ��������ص�����ռ� packCommand() �Ľ����ͳһ sendFrame()��
         1. 1 ms ��ʱ �� ���� motorList[]��
         2. ��ÿ�� motor ���� motor->packCommand()��
         3. ���ࣨ���� DJIMotor���� packCommand() �У��ѵ�ǰ target ת���ɶ�ӦЭ��ı��ģ�����д�� group �Ļ��棩��
         4. ����� DJI ϵ�У����ͬ��� 4 ������ϲ���һ֡������������������ֱ�ӷ���һ֡��
           
            
      5. ��Ա������FDCAN_HandleTypeDef* hfdcan��bus_id����̬���������ָ��
      6. 

        


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
   2. ��֮��
      1. DJI
         1. ��DJI_Motor����һ�����DJI_Group��֡��
         2. DJIһ��CAN�ϰ˸����������Ƭ֡��id1~4һƬ��һ��canid,5~8һƬ��һ��canid
         3. ֮���������Ҫ�̳�
      2. �������
         1. �̳�Motor_Base��ɸ��Ե�Э�顣
   3. ������ͱ��ĵ����ɺͻ��ձ��ĵĽ����ڵ������ʵ��
   4. ����ʵ��
      1. PID��Ϊ������еĳ�Ա�����ǵ����̳�PID�ࡣ
      2. �ṩͨ�ýӿ�(Motor_Base�����)��

        setTargetRPM() / setTargetCurrent() / setTargetAngle()/setTargetTotalAngle()

        getRPM() / getPosition() / getCurrent() / getTotalAngle()

        packCommand()����Ŀ����ת�� CAN ���ģ�

        unpackFeedback()������������ر��ģ�
        ��֮���ɾ���������ɱջ����Ƶķ�װ��
      3. �ڵ�����а�update()[���µ����Ҫ���Ͳ���] ,��packCommand()[�����������]�ֿ�
         1. ������fdCANbus�еĲ���
         2. 1kHz��ʱ���жϴ��� -> fdcan_global_scheduler_tick_isr() �ͷ��ź��� schedSem_��
         3. schedulerTaskbody ���ź����ȴ��б����ѡ�
         4. schedulerTaskbody ���� motorList_����ÿ��ע��ĵ������ m->update()��
         5. �� update() �ڲ��������������״̬���� ANGLE_CONTROL��ִ��PID���㣬���������ڲ��� target_current_��
         6. schedulerTaskbody �ٴα��� motorList_������ m->packCommand()��
         7. packCommand()���� DJI_Group ��ʵ�֣���ȡ�ո��� update() ������� target_current_������������CAN֡��
         8. schedulerTaskbody �����д���õ�֡ͨ��   sendFrame() ���ͳ�ȥ��
      4. DJI_Motor ����

        ���� DJI ������õĴ��Э�飨4 �����֡����

        �����ͺţ�M3508��M2006��GM6020���̳�����࣬������巴��������
         1. DJI_Motor�̳�Motor_Base
               1. ���𱣴��������id,�����ش�����`updateFeedback()`���ṩ�ӿڣ�������Group���
               2. M3508/M2006��M6020����һ��CAN��(���˷�busλ��)
               3. DJI_Motor��DJI_Group
                  1. DJI_Motor�Ǹ��𵥵��,רע�ڷ���������״̬�洢
                  2. DJI_Group������֡
                  3. DJI_Motor��DJI_Group���кͼ�����
         2. ��̳��� M3508/M2006
            1. �������ͽ���Э��һ����ֻ����������ͬ��
         3. GM6020
            1. ֻ��֡ͷ�������Ǹ���ͬ
            2. ����
      5. �̰߳�ȫ
         1. fdCANbus��rxTask�����updateFeedback(д����),��schedulerTask���ȡ��Щ�ֶν���packCommand()(������������)�������Ҫ�Թ���������һ��������
         2. ������Ŀǰ˼·�����֣�һ����Motor�ķ����ͷ�������ʹ���������Ļ�������������taskENTER_CRITICAL()�����ڱ���.
      6. matchesFrame ��Ĭ��ʵ������չ
         1. ����������ʵ��Ĭ����Ϊ���Ƚ� id_ �� isExtended_�������������� override������ DJI group Ҫƥ�� group-feedback frame ���ַ�����Ա����
         2. ��ʵҲ���԰�matchFrameɾ�ˣ�Ȼ��ֱ�ӵ���fdCANbus��matchesFrameDefualt����ʵҲ��ʵ�ֵȼ��߼�
      7. ����ע��Ψһ�Լ��(IMPORTANT!)
      8. �����������Ӧ���Ǻ͵�Ƭ���������ڵȼۣ��о�û���������ı�Ҫ��
      9.  


    


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

```scss
                        ������������������������������������������������������
                        ��        FDCAN HW         ��
                        ��   (FIFO0, TX FIFO)      ��
                        �������������������������Щ���������������������������
                                    �� RX FIFO �ж�
                                    ��
                        ������������������������������������������������������
                        �� fdcan_global_rx_isr()   ��
                        �� (ȫ��C�ص����ҵ�bus)   ��
                        �������������������������Щ���������������������������
                                    �� pushRxFromISR()
                                    ��
                        ������������������������������������������������������
                        ��     fdCANbus::rxQueue_  ��
                        ��   (RtosQueue<CanFrame>) ��
                        �������������������������Щ���������������������������
                                    ��
                       RxTask wakes�� recv() from queue
                                    ��
                        ������������������������������������������������������
                        ��   fdCANbus::rxTaskbody  ��
                        ��   ���� motorList_       ��
                        ��   motor->matchesFrame() ��
                        ��   motor->updateFeedback ��
                        �������������������������Щ���������������������������
                                    ��
                         �����������������������ة���������������������
                         ��                     ��
               ��������������������������������������   ��������������������������������������
               ��   Motor_Base    ��   ��  Motor_Subclass ��
               �� (base class)    ��   �� (DJI_M3508 etc.)��
               �� updateFeedback()��   �� override packs  ��
               ��  getRPM()/...   ��   �� specific logic ��
               ��������������������������������������   ��������������������������������������
                                    ��
                                    �� packCommand()
                                    ��
                        �������������������������ة�������������������������
                        �� fdCANbus::schedulerTask��
                        �� (1kHz ����)            ��
                        �� ���� motorList_        ��
                        �� �ռ��� motor packCommand��
                        �� sendFrame()             ��
                        �������������������������Щ�������������������������
                                    ��
                        �������������������������ة�������������������������
                        ��   FDCAN TX FIFO         ��
                        ��   HAL_FDCAN_AddMessage  ��
                        ������������������������������������������������������


```


#### User��
    ���ڴ�Ż���RC10_LIB��д��Ӧ�ò㣬����������࣬Debug�࣬demo�ࡣ
    �Լ�ʵ������Ҫ����������������

### ��������Э���涨
1. �����о���д����ע�ͣ�����Լ�����д����ʹ��vscode�Դ���ai���в�ȫ�����ߵ�ע��Ҳ��������aiд�ġ�
2. 


