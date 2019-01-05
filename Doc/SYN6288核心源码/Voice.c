#include "Voice.h"
void SYN_FrameInfo(uint8_t Music,uint8_t *HZdata)
{
		 unsigned  char  Frame_Info[50];
         unsigned  char  HZ_Length;  
		 unsigned  char  ecc  = 0;  		
	     unsigned  int i=0; 
		 HZ_Length =strlen((char*)HZdata); 			
         
		 Frame_Info[0] = 0xFD ; 		
		 Frame_Info[1] = 0x00 ; 			
		 Frame_Info[2] = HZ_Length + 3; 		
		 Frame_Info[3] = 0x01 ; 				 		 
		 Frame_Info[4] = 0x01 | Music<<4 ; 
	 
		 for(i = 0; i<5; i++)   				
	     {  
	         ecc=ecc^(Frame_Info[i]);		
	     }

	   	 for(i= 0; i<HZ_Length; i++)   	
	     {  
	         ecc=ecc^(HZdata[i]); 				
	     }		 
	  
		  memcpy(&Frame_Info[5], HZdata, HZ_Length);
		  Frame_Info[5+HZ_Length]=ecc;
		  PrintCom(Frame_Info,5+HZ_Length+1);
}

void YS_SYN_Set(uint8_t *Info_data)
{
	uint8_t Com_Len;
	Com_Len =strlen((char*)Info_data);
	PrintCom(Info_data,Com_Len);
}

void PrintCom(uint8_t *DAT,uint8_t len)
{
	uint8_t i;
	for(i=0;i<len;i++)
	{
	 	USART_Send_Byte(*DAT++);
	}	
}
void USART_Send_Byte(uint16_t mydata)
{
		USART_SendData(USART3,mydata);
    while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
}
void voice_init(void)   /*��ʼ��*/
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��PA,PB,PC�˿�ʱ��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 													//IO���ٶ�Ϊ50MHz	
	//PB�˿ڳ�ʼ��	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;						//����PC10~PC12�˿��������
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU; 		 												//�������	
	GPIO_Init(GPIOB, &GPIO_InitStructure);		
}
void Play_Sound(uint8_t *HZdata)  /*��������*/
{
	if(!PLAY)
	{					
			SYN_FrameInfo(0,HZdata) ;  /*����*/
		  SYN_FrameInfo(0,HZdata) ;  /*����*/
			printf("������\r\n");
	}	
	else
	{
		printf("��������æ������\r\n");
		delay_ms(1000);
	}
}
