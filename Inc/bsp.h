#ifndef BSP_H
#define BSP_H

#include "stm32f4xx_hal.h"
#include "uart_esp.h"
#include "UART_MV.h"
#include "tim.h"
#include "adc.h"
#include "spi.h"
#include "cmsis_os.h"
#include "voice.h"
#include "LDchip.h"
#include "Reg_RW.h"
#include "iic_oled.h"
#define ADC_BUF_NUM 30

extern float Battery_V;
extern uint16_t Distance0;//����Թ�1�ĵ�ѹֵ
extern uint16_t Distance1;//����Թ�2�ĵ�ѹֵ

void BSP_Init(void);
void ADC_Handler(void);
void Car_Move(float forward,float sita);
//ͨ�ú궨��
#define ABS(x) ((x)>0?(x):-(x))
#define SIGN(x) ((x)/ABS(x))
#define LIMIT(x,l) (ABS(x)>(l)?SIGN(x)*(l):(x))
#define LED(num,value) HAL_GPIO_WritePin(LED##num##_GPIO_Port,LED##num##_Pin,(GPIO_PinState)value);
//�����˶������궨�壬�ٶȶ���Ϊ0-1000
#define RIGHT_FORWARD(x) __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,x);__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,0)
#define RIGHT_REVERSE(x) __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,x);__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,0)
#define RIGHT_Break __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,1000);__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,1000)

#define LEFT_FORWARD(x) __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,x);__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,0)
#define LEFT_REVERSE(x) __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,x);__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,0)
#define LEFT_Break __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,1000);__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,1000)

//ͷ������ ǰ���������������
#define HEAD_FORWARD(x) __HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_3,0);__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_4,x)
#define HEAD_REVERSE(x) __HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_4,0);__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_3,x)
#define HEAD_Break __HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_3,1000);__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_4,1000)

//�ۺ������ŵĵ������
#define MECH_RUN(name,value) {\
    if(value>0){name##_FORWARD(LIMIT(value,1000));}else if(value<0){name##_REVERSE(LIMIT(value,1000));}else{ name##_Break;}\
}
//�ֲ����� ����ֵӦΪ500-2500 ��ǰȷ���ķ�ΧΪ750-1300
#define HAND_SET(x) __HAL_TIM_SetCompare(&htim5,TIM_CHANNEL_3,x)
#define HAND_UP HAND_SET(1300)
#define HAND_DOWN HAND_SET(750)
typedef struct {
    int HeadTargetEnc;
    int HeadRealEnc;
    int HeadTargetCurrent;
    int action_priority;//�¼����ȼ������ȼ��ߵĻ���ռ���ȼ��͵�ʱ�� �������߳����ȼ��������
}car_t;
extern car_t car;
#endif
