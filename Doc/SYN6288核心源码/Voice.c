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
    while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
}
void voice_init(void)   /*初始化*/
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能PA,PB,PC端口时钟
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 													//IO口速度为50MHz	
	//PB端口初始化	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;						//设置PC10~PC12端口推挽输出
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU; 		 												//推挽输出	
	GPIO_Init(GPIOB, &GPIO_InitStructure);		
}
void Play_Sound(uint8_t *HZdata)  /*播放声音*/
{
	if(!PLAY)
	{					
			SYN_FrameInfo(0,HZdata) ;  /*播放*/
		  SYN_FrameInfo(0,HZdata) ;  /*播放*/
			printf("播放中\r\n");
	}	
	else
	{
		printf("播放器繁忙，请检查\r\n");
		delay_ms(1000);
	}
}
