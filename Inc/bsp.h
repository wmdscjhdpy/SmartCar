#ifndef BSP_H
#define BSP_H

#include "stm32f4xx_hal.h"
#include "tim.h"

extern int32_t Enc_Head;//ͷ��������ֵ

void BSP_Init(void);

//�����˶������궨�壬�ٶȶ���Ϊ0-1000
#define LEFT_FORWARD(x) __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,x);__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,0)
#define LEFT_REVERSE(x) __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,x);__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,0)
#define LEFT_Break __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,1000);__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,1000)

#define RIGHT_FORWARD(x) __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,x);__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,0)
#define RIGHT_REVERSE(x) __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,x);__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,0)
#define RIGHT_Break __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,1000);__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,1000)

#define HEAD_FORWARD(x) __HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_3,x);__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_4,0)
#define HEAD_REVERSE(x) __HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_4,x);__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_3,0)
#endif
