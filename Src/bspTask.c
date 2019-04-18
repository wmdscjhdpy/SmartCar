#include "bsp.h"
//ID声明

//任务声明
static void Encoder_Task(void const * argument);
static void VoiceASR_Task(void const * argument);
static void Head_Task(void const * argument);
int16_t Lflag=0;
int16_t Rflag=0;
void Start_Task(void const * argument)
{
  //创建编码器任务
  LED(3,1);
  osThreadDef(EncoderTask, Encoder_Task, osPriorityNormal, 0, 256);
  osThreadCreate(osThread(EncoderTask), NULL);
  osThreadDef(HeadTask, Head_Task, osPriorityNormal, 0, 128);
  osThreadCreate(osThread(HeadTask), NULL);
  osThreadDef(VoiceASRTask, VoiceASR_Task, osPriorityNormal, 0, 256);
  osThreadCreate(osThread(VoiceASRTask), NULL);
  osDelay(1000);
  //Play_Sound("锅锅国尬摇锅锅国尬鸽国锅");
  Play_Sound("你好");
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
///记录编码器数值,整合ADC数据
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
    static uint8_t Cmdflag=0;//是否收到一级口令的标志位
    nAsrStatus=LD_ASR_RUNING;
    osDelay(2000);
    if (RunASR()==0)	//	启动一次ASR识别流程：ASR初始化，ASR添加关键词语，启动ASR运算
    {		
        nAsrStatus = LD_ASR_ERROR;
    }
    while(1)
    {
        ProcessInt0();//查询是否有识别指令
        switch(nAsrStatus)
		{
			case LD_ASR_RUNING:
			case LD_ASR_ERROR:		
			break;
			case LD_ASR_NONE:
                nAsrStatus=LD_ASR_RUNING;
                if (RunASR()==0)	//	启动一次ASR识别流程：ASR初始化，ASR添加关键词语，启动ASR运算
                {		
                    nAsrStatus = LD_ASR_ERROR;
                }
                break;
			case LD_ASR_FOUNDOK:
                nAsrRes = LD_GetResult();	//	一次ASR识别流程结束，去取ASR识别结果	
                if(nAsrRes==CODE_CMD)//接受到了口令
                {
                    LED(1,1);
                    Cmdflag=1;
                    Play_Sound("老哥想干什么？");
                }else if(Cmdflag==1)
                {
                    Cmdflag=0;
                    LED(1,0);
                    switch(nAsrRes)		   /*对结果执行相关操作,客户修改*/
                      {
                            case CODE_KD:	        /*命令“开灯”*/
                                LED(2,1);
                                Play_Sound("灯已经给您打开啦！看一下对不对？");
                            break;
                            case CODE_GD:		/*命令“关灯”*/
                                LED(2,0);
                                Play_Sound("我觉得现在灯肯定关了，你看下？");
                            break;
                            case CODE_QM:		/*命令“全灭”*/
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
//头部的管理任务
//注意 这里的头部 正转是抬头
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
    //进行此步骤校准完成后 编码器的值为-84
    HEAD_Break;
    car.HeadTargetCurrent=500;//默认值
    for(;;)
    {//处理头部目标位置的控制
        if(ABS(car.HeadTargetEnc-car.HeadRealEnc)>5)
        {
            MECH_RUN(HEAD,SIGN(car.HeadTargetEnc-car.HeadRealEnc)*car.HeadTargetCurrent);
        }else{
            HEAD_Break;
        }
        osDelay(10);
    }
}
