#ifndef UART_Debug
#define UART_Debug

#define HUART huart3 //�������޸ĵ����õĴ���
#include "stm32f4xx_hal.h" //���32�ͺŲ�һ��Ҫ�޸�����
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
//��Ϊ�����õ�ָ�� ͳһʹ��DMA���� �Ӵ��ڿ����жϵķ�ʽ������ �ú�����Ҫ������������һ��
void Debug_Init(void);

//�ú�����Ҫ���ڴ����жϷ���������
void Debug_IT(void);
static void MV_Enable(void);
#endif
