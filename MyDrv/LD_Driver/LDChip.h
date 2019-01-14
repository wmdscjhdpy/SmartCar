/************************************************************************************
**	版权所有：Copyright (c) 2005 - 2010 ICRoute INC.
** 硬件说明：STM32f103vet6  8MHZ  内部时钟为72MHZ 
** 					 LD3320 有源晶振 12MHZ  不同的晶振请修改LDChip.h 中的定义值（＃define CLK_IN  12）
** 软件说明：并行读写寄存器方式，如采用其他方式即修改Reg_RW.c 文件即可！
** 修改者:  http://shop64790118.taobao.com/
** 时间：2011.10.15
** 说明：本程序修改于 ICRoute公司 配套的源程序，版权归其所有。
************************************************************************************/

#ifndef LD_CHIP_H
#define LD_CHIP_H


//	以下三个状态定义用来记录程序是在运行ASR识别还是在运行MP3播放
#define LD_MODE_IDLE		0x00
#define LD_MODE_ASR_RUN		0x08
#define LD_MODE_MP3		 	0x40


//	以下五个状态定义用来记录程序是在运行ASR识别过程中的哪个状态
#define LD_ASR_NONE				0x00	//	表示没有在作ASR识别
#define LD_ASR_RUNING			0x01	//	表示LD3320正在作ASR识别中
#define LD_ASR_FOUNDOK			0x10	//	表示一次识别流程结束后，有一个识别结果
#define LD_ASR_FOUNDZERO 		0x11	//	表示一次识别流程结束后，没有识别结果
#define LD_ASR_ERROR	 		0x31	//	表示一次识别流程中LD3320芯片内部出现不正确的状态


#define CLK_IN   	8	/* user need modify this value according to clock in */
#define LD_PLL_11			(uint8_t)((CLK_IN/2.0)-1)
#define LD_PLL_MP3_19		0x0f
#define LD_PLL_MP3_1B		0x18
#define LD_PLL_MP3_1D   	(uint8_t)(((90.0*((LD_PLL_11)+1))/(CLK_IN))-1)

#define LD_PLL_ASR_19 		(uint8_t)(CLK_IN*32.0/(LD_PLL_11+1) - 0.51)
#define LD_PLL_ASR_1B 		0x48
#define LD_PLL_ASR_1D 		0x1f

// LD chip fixed values.
#define        RESUM_OF_MUSIC               0x01
#define        CAUSE_MP3_SONG_END           0x20

#define        MASK_INT_SYNC				0x10
#define        MASK_INT_FIFO				0x04
#define    	   MASK_AFIFO_INT				0x01
#define        MASK_FIFO_STATUS_AFULL		0x08



void LD_reset(void);
void ProcessInt0(void);//判断声音识别id
uint8_t LD_GetResult(void);//判断后获得识别的id
uint8_t RunASR(void);//相当于初始化
extern uint8_t  nLD_Mode;
extern uint8_t  nAsrStatus;


//识别码（客户修改处）
#define CODE_CMD	0	 /*唤醒指令*/
#define CODE_KD	  2	    /*闪烁*/
#define CODE_GD	3	 /*按键触发*/
#define CODE_QM	  4	    /*全灭*/

void  LD3320_delay(unsigned long uldata);

#define MIC_VOL 0x43
#endif
