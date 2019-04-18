#include "bsp.h"

uint32_t ADC_Buffer[ADC_BUF_NUM];
float Battery_V;
uint16_t Distance0;//红外对管1的电压值
uint16_t Distance1;//红外对管2的电压值
car_t car;
//初始化所有车载模块
void BSP_Init()
{
    
    //底盘PWM
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
    //抬头编码器
    HAL_TIM_Encoder_Start(&htim1,TIM_CHANNEL_1|TIM_CHANNEL_2);
    //抬头PWM
    HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_4);
    HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_3);
    //两个舵机PWM
    HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_4);
    
    HAL_TIM_Base_Start(&htim10);//开启模拟串口的定时器
    //电池电压，红外对管电压采集ADC
    HAL_ADC_Start_DMA(&hadc1,ADC_Buffer,ADC_BUF_NUM);
    //ESP8266串口
    ESP_Init();
    //屏幕初始化
    OLED_Init();
    OLED_ON();
    OLED_CLS();
    OLED_OPEN_SCREEN();
    OLED_ON();
}
//处理ADC数据
void ADC_Handler(void)
{
    float tmp_a=0,tmp_b=0,tmp_c=0;
    for(uint16_t i=0;i<ADC_BUF_NUM/3;i++)
    {
        tmp_a+=ADC_Buffer[i*3];
        tmp_b+=ADC_Buffer[i*3+1];
        tmp_c+=ADC_Buffer[i*3+2];
    }
    Battery_V=((tmp_a*3/ADC_BUF_NUM)/4096)*3.3f*2;//最终电池电压
    Distance0=tmp_b*3/ADC_BUF_NUM;
    Distance1=tmp_c*3/ADC_BUF_NUM;
}
//车辆的移动函数 因为车子只能前进和旋转 因此只有两个变量
//定义sita为正时顺时针旋转
void Car_Move(float forward,float sita)
{
    float Left=0,Right=0;
    float limit_tmp;
    //运动解算
    Left=sita+forward;
    Right=-sita+forward;
    //进行限幅到电机能够的最大输出
    if(ABS(Left)>ABS(Right) && ABS(Left)>1000)
    {
        limit_tmp=1000.0f/ABS(Left);
        Left*=limit_tmp;
        Right*=limit_tmp;
    }else if(ABS(Right)>ABS(Left) && ABS(Right)>1000)
    {
        limit_tmp=1000.0f/ABS(Right);
        Left*=limit_tmp;
        Right*=limit_tmp;
    }
    MECH_RUN(LEFT,(int16_t)Left);
    MECH_RUN(RIGHT,(int16_t)Right);
}
