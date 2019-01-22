/*
*********************************************************************************************************
									WMD模块化散装库————————OLED库（HAL限定）
							初始化：cube需要两个高速 开漏 输出io 根据需要更改宏定义 
		使用：可以直接写屏幕 也可以开启printf写 根据刷新形式选择的不同来选择 保证pointled周期执行
							PS 可以自己加入开机的图像数据 然后可以调用OPEN_SCREEN函数
												2017.11.14 V4.0
*********************************************************************************************************
*/
#include "iic_oled.h"
#include "codetab.h"	  //标准英文字库
#include <string.h>
//0x00 命令位
//0x40 存储位
//普通io模拟iic的oled驱动程序
//creae by WMD
//更新时间 17.11.4

#define OLED_PAGE_SHIFT (0)//发现y坐标会整天偏移 可以由程序决定人工纠偏量
#define OLED_AS_SCREEN //使用类屏幕型输出方式
//#define OLED_PUTC //如果不需要用到oled重定向到printf 就把这句注释掉
uint8_t IIC_OLED_Send_normal(uint8_t addr, uint8_t reg, uint8_t len, uint8_t data);
uint8_t IIC_OLED_SendBuffer(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data);//模拟IIC启用


uint8_t OLED_Init(void)
{
    HAL_Delay(100); //这里的延时很重要
		uint8_t startcmd[28]={
    0xAE, //display off
    0x20, //Set Memory Addressing Mode    
    0x10, //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET,11,Invalid
    0xb0, //Set Page Start Address for Page Addressing Mode,0-7
    0xc8, //Set COM Output Scan Direction
    0x00, //---set low column address
    0x10, //---set high column address
    0x40, //--set start line address
    0x81, //--set contrast control register
    0xff, //亮度调节 0x00~0xff
    0xa1, //--set segment re-map 0 to 127
    0xa6, //--set normal display
    0xa8, //--set multiplex ratio(1 to 64)
    0x3F, //
    0xa4, //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    0xd3, //-set display offset
    0x00, //-not offset
    0xd5, //--set display clock divide ratio/oscillator frequency
    0xf0, //--set divide ratio
    0xd9, //--set pre-charge period
    0x22, //
    0xda, //--set com pins hardware configuration
    0x12,
    0xdb, //--set vcomh
    0x20, //0x20,0.77xVcc
    0x8d, //--set DC-DC enable
    0x14, //
    0xaf //--turn on oled panel
	};
		uint8_t result=0;
	result=IIC_OLED_SendBuffer(0x78,0x00,28,startcmd);
	return result;
}

void OLED_ON(void)//从休眠中唤醒oled
{
	uint8_t i2ccmd[3]={
    0X8D,  //设置电荷泵
    0X14,  //开启电荷泵
    0XAF  //OLED唤醒
	};
	IIC_OLED_SendBuffer(0x78,0x00,3,i2ccmd);
}
/**
  * @brief  OLED_SetPos，设置光标
  * @param  x,光标x位置
    *                   y，光标y位置
  * @retval 无
  */
void OLED_SetPos(unsigned char x, unsigned char y) //设置起始点坐标 如果y坐标过大则自动取余
{ 
	uint8_t i2ccmd[3]={
    0xb0+((y%8)+OLED_PAGE_SHIFT)%8,
    ((x&0xf0)>>4)|0x10,
    (x&0x0f)|0x01};
	IIC_OLED_SendBuffer(0x78,0x00,3,i2ccmd);
}

 /**
  * @brief  OLED_Fill，填充整个屏幕
  * @param  fill_Data:要填充的数据
    * @retval 无
  */
void OLED_Fill(unsigned char fill_Data)//全屏填充
{
    unsigned char m;
    for(m=0;m<8;m++)
    {
			uint8_t i2ccmd[3]={
        0xb0+m,       //page0-page1
        0x00,    //low column start address
        0x10     //high column start address
			};
			IIC_OLED_SendBuffer(0x78,0x00,3,i2ccmd);
                IIC_OLED_Send_normal(0x78,0x40,128,fill_Data);
    }
}

void OLED_Full_Print(uint8_t* p)//全屏打印 要求行列式存储数组不索引
{
	uint16_t j;
	OLED_SetPos(0,0);
	for(j=0;j<8;j++)
	{
			IIC_OLED_SendBuffer(0x78,0x40,128,(uint8_t*)&p[j*128]);
	}
}
void OLED_OPEN_SCREEN(void)
{
	OLED_Full_Print(OPEN_SCREEN);
}
void OLED_CLS(void)//清屏
{
    OLED_Fill(0x00);
	OLED_SetPos(0,0);
}


