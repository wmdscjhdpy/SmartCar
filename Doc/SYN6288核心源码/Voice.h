#ifndef __VOICE_H
#define __VOICE_H
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "USART_Config.h"   //USART设置
#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#define PLAY PBin(1)/*空闲接口设置*/
void SYN_FrameInfo(uint8_t Music,uint8_t *HZdata);
void YS_SYN_Set(uint8_t *Info_data);
void PrintCom(uint8_t *DAT,uint8_t len);
void USART_Send_Byte(uint16_t mydata);/*改串口只要改这个接口就行了*/
void voice_init(void);   /*初始化*/
void Play_Sound(uint8_t *HZdata);  /*播放声音*/

#endif
