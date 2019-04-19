#include "UART_MV.h"
#include "usart.h" //���ļ����������ʱ�� ˵��cubeû�а����赥������c�ļ� ֻ��Ҫ��������һ��ͷ�ļ��Ϳ�����
#include "string.h"
#include "bsp.h"

#define BUF_MAX 30 //�趨������߻�����
uint8_t rebuf[BUF_MAX]={0};
uint8_t sebuf[BUF_MAX]={0};
static void Debug_Handle(void);
static Rec_flame_t* Rec;//���ڽ�������
//��Ϊ�����õ�ָ�� ͳһʹ��DMA���� �Ӵ��ڿ����жϵķ�ʽ������ �ú�����Ҫ������������һ��
void Debug_Init(void)
{
	HAL_UART_Receive_DMA(&HUART,rebuf,BUF_MAX);
	__HAL_UART_ENABLE_IT(&HUART,UART_IT_IDLE);
}

//�ú�����Ҫ���ڴ����жϷ���������
void Debug_IT()
{
	if(__HAL_UART_GET_FLAG(&HUART,UART_FLAG_IDLE))
	{
		HAL_UART_DMAStop(&HUART);
			Debug_Handle();//�������ݴ�����
		memset(rebuf,0,BUF_MAX);
		__HAL_UART_CLEAR_IDLEFLAG(&HUART);
		HAL_UART_Receive_DMA(&HUART,rebuf,BUF_MAX);
	}
}

static void Debug_Handle()
{
	Rec=(Rec_flame_t*)rebuf;//����ָ��ָ������ͷ
    if(Rec->Head==0x4f)//У������֡
    {
        uint8_t parity=0x00;//У���ֽ�
        for(uint8_t i=0;i<sizeof(Rec_flame_t)-1;i++)//��֡�ڵ�ǰ�������ֽڽ����ۼ����
        {
            parity+=rebuf[i];
        }
        if(parity!=Rec->End)return;//У�鲻���ϣ��Ǵ�������֡������
        //���������ݴ����������
        switch(Rec->State)
        {
            case MvStateCarCtl:
                Car_Move(Rec->XData,Rec->YData);
            break;
        }
    }
}

//��MV��ʹ��λ
void MV_Enable(void)
{
    Rec_flame_t* send=(Rec_flame_t*)sebuf;
    send->Head=0x4f;
    send->State=0x01;
    send->adden1=0x01;//ָ��Ϊ����׷��ָ��
    uint8_t parity=0x00;//У���ֽ�
    for(uint8_t i=0;i<sizeof(Rec_flame_t)-1;i++)//��֡�ڵ�ǰ�������ֽڽ����ۼ����
    {
        parity+=sebuf[i];
    }
    send->End=parity;
    HAL_UART_Transmit(&HUART,sebuf,sizeof(Rec_flame_t),0xff);
}
