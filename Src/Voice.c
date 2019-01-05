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
         Frame_Info[4] = 0x00 | Music<<4 ; //00��GB2132 01:GBK
	 
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

void voice_init(void)   /*��ʼ��*/
{
    
}
void Play_Sound(uint8_t *HZdata)  /*��������*/
{
	if(!PLAYING)
	{					
            SYN_FrameInfo(0,HZdata) ;  /*����*/
            SYN_FrameInfo(0,HZdata) ;  /*����*/
			//printf("������\r\n");
	}	
	else
	{
		//printf("��������æ������\r\n");
		HAL_Delay(1000);
	}
}
