#include "BSP_fdCAN_Driver.h"

// --- ȫ�ֱ����뺯�� ---
// ���֧��3��FDCAN����ʵ��
static fdCANbus* g_fdcan_bus_map[3] = {nullptr, nullptr, nullptr};

/**
 * @brief ע��һ��fdCANbusʵ���Խ���ȫ���ж�·��
 * @param bus ָ��fdCANbusʵ����ָ��
 */
void register_fdcan_bus_for_isr(fdCANbus* bus) 
{
    if (bus && bus->getbusID() > 0 && bus->getbusID() <= 3) 
        g_fdcan_bus_map[bus->getbusID() - 1] = bus;
    
}


                                    

// --- init: ������������ CAN��filter/interrupt�� ---
void fdCANbus::init() 
{
    FDCAN_FilterTypeDef sFilterConfig = {0};
    sFilterConfig.FilterType = FDCAN_FILTER_MASK;
    sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    // ����һ���������б�׼֡���˲���
    sFilterConfig.IdType = FDCAN_STANDARD_ID;
    sFilterConfig.FilterIndex = 0;
    
    sFilterConfig.FilterID1 = 0x000;
    sFilterConfig.FilterID2 = 0x000; // MaskΪ0����ʾ�������κ�λ��ȫ������
    HAL_FDCAN_ConfigFilter(hfdcan_, &sFilterConfig);

    // ����һ������������չ֡���˲���
    sFilterConfig.IdType = FDCAN_EXTENDED_ID;
    sFilterConfig.FilterIndex = 1; // ʹ�ò�ͬ���˲�������
    sFilterConfig.FilterID1 = 0x00000000;
    sFilterConfig.FilterID2 = 0x00000000;
    HAL_FDCAN_ConfigFilter(hfdcan_, &sFilterConfig);

    // ����FDCANӲ��
    if (HAL_FDCAN_Start(hfdcan_) != HAL_OK) 
    {
        // ������
        HAL_FDCAN_Start_ERROR = true;
    }

    // ����FIFO0����Ϣ�ж�
    if (HAL_FDCAN_ActivateNotification(hfdcan_, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
        // ������
        HAL_FDCAN_ActivateNotification_ERROR = true;
    }

    // ע������ȫ��ӳ���
    register_fdcan_bus_for_isr(this);

    //��������
    rxTask_.start(tskIDLE_PRIORITY + 3, 256);
    schedulerTask_.start(tskIDLE_PRIORITY + 4, 256);
}

// --- registerMotor ---
bool fdCANbus::registerMotor(Motor_Base* m) 
{
    for (std::size_t i = 0; i < MAX_MOTORS; ++i) 
    {
        if (motorList_[i] == nullptr) 
        {
            motorList_[i] = m;
            return true;
        }
    }
    return false;
}

// --- sendFrame: ���� HAL ���� ---
bool fdCANbus::sendFrame(const CanFrame& cf) 
{
    // ʹ�û���������Ӳ�����ʣ�����1ms�ĳ�ʱ�ȴ�
    if (xSemaphoreTake(tx_mutex_, pdMS_TO_TICKS(1)) != pdTRUE) 
        return false; // ��ȡ��ʧ�ܣ�ֱ�ӷ��أ���������������
    

    if (!hfdcan_) 
    {
        xSemaphoreGive(tx_mutex_);
        return false;
    }

    FDCAN_TxHeaderTypeDef tx_header;
    std::memset(&tx_header, 0, sizeof(tx_header));
    tx_header.Identifier = cf.ID;
    tx_header.IdType = (cf.isextended ? FDCAN_EXTENDED_ID : FDCAN_STANDARD_ID);
    tx_header.TxFrameType = FDCAN_DATA_FRAME;
    tx_header.DataLength = FDCAN_DLC_BYTES_8; // ���е������8�ֽ�
    tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    tx_header.BitRateSwitch = FDCAN_BRS_OFF;
    tx_header.FDFormat = FDCAN_CLASSIC_CAN;
    tx_header.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    tx_header.MessageMarker = 0;

    HAL_StatusTypeDef status = HAL_FDCAN_AddMessageToTxFifoQ(hfdcan_, &tx_header, const_cast<uint8_t*>(cf.data));

    xSemaphoreGive(tx_mutex_); // �ͷ���

    return status == HAL_OK;
}

// --- pushRxFromISR ---
bool fdCANbus::pushRxFromISR(const CanFrame& cf, BaseType_t* pxHigherPriorityTaskWoken) 
{
    return rxQueue_.sendFromISR(cf, pxHigherPriorityTaskWoken);
}

// --- rxTaskBody ---
void fdCANbus::rxTaskbody() 
{
    CanFrame cf;
    for (;;) 
    {
        if (rxQueue_.recv(cf, portMAX_DELAY)) 
        {
            for (std::size_t i = 0; i < MAX_MOTORS; ++i) 
            {
                Motor_Base* m = motorList_[i];
                if (m && m->matchesFrame(cf)) 
                   m->updateFeedback(cf);
                
            }
        }
    }
}

// --- schedulerTaskBody ---
void fdCANbus::schedulerTaskbody() 
{
    CanFrame frames_to_send[MAX_MOTORS * 2];
    for (;;) 
    {
        // ���������ȴ���ֱ����TIM�жϵ�ISR����
        xSemaphoreTake(schedSem_, portMAX_DELAY);
        std::size_t frameCnt = 0;
        for (std::size_t i = 0; i < MAX_MOTORS; ++i) 
        {
            Motor_Base* m = motorList_[i];
            if (!m) continue;
            frameCnt += m->packCommand(&frames_to_send[frameCnt], (sizeof(frames_to_send)/sizeof(frames_to_send[0])) - frameCnt);
            if (frameCnt >= (sizeof(frames_to_send)/sizeof(frames_to_send[0]))) break;
        }
        for (std::size_t j = 0; j < frameCnt; ++j)
            sendFrame(frames_to_send[j]);
    }
}


// --- matchesFrameDefault ---
bool fdCANbus::matchesFrameDefault(const CanFrame& cf, uint32_t targetId, bool isExt) {
    return (cf.isextended == isExt) && (cf.ID == targetId);
}

// --- RxTask / SchedTask loop impl & ctor ---
fdCANbus::RxTask::RxTask(fdCANbus* parent)
: RtosTask("fdcan_rx"), parent_(parent) 
{

}

void fdCANbus::RxTask::run() 
{
    parent_->rxTaskbody();
}

fdCANbus::SchedTask::SchedTask(fdCANbus* parent)
: RtosTask("fdcan_sched"), parent_(parent) 
{
    
}

void fdCANbus::SchedTask::run() 
{
    parent_->schedulerTaskbody();
}

// --- ȫ��C���ص����� ---

/**
 * @brief ȫ�ֵ�FDCAN�����жϴ�����
 * @param hfdcan �����жϵ�FDCAN���
 */
extern "C" void fdcan_global_rx_isr(FDCAN_HandleTypeDef* hfdcan) 
{
    fdCANbus* target_bus = nullptr;
    for (int i = 0; i < 3; ++i) 
    {
        if (g_fdcan_bus_map[i] && g_fdcan_bus_map[i]->getFDCANHandle() == hfdcan) 
        {
            target_bus = g_fdcan_bus_map[i];
            break;
        }
    }

    if (!target_bus) 
        return;

    CanFrame rx_frame;
    FDCAN_RxHeaderTypeDef rx_header;
    if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rx_header, rx_frame.data) == HAL_OK) 
    {
        rx_frame.ID = rx_header.Identifier;
        rx_frame.isextended = (rx_header.IdType == FDCAN_EXTENDED_ID);
        rx_frame.DLC = (rx_header.DataLength >> 16) & 0x0F;
        BaseType_t higher_priority_task_woken = pdFALSE;
        target_bus->pushRxFromISR(rx_frame, &higher_priority_task_woken);
        portYIELD_FROM_ISR(higher_priority_task_woken);
    }
}

/**
 * @brief ȫ�ֵĵ�����Tick�жϴ�����
 */
extern "C" void fdcan_global_scheduler_tick_isr() 
{
    BaseType_t higher_priority_task_woken = pdFALSE;
    for (int i = 0; i < 3; ++i) 
    {
        if (g_fdcan_bus_map[i]) 
        {
            xSemaphoreGiveFromISR(g_fdcan_bus_map[i]->schedSem_, &higher_priority_task_woken);
        }
    }
    portYIELD_FROM_ISR(higher_priority_task_woken);
}

/**
 * @brief  ��д HAL_FDCAN_RxFifo0Callback ������
 * @param  hfdcan FDCAN���
 * @param  RxFifo0ITs FIFO0�жϱ�־
 * @note   �˺������� HAL_FDCAN_IRQHandler �б��Զ�����
 */
extern "C" void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
  if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
  {
    // ֱ�ӵ���ȫ���жϴ�����
    fdcan_global_rx_isr(hfdcan);
  }
}
