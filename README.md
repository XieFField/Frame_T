## RC10_R1_FRAME_TEST ˵����

### ���
    ���ĵ����ڼ�¼RC10_LIB�����˼·���������������RC10_LIB�����Ʋ��û��ֲᡣ
    ���ĵ�д�Ļ���������ң����ʱ��ֻ��������¼���ߵ��뷨��ʵ��

    һ�������������ϵ�������ϵľͱ任Ϊ����ϵ��yaw����ʱ����תΪ������(���ֶ���)
    ����ʹ��arm_math����м��ٵģ�����ʹ��

    ��ʵ���READMEҲ��ʼ�������AIд�Ķ����ˣ��������ֻ��Ϊ������������չRC10_LIBʱ�򿴵�
    

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
      4. ʵ��CAN����Ƶ��Ϊ1kHz����ش�Ƶ��һ�¡���ͨ���ڲ�һ��1kHz�ĵ�����������ɣ�������ͳһ��������ע�ᵽ�����ϵĶ���
         **�������̵�����ʵ��**��
         1. **˫ע��**: �û���Ҫ��**���������**���� `m3508_1`����**��������**���� `DJI_Group_1`����ע�ᵽ `fdCANbus`��
            *   ע����������Ϊ���õ������ܵ����� `update()` ���������ý���������ͨ�� `matchesFrame()` �ҵ��������� `updateFeedback()`��
            *   ע��������Ϊ���õ������ܵ����� `packCommand()` �������������ָ�
         2. **1kHz��ʱ���ж�** �������ͷ� `schedulerTask_` ���ź�����
         3. `schedulerTask_` �����ѣ���ʼִ�����ֱ�����
            *   **��һ�ֱ��� (Update)**: ���� `motorList_`����ÿ��ע��Ķ������ `update()` ��������ʱ��`m3508_1->update()` �ᱻ���ã�ִ��PID���㲢�������ڲ��� `target_current_`���� `DJI_Group_1->update()` �ǿպ�������ִ���κβ�����
            *   **�ڶ��ֱ��� (Pack & Send)**: �ٴα��� `motorList_`����ÿ��������� `packCommand()`����ʱ��`m3508_1->packCommand()` �ǿպ������� `DJI_Group_1->packCommand()` �ᱻ���ã�����������Ա `m3508_1` �� `target_current_` ֵ������������CAN֡��
         4. `schedulerTask_` �������ռ�����֡ͨ�� `sendFrame()` ���ͳ�ȥ��
         ������ƾ�ȷ�ط�����ְ�𣺵����������㣬��������������

      5. ��Ա������FDCAN_HandleTypeDef* hfdcan��bus_id����̬���������ָ��
      6. 

        


