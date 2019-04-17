/***************************乐声电子科技有限公司****************************
**  工程名称：YS-V0.4语音识别开发板程序
**	CPU: STM32f103RCT6
**	晶振：8MHZ
**	波特率：9600 bit/S
**	配套产品信息：YS-V0.4语音识别开发板
**                http://yuesheng001.taobao.com
**  作者：zdings
**  联系：751956552@qq.com
**  修改日期：2012.4.11
**  说明：本程序 具备语音识别、串口通信、开发板基本功能演示。
***************************乐声电子科技有限公司******************************/
#include "bsp.h"
#include "LDchip.h"
#include "Reg_RW.h"

/************************************************************************************
//	nAsrStatus 用来在main主程序中表示程序运行的状态，不是LD3320芯片内部的状态寄存器
//	LD_ASR_NONE:		表示没有在作ASR识别
//	LD_ASR_RUNING：		表示LD3320正在作ASR识别中
//	LD_ASR_FOUNDOK:		表示一次识别流程结束后，有一个识别结果
//	LD_ASR_FOUNDZERO:	表示一次识别流程结束后，没有识别结果
//	LD_ASR_ERROR:		表示一次识别流程中LD3320芯片内部出现不正确的状态
*********************************************************************************/


void LD3320_Init(void);

uint8 RunASR(void);


///***********************************************************
//* 名    称： LD3320_main(void)
//* 功    能： 主函数LD3320程序入口
//* 入口参数：  
//* 出口参数：
//* 说    明：
//* 调用方法： 
//**********************************************************/ 

//void  LD3320_main(void)
//{
//	uint8 nAsrRes=0;
//	LD3320_Init();	  
//    //

//	nAsrStatus = LD_ASR_NONE;		//	初始状态：没有在作ASR

//	while(1)
//	{
// 	switch(nAsrStatus)
//		{
//			case LD_ASR_RUNING:
//			case LD_ASR_ERROR:		
//												break;
//			case LD_ASR_NONE:
//											nAsrStatus=LD_ASR_RUNING;
//											if (RunASR()==0)	//	启动一次ASR识别流程：ASR初始化，ASR添加关键词语，启动ASR运算
//											{		
//												nAsrStatus = LD_ASR_ERROR;
//											}
//											break;

//			case LD_ASR_FOUNDOK:
//											 nAsrRes = LD_GetResult( );	//	一次ASR识别流程结束，去取ASR识别结果				
//														
//												 switch(nAsrRes)		   /*对结果执行相关操作,客户修改*/
//												  {
//													  case CODE_LSD:			/*命令“流水灯”*/
//																						 break;
//														case CODE_KD:	        /*命令“闪烁”*/
//																						 break;
//														case CODE_GD:		/*命令“按键触发”*/
//												
//																						break;
//														case CODE_QM:		/*命令“全灭”*/
//												
//																						break;
//														default:break;
//													}	
//												nAsrStatus = LD_ASR_NONE;
//											break;
//			
//			case LD_ASR_FOUNDZERO:
//			default:
//								nAsrStatus = LD_ASR_NONE;
//								break;
//			}//switch	
//	}// while

//}
/***********************************************************
* 名    称：LD3320_Init(void)
* 功    能：模块驱动端口初始配置
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void LD3320_Init(void)
{
	LD_reset();
}

/***********************************************************
* 名    称： void Delay_( int i)
* 功    能： 短延时
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void Delay_( int i)
 {     
    while( i--)
       {	

        }
 }	
/***********************************************************
* 名    称：	LD3320_delay(unsigned long uldata)
* 功    能：	长延时函数
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
 void  LD3320_delay(unsigned long uldata)
{
	unsigned int j  =  0;
	unsigned int g  =  0;
	for (j=0;j<1;j++)
	{
		for (g=0;g<uldata;g++)
		{
			Delay_(120);
		}
	}
}

