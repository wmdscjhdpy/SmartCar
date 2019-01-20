#include "bsp.h"
#include "cmsis_os.h"
#include "voice.h"
#include "LDchip.h"
#include "Reg_RW.h"
//ID����

//��������
static void Encoder_Task(void const * argument);
static void VoiceASR_Task(void const * argument);
void Start_Task(void const * argument)
{
  //��������������
  LED(3,1);
  osThreadDef(EncoderTask, Encoder_Task, osPriorityNormal, 0, 256);
  osThreadCreate(osThread(EncoderTask), NULL);
  osThreadDef(VoiceASRTask, VoiceASR_Task, osPriorityNormal, 0, 256);
  osThreadCreate(osThread(VoiceASRTask), NULL);
  osDelay(1000);
  Play_Sound("��������ҡ�������θ����");
  LED(3,0);
  while(1)
  {
     osDelay(100);
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
static void VoiceASR_Task(void const * argument)
{
    static uint8_t nAsrRes=0;
    static uint8_t Cmdflag=0;//�Ƿ��յ�һ������ı�־λ
    nAsrStatus=LD_ASR_RUNING;
    osDelay(2000);
    if (RunASR()==0)	//	����һ��ASRʶ�����̣�ASR��ʼ����ASR��ӹؼ��������ASR����
    {		
        nAsrStatus = LD_ASR_ERROR;
    }
    while(1)
    {
        ProcessInt0();//��ѯ�Ƿ���ʶ��ָ��
        switch(nAsrStatus)
		{
			case LD_ASR_RUNING:
			case LD_ASR_ERROR:		
			break;
			case LD_ASR_NONE:
                nAsrStatus=LD_ASR_RUNING;
                if (RunASR()==0)	//	����һ��ASRʶ�����̣�ASR��ʼ����ASR��ӹؼ��������ASR����
                {		
                    nAsrStatus = LD_ASR_ERROR;
                }
                break;
			case LD_ASR_FOUNDOK:
                nAsrRes = LD_GetResult();	//	һ��ASRʶ�����̽�����ȥȡASRʶ����	
                if(nAsrRes==CODE_CMD)//���ܵ��˿���
                {
                    LED(1,1);
                    Cmdflag=1;
                    Play_Sound("�ϸ����ʲô��");
                }else if(Cmdflag==1)
                {
                    Cmdflag=0;
                    LED(1,0);
                    switch(nAsrRes)		   /*�Խ��ִ����ز���,�ͻ��޸�*/
                      {
                            case CODE_KD:	        /*������ơ�*/
                                LED(2,1);
                                Play_Sound("���Ѿ�������������һ�¶Բ��ԣ�");
                            break;
                            case CODE_GD:		/*����صơ�*/
                                LED(2,0);
                                Play_Sound("�Ҿ������ڵƿ϶����ˣ��㿴�£�");
                            break;
                            case CODE_QM:		/*���ȫ��*/
                            break;
                            default:
                            break;
                      }
                }
                nAsrStatus = LD_ASR_NONE;
            break;
			
			case LD_ASR_FOUNDZERO:
			default:
                nAsrStatus = LD_ASR_NONE;
            break;
        }//switch	
        osDelay(100);
    }
}
