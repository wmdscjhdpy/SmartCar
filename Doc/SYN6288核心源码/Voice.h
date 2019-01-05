#ifndef __VOICE_H
#define __VOICE_H
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "USART_Config.h"   //USART����
#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#define PLAY PBin(1)/*���нӿ�����*/
void SYN_FrameInfo(uint8_t Music,uint8_t *HZdata);
void YS_SYN_Set(uint8_t *Info_data);
void PrintCom(uint8_t *DAT,uint8_t len);
void USART_Send_Byte(uint16_t mydata);/*�Ĵ���ֻҪ������ӿھ�����*/
void voice_init(void);   /*��ʼ��*/
void Play_Sound(uint8_t *HZdata);  /*��������*/

#endif
