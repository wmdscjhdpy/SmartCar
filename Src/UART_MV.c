#include "UART_MV.h"
#include "usart.h" //该文件出现问题的时候 说明cube没有把外设单独生成c文件 只需要引用另外一个头文件就可以了
#include "string.h"
#include "bsp.h"

#define BUF_MAX 30 //设定接收最高缓存区
uint8_t rebuf[BUF_MAX]={0};
uint8_t sebuf[BUF_MAX]={0};
static void Debug_Handle(void);
static Rec_flame_t* Rec;//用于接收数据
//作为调试用的指令 统一使用DMA接收 加串口空闲中断的方式来调用 该函数需要在主函数调用一次
void Debug_Init(void)
{
	HAL_UART_Receive_DMA(&HUART,rebuf,BUF_MAX);
	__HAL_UART_ENABLE_IT(&HUART,UART_IT_IDLE);
}

//该函数需要放在串口中断服务函数里面
void Debug_IT()
{
	if(__HAL_UART_GET_FLAG(&HUART,UART_FLAG_IDLE))
	{
		HAL_UART_DMAStop(&HUART);
			Debug_Handle();//调用数据处理函数
		memset(rebuf,0,BUF_MAX);
		__HAL_UART_CLEAR_IDLEFLAG(&HUART);
		HAL_UART_Receive_DMA(&HUART,rebuf,BUF_MAX);
	}
}

static void Debug_Handle()
{
	Rec=(Rec_flame_t*)rebuf;//接收指针指向数组头
    if(Rec->Head==0x4f)//校验数据帧
    {
        uint8_t parity=0x00;//校验字节
        for(uint8_t i=0;i<sizeof(Rec_flame_t)-1;i++)//对帧内的前面所有字节进行累加求和
        {
            parity+=rebuf[i];
        }
        if(parity!=Rec->End)return;//校验不符合，是错误数据帧，丢弃
        //真正的数据处理段在下面
        switch(Rec->State)
        {
            case MvStateCarCtl:
                Car_Move(Rec->XData,Rec->YData);
            break;
        }
    }
}

//给MV发使能位
void MV_Enable(void)
{
    Rec_flame_t* send=(Rec_flame_t*)sebuf;
    send->Head=0x4f;
    send->State=0x01;
    send->adden1=0x01;//指定为物体追踪指令
    uint8_t parity=0x00;//校验字节
    for(uint8_t i=0;i<sizeof(Rec_flame_t)-1;i++)//对帧内的前面所有字节进行累加求和
    {
        parity+=sebuf[i];
    }
    send->End=parity;
    HAL_UART_Transmit(&HUART,sebuf,sizeof(Rec_flame_t),0xff);
}
