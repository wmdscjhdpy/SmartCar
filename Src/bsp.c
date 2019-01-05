#include "bsp.h"

int32_t Enc_Head=0;//头部编码器值

//初始化所有车载模块
void BSP_Init()
{
    //底盘PWM
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_ALL);
    //抬头编码器
    HAL_TIM_Encoder_Start(&htim1,TIM_CHANNEL_1|TIM_CHANNEL_2);
    //抬头PWM
    HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_3|TIM_CHANNEL_4);
    ///@TODO:两个舵机PWM
    HAL_TIM_Base_Start(&htim10);//开启模拟串口的定时器
    
}
