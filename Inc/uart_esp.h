#ifndef UART_Debug
#define UART_Debug

#define ESPUART huart1 //�������޸ĵ����õĴ���
#include "stm32f4xx_hal.h" //���32�ͺŲ�һ��Ҫ�޸�����

//��Ϊ�����õ�ָ�� ͳһʹ��DMA���� �Ӵ��ڿ����жϵķ�ʽ������ �ú�����Ҫ������������һ��
void ESP_Init(void);

//�ú�����Ҫ���ڴ����жϷ���������
void ESP_IT(void);

#endif
