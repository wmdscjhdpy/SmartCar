#ifndef __VOICE_H
#define __VOICE_H
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "stm32f4xx_hal.h"
#include "usart.h"
#define VOICE_UART huart4
#define PrintCom(x,y) SIM_UART_Send(x,y)
#define PLAYING HAL_GPIO_ReadPin(BUSY_GPIO_Port,(GPIO_PinState)BUSY_Pin)
void SIM_UART_Send(uint8_t *buf, uint16_t len);
void SYN_FrameInfo(uint8_t Music,uint8_t *HZdata);
void YS_SYN_Set(uint8_t *Info_data);
void voice_init(void);   /*≥ı ºªØ*/
void Play_Sound(uint8_t *HZdata);  /*≤•∑≈…˘“Ù*/

#endif
