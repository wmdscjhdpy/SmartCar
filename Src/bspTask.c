#include "bsp.h"
#include "cmsis_os.h"
#include "voice.h"
//ID����

//��������
void Encoder_Task(void const * argument);

void Start_Task(void const * argument)
{
  //��������������
  osThreadDef(EncoderTask, Encoder_Task, osPriorityNormal, 0, 256);
  osThreadCreate(osThread(EncoderTask), NULL);
  while(1)
  {
     osDelay(100);
    Play_Sound("С��");
    //SIM_UART_Send("hello",6);
  }
}
///��¼��������ֵ,����ADC����
static void Encoder_Task(void const * argument)
{
   int16_t enc_dx;
   TIM1->CNT=0x7fff;
   while(1)
   {
       enc_dx=TIM1->CNT-0x7fff;
       Enc_Head+=enc_dx;
       TIM1->CNT=0x7fff;
       ADC_Handler();
       osDelay(10);
   }
}
