#ifndef UART_Debug
#define UART_Debug

#define HUART huart3 //在这里修改调试用的串口
#include "stm32f4xx_hal.h" //如果32型号不一样要修改这里
typedef struct 
{
    uint8_t Head;
    uint8_t State;
    uint8_t adden1;
    uint8_t adden2;
    float XData;
    float YData;
    uint8_t End;
}Rec_flame_t;
//作为调试用的指令 统一使用DMA接收 加串口空闲中断的方式来调用 该函数需要在主函数调用一次
void Debug_Init(void);

//该函数需要放在串口中断服务函数里面
void Debug_IT(void);
static void MV_Enable(void);
#endif
