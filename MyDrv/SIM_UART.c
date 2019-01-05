/** 
* @file	  SIM_UART.c
* @brief      �Ͳ�����ģ�⴮������ 
* @details  ��Ҫһ����ʱ����Ϊ����ʱ����������ǰѡ�õ���84Mhzʱ����875��Ƶ������CNT=10ʱΪһ��9600�����ʵĴ���ʱ��
* @author   WMD
* @date     2019��1��5��15:01:07 
* @version  0.1
* @par Copyright (c):  
*       WMD 
* @par ��־
*/  
#include "stm32f4xx_hal.h"
#include "tim.h"
#define OI_TXD(x)	HAL_GPIO_WritePin(SIM_UART_TX_GPIO_Port,SIM_UART_TX_Pin,x)


uint8_t len = 0;	//���ռ���
uint8_t USART_buf[11];  //���ջ�����

enum{
	COM_START_BIT,
	COM_D0_BIT,
	COM_D1_BIT,
	COM_D2_BIT,
	COM_D3_BIT,
	COM_D4_BIT,
	COM_D5_BIT,
	COM_D6_BIT,
	COM_D7_BIT,
	COM_STOP_BIT,
};

uint8_t recvStat = COM_STOP_BIT;
uint8_t recvData = 0;
void wait_bit(void)
{
    TIM10->CNT=0;
    HAL_TIM_Base_Start(&htim10);
    while(TIM10->CNT<10);//�ȴ�9�μ���
    HAL_TIM_Base_Stop(&htim10);
}
void IO_TXD(uint8_t Data)
{
    
	uint8_t i = 0;
	OI_TXD(0);  
	wait_bit();
	for(i = 0; i < 8; i++)
	{
		if(Data&0x01)
			OI_TXD(1);  
		else
			OI_TXD(0); 	
		
		wait_bit();
		Data = Data>>1;
	}
	OI_TXD(1);
	wait_bit();
}
	
void SIM_UART_Send(uint8_t *buf, uint8_t len)
{
	uint8_t t;
	for(t = 0; t < len; t++)
	{
		IO_TXD(buf[t]);
	}
}

