#include "uart_esp.h"
#include "bsp.h"
#include "usart.h" //���ļ����������ʱ�� ˵��cubeû�а����赥������c�ļ� ֻ��Ҫ��������һ��ͷ�ļ��Ϳ�����
#include "string.h"

#define BUF_MAX 30 //�趨������߻�����
uint8_t esprebuf[BUF_MAX]={0};
static void ESP_Handle(void);
//��Ϊ�����õ�ָ�� ͳһʹ��DMA���� �Ӵ��ڿ����жϵķ�ʽ������ �ú�����Ҫ������������һ��
void ESP_Init(void)
{
	HAL_UART_Receive_DMA(&ESPUART,esprebuf,BUF_MAX);
	__HAL_UART_ENABLE_IT(&ESPUART,UART_IT_IDLE);
    HAL_UART_Transmit(&ESPUART,(uint8_t*)"Hi,I am online\n",16,1000);
}

//�ú�����Ҫ���ڴ����жϷ���������
void ESP_IT()
{
	if(__HAL_UART_GET_FLAG(&ESPUART,UART_FLAG_IDLE))
	{
		HAL_UART_DMAStop(&ESPUART);
			ESP_Handle();//�������ݴ�����
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
            Car_Move(esprebuf[1]*20.0f,esprebuf[2]*20.0f);
        break;
        case 0x04:
            Car_Move(1000,0);
        break;
        case 0x05:
            Car_Move(-1000,0);
        break;
        case 0x06:
            Car_Move(0,-1000);
        break;
        case 0x07:
            Car_Move(0,1000);
        break;
    }
}