/**
  * @brief  OLED_ShowStr，显示codetab.h中的ASCII字符,有6*8和8*16可选择
  * @param  x,y : 起始点坐标(x:0~127, y:0~7);
    *                   ch[] :- 要显示的字符串; 
*                   TextSize : 字符大小(1:6*8 ; 2:8*16 ; 3: 16x16)
    * @retval 屏幕终点坐标
  */
point OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
    unsigned char c = 0,j = 0;
    switch(TextSize)
    {
        case 1:
        {
            while(ch[j] != '\0')
            {
				if(ch[j]=='\n')
				{
					IIC_OLED_Send_normal(0x78,0x40,128-x,0x00);//填充剩余空的地方
					x = 0;
					y++;
					j++;
					OLED_SetPos(x,y);
					continue;
				}
                c = ch[j] - 32;
                if(x > 122)
                {
                    x = 0;
                    y++;
                }
                OLED_SetPos(x,y);
                    IIC_OLED_SendBuffer(0x78,0x40,6,(uint8_t*)F6x8[c]);
                x += 6;
                j++;
            }
						if(x > 122)
                {
                    x = 0;
                    y++;
                }
								point re={x,y};
								return re;
        }
        case 2:
        {
            while(ch[j] != '\0')
            {
				if(ch[j]=='\n')
				{
					OLED_SetPos(x,y);
					IIC_OLED_Send_normal(0x78,0x40,128-x,0x00);
					OLED_SetPos(x,y+1);
					IIC_OLED_Send_normal(0x78,0x40,128-x,0x00);
					x = 0;
					y+=2;
					j++;
					OLED_SetPos(x,y);
					continue;
				}
                c = ch[j] - 32;
                if(x > 120)
                {
                    x = 0;
                    y+=2;
                }
                OLED_SetPos(x,y);
                    IIC_OLED_SendBuffer(0x78,0x40,8,(uint8_t*)&F8X16[c*16]);
                OLED_SetPos(x,y+1);
                   IIC_OLED_SendBuffer(0x78,0x40,8,(uint8_t*)&F8X16[c*16+8]);
                x += 8;
                j++;
            }
						if(x > 120)
                {
                    x = 0;
                    y+=2;
                }
						point re={x,y};
						return re;
        }
				case 3:
        {
            while(ch[j]!=255)
            {
				if(ch[j]=='\n')
				{
					x = 0;
					y+=2;
					j++;
					continue;
				}
                if(x > 112)
                {
                    x = 0;
                    y+=2;
                }
                OLED_SetPos(x,y);
                    IIC_OLED_SendBuffer(0x78,0x40,16,(uint8_t*)&myf[ch[j]][0]);
                OLED_SetPos(x,y+1);
                   IIC_OLED_SendBuffer(0x78,0x40,16,(uint8_t*)&myf[ch[j]][16]);
                x += 16;
                j++;
            }
						if(x > 112)
                {
                    x = 0;
                    y+=2;
                }
						point re={x,y};
						return re;
        }
    }
		point re={0,0};
		return re;
}
#ifdef OLED_PUTC
//方便串口重定向所出的文件
#define OLED_MAX_BUF 128
uint8_t oledbuf[OLED_MAX_BUF]={0};
int16_t oled_point=0;//指向当前屏幕缓存区尾部
int fputc(int ch, FILE *f)//将printf重定向到oled的缓存
{
	if(oled_point<OLED_MAX_BUF)
	{
		oledbuf[oled_point++]=ch;
		return (ch);
	}else return -1;
} 
//应该保证如下函数进行周期调用 以达到缓存区到达屏幕显示的效果
#ifdef OLED_AS_CMD		//使用 趋向命令行模式
point screen_line={0,0};//指向屏幕当前光标
void printoled(void)
{
	if(oled_point==0)return;
	screen_line=OLED_ShowStr(screen_line.x,screen_line.y,oledbuf,2);//打印出字符串 记录当前坐标位置
	if(screen_line.y>=8)screen_line.y-=8;
	//清空缓存区
	oled_point=0;
	for(uint8_t i=0;oledbuf[i]!=0;i++)oledbuf[i]=0;
}
#elif defined OLED_AS_SCREEN  //使用趋向屏幕类型模式
void printoled(void)
{
	if(oled_point==0)return;
	OLED_ShowStr(0,0,oledbuf,2);//打印出字符串 记录当前坐标位置
	//清空缓存区
	oled_point=0;
	for(uint8_t i=0;oledbuf[i]!=0;i++)oledbuf[i]=0;
}

#endif

#endif
