#include "bsp.h"

int32_t Enc_Head=0;//ͷ��������ֵ
uint32_t ADC_Buffer[ADC_BUF_NUM];
float Battery_V;
uint16_t Distance0;//����Թ�1�ĵ�ѹֵ
uint16_t Distance1;//����Թ�2�ĵ�ѹֵ
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
    //��ص�ѹ������Թܵ�ѹ�ɼ�ADC
    HAL_ADC_Start_DMA(&hadc1,ADC_Buffer,ADC_BUF_NUM);
}
//����ADC����
void ADC_Handler(void)
{
    float tmp_a=0,tmp_b=0,tmp_c=0;
    for(uint16_t i=0;i<ADC_BUF_NUM/3;i++)
    {
        tmp_a+=ADC_Buffer[i*3];
        tmp_b+=ADC_Buffer[i*3+1];
        tmp_c+=ADC_Buffer[i*3+2];
    }
    Battery_V=((tmp_a*3/ADC_BUF_NUM)/4096)*3.3f*2;//���յ�ص�ѹ
    Distance0=tmp_b*3/ADC_BUF_NUM;
    Distance1=tmp_c*3/ADC_BUF_NUM;
}
