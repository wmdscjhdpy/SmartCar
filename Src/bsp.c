#include "bsp.h"

int32_t Enc_Head=0;//ͷ��������ֵ

//��ʼ�����г���ģ��
void BSP_Init()
{
    //����PWM
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_ALL);
    //̧ͷ������
    HAL_TIM_Encoder_Start(&htim1,TIM_CHANNEL_1|TIM_CHANNEL_2);
    //̧ͷPWM
    HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_3|TIM_CHANNEL_4);
    ///@TODO:�������PWM
    HAL_TIM_Base_Start(&htim10);//����ģ�⴮�ڵĶ�ʱ��
    
}