2. FreeRTOS�������
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








   
3. �����װ��ʵ��
   1. ������һ��Motor_Base�����࣬��Ϊ���࣬ͳһ�������Ҫ��ͨ�ýӿڱ���������������д��
   2. **������ƣ����ռ�ת����߶�ͳһ**
      *   **���ռ�ת��**: �� `DJI_Motor::updateFeedback()` �����У���CAN�����յ���ԭʼ���ת�����ݣ�������ֵ��ת�٣���**����**ͨ������ `virtual float get_GearRatio() const` ���������ȷ�ļ��ٱȣ�����ת��Ϊ**�����߶�**�����ݡ�
      *   **״̬ͳһ**: ת�������д洢�� `Motor_Base` �еĳ�Ա������`rpm_`, `angle_`, `totalAngle_`����ͳһΪ**������״̬**��
      *   **���Ʊջ�ͳһ**: ����PID���ƻ�·���� `update()` �����У���Ŀ��ֵ��`target_rpm_`���ͷ���ֵ��`this->rpm_`�������������߶Ƚ��м��㣬ȷ���˿��Ƶ���ȷ�ԡ�
   3. ��֮��
      1. DJI
         1. ��DJI_Motor����һ�����DJI_Group��֡��
         2. DJIһ��CAN�ϰ˸����������Ƭ֡��id1~4һƬ��һ��canid,5~8һƬ��һ��canid
         3. ֮���������Ҫ�̳�
      2. �������
         1. �̳�Motor_Base��ɸ��Ե�Э�顣
   4. ������ͱ��ĵ����ɺͻ��ձ��ĵĽ����ڵ������ʵ��
   5. ����ʵ��
      1. PID��Ϊ������еĳ�Ա�����ǵ����̳�PID�ࡣ
      2. �ṩͨ�ýӿ�(Motor_Base�����)��

        setTargetRPM() / setTargetCurrent() / setTargetAngle()/setTargetTotalAngle()

        getRPM() / getPosition() / getCurrent() / getTotalAngle()

        packCommand()����Ŀ����ת�� CAN ���ģ�

        updateFeedback()������������ر��ģ�
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
         1. `rxTask` (��������) �� `schedulerTask` (��������) ֮��������ݹ����� `rpm_`, `angle_`����`rxTask` ��д���ߣ�`schedulerTask` �Ƕ�ȡ�ߡ����� `schedulerTask` �����ȼ����ߣ������ڵ�ǰ����У����ݶ�ȡ����ԭ�Ӳ����������ϴ������ݾ����ķ��գ�������1kHz�ĵ���Ƶ����ʵ�ʷ����ĸ��ʽϵͣ���
         2. **��ǰ����**����ʱδ������ʽ������������FreeRTOS��������Ⱥ��������͵�ԭ���ԣ�float/int32��32λ����ͨ����ԭ�Ӷ�д�ģ���������⡣���δ���������ݲ�һ�µ����⣬���Կ����� `updateFeedback` �� `update` �жԹ������ݿ�ʹ�� `taskENTER_CRITICAL()` / `taskEXIT_CRITICAL()` ���б�����
      6. matchesFrame ��Ĭ��ʵ������չ
         1. ����������ʵ��Ĭ����Ϊ���Ƚ� id_ �� isExtended_�������������� override������ DJI group Ҫƥ�� group-feedback frame ���ַ�����Ա����
         2. ��ʵҲ���԰�matchFrameɾ�ˣ�Ȼ��ֱ�ӵ���fdCANbus��matchesFrameDefualt����ʵҲ��ʵ�ֵȼ��߼�
      7. ����ע��Ψһ�Լ��(IMPORTANT!)
      8. �����������Ӧ���Ǻ͵�Ƭ���������ڵȼۣ��о�û���������ı�Ҫ��


    


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
4. ����(USART/USB)�ײ��������Լ�ROS-like�Ŀ����ͨ�Ż��ơ�
   1. USART/USB
      1. USART/USB����Ӳ���� SerialDevice
         1. **���˼��**���ϸ���ѭ `fdCANbus` �����������ƾ��顣`SerialDevice` ֻ���ġ���Ρ��շ��ֽڿ飬�����������޸�֪����ͨ������ע���Э������� `I_Protocol` ����������֡�Ĵ���������
         2. **���ͻ���**��ʹ�� FreeRTOS ��**������ (Mutex)** ����Ӳ�����ͽӿڡ��κ������뷢�����ݣ������Ȼ�ȡ����������ɺ����жϻص����ͷ�������ȷ���˶����񻷾��·��Ͳ������̰߳�ȫ��
         3. **���ջ���**��ʹ�� FreeRTOS ��**���� (Queue)** ��Ϊ�ж�������֮���������Ӳ�������жϣ��� UART IDLE �жϣ����յ�һ�����ݿ�󣬽����װ��һ���ṹ�岢����������ն��� `rx_queue_`��Ȼ���˳��жϡ�һ�������� `RxTask` ����������ȴ��ö��У��Ӷ�ʵ���ж������ݴ������ȫ���
      2. Э������� I_Protocol
         1. �����ˡ�ʲô������Ч�����ݰ�����������ֽ����н���������֡��`parse_stream`���ͽ�Ӧ�ò����ݴ�����ֽ�����`pack_frame`����
      3. ����ӿ�������ע��
         1. ����һ�� `I_Protocol` ���������Ϊ�ӿڡ��κξ���Э�飨��Modbus���Զ���Э�飩���̳в�ʵ�ִ˽ӿڡ�
         2. �ڴ��� `SerialDevice` ����ʱ����һ������� `I_Protocol` ʵ�֡�ע�롱��ȥ��ʵ��Ӳ����Э��Ľ��
      4. ����ʵ�֣���AI���ɣ������ο���
         1.  **Э��ӿ� (I_Protocol)**
             1.  ����������ƵĹؼ�����������ͨ��Э��ġ���ء���
             2.  ˼·�� ����һ��������� `I_Protocol`����ǿ�����о����Э��ʵ�ֶ������ṩ�������Ĺ��ܣ�
                 1. `parse_stream()`: ��һ���������ֽ����У����Խ�����һ���������������ݰ���
                 2. `pack_frame()`: ��һ���ṹ�������ݶ�������һ��׼�����͵��ֽڡ�
               ```cpp
               // I_Protocol.h (α����) [�����ο�] 

               // ��������һ�������������ݰ�
               struct DataFrame {
                  uint8_t data[64];
                  size_t  length;
               };

               // Э��ӿ�
               class I_Protocol {
               public:
                  virtual ~I_Protocol() = default;

                  /**
                  * @brief ���Դӽ��ջ������н���һ�����������ݰ�
                  * @param buffer ����ԭʼ�ֽ����Ļ�����
                  * @param len �������е����ݳ���
                  * @param frame_callback ��������ɹ���ͨ���˻ص������������ݰ�
                  */
                  virtual void parse_stream(uint8_t* buffer, size_t len, std::function<void(const DataFrame&)> frame_callback) = 0;

                  /**
                  * @brief ��Ӧ�ò����ݴ���ɴ����͵��ֽ�֡
                  * @param app_data    ����Ӧ�ò������
                  * @param out_buffer  �������ֽ��������������
                  * @return �������ֽ���
                  */
                  virtual size_t pack_frame(const YourAppData& app_data, uint8_t* out_buffer) = 0;
               };
               ```
         2. **Ӳ������ (SerialDevice)**
            1. ˼·��
               1. **ͳһ�ӿ�**����һ�� `enum class DeviceType` ������ `UART` �� `USB_VCP`��
               2. **���ͻ��� (�ź�������)**��һ�� `send()` �������ڲ�ʹ�û���������֤ͬһʱ��ֻ��һ�������ܵ��� `HAL_UART_Transmit_DMA` �ȷ��ͺ�����
               3. **���ջ��� (��������)**��ʹ�� `HAL_UARTEx_ReceiveToIdle_DMA` �ȷ�ʽ���ղ��������ݡ��� `HAL_UARTEx_RxEventCallback` �ж�����յ������ݿ飨ָ��+���ȣ�����ɽṹ�壬ͨ�� `xQueueSendFromISR` ���͵� `rx_queue_`��
               4. **�������**��`SerialDevice` �ڲ�����һ�� `RxTask` ʵ�������� `fdCANbus`�����������������һ��ѭ������������ `xQueueReceive` �ϵȴ� `rx_queue_` �����ݣ��յ������ `I_Protocol` �� `parse_stream` ���н�����
               5. **�ص�����**��`parse_stream` ����������������֡��ͨ���ص�����֪ͨ�ϲ�Ӧ�á�
            2. α���룺[AI���ɣ������ο�]
               ```cpp
               // SerialDevice.h (α����) [�����ο�]

               #include "I_Protocol.h"
               #include "BSP_RTOS.h" // ���� RtosTask �� RtosQueue

               // �����豸����
               enum class DeviceType { UART, USB_VCP };

               // �ж�������е����ݽṹ
               struct RxDataBlock {
                   uint8_t* buffer;
                   size_t   size;
               };

               class SerialDevice {
               public:
                  SerialDevice(DeviceType type, I_Protocol& protocol, UART_HandleTypeDef* uart_handle = nullptr);
                  void init(); // ���������Ӳ��
                  bool send(const uint8_t* data, size_t len, uint32_t timeout_ms = 10);
                  void register_receive_callback(std::function<void(const DataFrame&)> callback);

                  // --- �жϻص���� ---
                  void tx_cplt_callback(); // �� HAL_UART_TxCpltCallback �е���
                  void rx_event_callback(uint16_t size); // �� HAL_UARTEx_RxEventCallback �е���

               private:
                  void rx_task_body(); // RxTask ����ѭ����

                  // ---- ��Ա���� ----
                  DeviceType type_;
                  I_Protocol& protocol_;
                  UART_HandleTypeDef* uart_handle_;
                  
                  // ���ͻ���
                  SemaphoreHandle_t tx_mutex_;
                  int tx_retry_count_ = 10;

                  // ���ջ���
                  RtosQueue<RxDataBlock> rx_queue_{16}; // ���ն���
                  uint8_t rx_dma_buffer_[RX_BUFFER_SIZE]; // DMA���ջ�����
                  std::function<void(const DataFrame&)> on_frame_received_callback_;

                  // Rx����
                  class RxTask : public RtosTask {
                  public:
                      explicit RxTask(SerialDevice* parent) : RtosTask("SerialRx", 0), parent_(parent) {}
                  protected:
                      void run() override { parent_->rx_task_body(); }
                  private:
                      SerialDevice* parent_;
                  };
                  RxTask rx_task_{this};
               };
               ```


