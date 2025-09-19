#include "frame_demo.h"

// 使用 volatile 防止编译器优化，确保在调试时可以观察到值的变化
volatile int counter = 0;

void FrameDemo::loop()
{
    
    counter++;


}

void FrameDemo::init()
{
    start(osPriorityNormal, 256);
}
