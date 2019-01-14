#include "bsp.h"
#include "cmsis_os.h"
#include "voice.h"
#include "LDchip.h"
#include "Reg_RW.h"
//ID声明

//任务声明
static void Encoder_Task(void const * argument);
static void VoiceASR_Task(void const * argument);
void Start_Task(void const * argument)
{
  //创建编码器任务
  osThreadDef(EncoderTask, Encoder_Task, osPriorityNormal, 0, 256);
  osThreadCreate(osThread(EncoderTask), NULL);
  osThreadDef(VoiceASRTask, VoiceASR_Task, osPriorityNormal, 0, 256);
  osThreadCreate(osThread(VoiceASRTask), NULL);
  osDelay(1000);
  Play_Sound("主人你好，我是疯狗，很高兴与你见面！");
  while(1)
  {
     osDelay(100);
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
       Enc_Head+=enc_dx;
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
                }else if(Cmdflag==1)
                {
                    Cmdflag=0;
                    switch(nAsrRes)		   /*对结果执行相关操作,客户修改*/
                      {
                            case CODE_KD:	        /*命令“开灯”*/
                                LED(2,1);
                            break;
                            case CODE_GD:		/*命令“关灯”*/
                                LED(2,0);
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
