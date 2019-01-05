#ifndef IIC_OLED
#define IIC_OLED
#include "stm32f4xx_hal.h"//���ݾ����ͺ����и��������ļ�


typedef struct
{
	uint8_t x;
	uint8_t y;
}point;//����ṹ���Է���ĩβ����


#ifdef OLED_PUTC  //��������OLED�ض���printf
int fputc(int ch, FILE *f);
//�˴��������������� ������printfˢ�º͹ر�printfˢ��
#define EA_OLED __HAL_TIM_ENABLE_IT(&htim2,TIM_IT_UPDATE)
#define DA_OLED __HAL_TIM_DISABLE_IT(&htim2,TIM_IT_UPDATE)



#endif
uint8_t OLED_Init(void);
void OLED_ON(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
point OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void OLED_OPEN_SCREEN(void);

#endif
