#include "uart_esp.h"
#include "bsp.h"
#include "usart.h" //该文件出现问题的时候 说明cube没有把外设单独生成c文件 只需要引用另外一个头文件就可以了
#include "string.h"

#define BUF_MAX 30 //设定接收最高缓存区
uint8_t esprebuf[BUF_MAX]={0};
static void ESP_Handle(void);
//作为调试用的指令 统一使用DMA接收 加串口空闲中断的方式来调用 该函数需要在主函数调用一次
void ESP_Init(void)
{
	HAL_UART_Receive_DMA(&ESPUART,esprebuf,BUF_MAX);
	__HAL_UART_ENABLE_IT(&ESPUART,UART_IT_IDLE);
    HAL_UART_Transmit(&ESPUART,(uint8_t*)"Hi,I am online\n",16,1000);
}

//该函数需要放在串口中断服务函数里面
void ESP_IT()
{
	if(__HAL_UART_GET_FLAG(&ESPUART,UART_FLAG_IDLE))
	{
		HAL_UART_DMAStop(&ESPUART);
			ESP_Handle();//调用数据处理函数
		memset(esprebuf,0,BUF_MAX);
		__HAL_UART_CLEAR_IDLEFLAG(&ESPUART);
		HAL_UART_Receive_DMA(&ESPUART,esprebuf,BUF_MAX);
	}
}

static void ESP_Handle()
{
	switch(esprebuf[0])
    {
        case 0x00:
            RIGHT_Break;
            LEFT_Break;
            HAND_DOWN;
        break;
        case 0x01:
            HAND_UP;
        break;
        case 0x02:
            HAND_DOWN;
        break;
        case 0x03:
            RIGHT_FORWARD(1000);
        break;
        case 0x04:
            LEFT_FORWARD(1000);
        break;
    }
}
