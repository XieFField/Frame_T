#include "frame_demo.h"

// ʹ�� volatile ��ֹ�������Ż���ȷ���ڵ���ʱ���Թ۲쵽ֵ�ı仯
volatile int counter = 0;

void FrameDemo::loop()
{
    
    counter++;


}

void FrameDemo::init()
{
    start(osPriorityNormal, 256);
}
