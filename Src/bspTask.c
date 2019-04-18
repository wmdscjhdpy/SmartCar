#include "bsp.h"
//ID����

//��������
static void Encoder_Task(void const * argument);
static void VoiceASR_Task(void const * argument);
static void Head_Task(void const * argument);
int16_t Lflag=0;
int16_t Rflag=0;
void Start_Task(void const * argument)
{
  //��������������
  LED(3,1);
  osThreadDef(EncoderTask, Encoder_Task, osPriorityNormal, 0, 256);
  osThreadCreate(osThread(EncoderTask), NULL);
  osThreadDef(HeadTask, Head_Task, osPriorityNormal, 0, 128);
  osThreadCreate(osThread(HeadTask), NULL);
  osThreadDef(VoiceASRTask, VoiceASR_Task, osPriorityNormal, 0, 256);
  osThreadCreate(osThread(VoiceASRTask), NULL);
  osDelay(1000);
  //Play_Sound("��������ҡ�������θ����");
  Play_Sound("���");
  HAND_UP;
  osDelay(500);
  HAND_DOWN;
  osDelay(500);
  HAND_UP;
  osDelay(500);
  HAND_DOWN;
  osDelay(500);
  LED(3,0);
  while(1)
  {
      //LEFT_FORWARD(1000);
      //RIGHT_FORWARD(1000);
      //continue;
     osDelay(100);
      if(Lflag>0)
      {
          //HEAD_FORWARD(Lflag);
          LEFT_FORWARD(Lflag);
      }else if(Lflag<0){
          //HEAD_REVERSE(-Lflag);
          LEFT_REVERSE(-Lflag);
      }else{
          //HEAD_Break;
          LEFT_Break;
      }
      if(Rflag>0)
      {
          RIGHT_FORWARD(Rflag);
      }else if(Rflag<0){
          RIGHT_REVERSE(-Rflag);
      }else{
          RIGHT_Break;
      }
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
       car.HeadRealEnc+=enc_dx;
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
//ͷ���Ĺ�������
//ע�� �����ͷ�� ��ת��̧ͷ
void Head_Task(void const * argument)
{
    int tmp_enc=car.HeadRealEnc;
    HEAD_REVERSE(500);
    for(;;)
    {
        osDelay(200);
        if(ABS(tmp_enc-car.HeadRealEnc)<3)
            break;
        tmp_enc=car.HeadRealEnc;
    }
    car.HeadRealEnc=-84;
    //���д˲���У׼��ɺ� ��������ֵΪ-84
    HEAD_Break;
    car.HeadTargetCurrent=500;//Ĭ��ֵ
    for(;;)
    {//����ͷ��Ŀ��λ�õĿ���
        if(ABS(car.HeadTargetEnc-car.HeadRealEnc)>5)
        {
            MECH_RUN(HEAD,SIGN(car.HeadTargetEnc-car.HeadRealEnc)*car.HeadTargetCurrent);
        }else{
            HEAD_Break;
        }
        osDelay(10);
    }
}
