#ifndef ESP_Debug
#define ESP_Debug

#define ESPUART huart1 //在这里修改调试用的串口
#include "stm32f4xx_hal.h" //如果32型号不一样要修改这里

//作为调试用的指令 统一使用DMA接收 加串口空闲中断的方式来调用 该函数需要在主函数调用一次
void ESP_Init(void);

//该函数需要放在串口中断服务函数里面
void ESP_IT(void);

#endif
