#ifndef BSP_H
#define BSP_H

#include "stm32f4xx_hal.h"
#include "tim.h"
#include "adc.h"
#define ADC_BUF_NUM 30

extern int32_t Enc_Head;//头部编码器值
extern float Battery_V;
extern uint16_t Distance0;//红外对管1的电压值
extern uint16_t Distance1;//红外对管2的电压值
void BSP_Init(void);
void ADC_Handler(void);
#define LED(num,value) HAL_GPIO_WritePin(LED##num##_GPIO_Port,LED##num##_Pin,(GPIO_PinState)value);
//车辆运动驱动宏定义，速度定义为0-1000
#define LEFT_FORWARD(x) __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,x);__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,0)
#define LEFT_REVERSE(x) __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,x);__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,0)
#define LEFT_Break __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,1000);__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,1000)

#define RIGHT_FORWARD(x) __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,x);__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,0)
#define RIGHT_REVERSE(x) __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,x);__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,0)
#define RIGHT_Break __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,1000);__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,1000)

#define HEAD_FORWARD(x) __HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_3,x);__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_4,0)
#define HEAD_REVERSE(x) __HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_4,x);__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_3,0)
#endif
