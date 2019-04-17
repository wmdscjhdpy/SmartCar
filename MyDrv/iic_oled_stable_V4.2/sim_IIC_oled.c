//OLEDר��ģ��iic�ļ�
//creae by WMD
//����ʱ�� 17.11.4
//��ģ��iicר������oled��Ļ
#include "stm32f4xx_hal.h"
#include "main.h"
//��oled��Ҫģ��iic�����е�ʱ��ֻ��Ҫ��������һ����iic_oled.c�оͿ�����
//uint8_t IIC_OLED_SendBuffer(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data);

//SCL:PB5
//SDA:PB6
void IIC_OLED_Delay(uint32_t time);
uint8_t IIC_OLED_Start(void);
void IIC_OLED_Stop(void);
void IIC_OLED_ACK(void);
void IIC_OLED_NACK(void);
uint8_t IIC_OLED_WaitACK(void);
void IIC_OLED_SendByte(uint8_t byte);
uint8_t IIC_OLED_ReadByte(void);

#define IIC_OLED_SCL_L		HAL_GPIO_WritePin(IIC_SCL_GPIO_Port,IIC_SCL_Pin, GPIO_PIN_RESET)
#define IIC_OLED_SCL_H		HAL_GPIO_WritePin(IIC_SCL_GPIO_Port,IIC_SCL_Pin, GPIO_PIN_SET)
#define IIC_OLED_SDA_L		HAL_GPIO_WritePin(IIC_SDA_GPIO_Port,IIC_SDA_Pin, GPIO_PIN_RESET)
#define IIC_OLED_SDA_H		HAL_GPIO_WritePin(IIC_SDA_GPIO_Port,IIC_SDA_Pin, GPIO_PIN_SET)
#define	IIC_OLED_SDA_Read	HAL_GPIO_ReadPin(IIC_SDA_GPIO_Port,IIC_SDA_Pin)

#define IIC_OLED_DELAY IIC_OLED_Delay(10);//���������Ļ������� ��΢���� ���������Ļˢ�¹��� ��΢����


/**
  * @brief  IIC���豸��ĳһ����ַд��ָ�����ȵ�����
  * @param  �豸��ַ
  * @param  �Ĵ�����ַ
  * @param  ���ݳ���
  * @param  Ҫ����������ڴ��ַ
  * @retval 0 �ɹ�		1 ʧ��
  */
uint8_t IIC_OLED_SendBuffer(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data)
{
	uint8_t i;
	
	if(IIC_OLED_Start())	//����ʧ��
	{
		return 1;
	}
	//IIC_OLED_SendByte(addr << 1);//�����豸��ַ �����ԭ�����Ҫ��λ �����ҵĲ���Ҫ��λ
	IIC_OLED_SendByte(addr);//�����豸��ַ
	if(IIC_OLED_WaitACK())
	{
		IIC_OLED_Stop();
		return 1;
	}
	IIC_OLED_SendByte(reg);//�����ڴ��ַ
	IIC_OLED_WaitACK();
	for(i = 0; i < len; i++)//���ֽڷ���ָ�����ȵ�����
	{
		IIC_OLED_SendByte(data[i]);
		if(IIC_OLED_WaitACK())
		{
			IIC_OLED_Stop();
			return 1;
		}
	}
	IIC_OLED_Stop();
	return 0;
}
uint8_t IIC_OLED_Send_normal(uint8_t addr, uint8_t reg, uint8_t len, uint8_t data)//��������������������data���Լ�
{
	uint8_t i;
	
	if(IIC_OLED_Start())	//����ʧ��
	{
		return 1;
	}
	//IIC_OLED_SendByte(addr << 1);//�����豸��ַ �����ԭ�����Ҫ��λ �����ҵĲ���Ҫ��λ
	IIC_OLED_SendByte(addr);//�����豸��ַ
	if(IIC_OLED_WaitACK())
	{
		IIC_OLED_Stop();
		return 1;
	}
	IIC_OLED_SendByte(reg);//�����ڴ��ַ
	IIC_OLED_WaitACK();
	for(i = 0; i < len; i++)//���ֽڷ���ָ�����ȵ�����
	{
		IIC_OLED_SendByte(data);
		if(IIC_OLED_WaitACK())
		{
			IIC_OLED_Stop();
			return 1;
		}
	}
	IIC_OLED_Stop();
	return 0;
}

//����us����ʱ
void IIC_OLED_Delay(uint32_t time)
{
	while(time--);
}


/**
  * @brief  ����IIC��ʼ�ź�
  * @param  void
  * @retval 0 �����ɹ�		1 ����ʧ��
  */
uint8_t IIC_OLED_Start(void)
{
	IIC_OLED_SDA_H;
	IIC_OLED_SCL_H;
	IIC_OLED_DELAY;
	if(!IIC_OLED_SDA_Read)
	{
		return 1;
	}
	IIC_OLED_SDA_L;
	IIC_OLED_DELAY;
	if(IIC_OLED_SDA_Read)
	{
		return 1;
	}
	IIC_OLED_DELAY;
	return 0;
}


/**
  * @brief  ����IICֹͣ�ź�
  * @param  void
  * @retval void 
  */
void IIC_OLED_Stop(void)
{
	IIC_OLED_SCL_L;
	IIC_OLED_DELAY;
	IIC_OLED_SDA_L;
	IIC_OLED_DELAY;
	IIC_OLED_SCL_H;
	IIC_OLED_DELAY;
	IIC_OLED_SDA_H;
	IIC_OLED_DELAY;
}


/**
  * @brief  ����IIC ACK�ź�
  * @param  void
  * @retval void 
  */
void IIC_OLED_ACK(void)
{
	IIC_OLED_SCL_L;
	IIC_OLED_DELAY;
	IIC_OLED_SDA_L;
	IIC_OLED_DELAY;
	IIC_OLED_SCL_H;
	IIC_OLED_DELAY;
	IIC_OLED_SCL_L;
	IIC_OLED_DELAY;
}


/**
  * @brief  ����IIC NACK�ź�
  * @param  void
  * @retval void 
  */
void IIC_OLED_NACK(void)
{
	IIC_OLED_SCL_L;
	IIC_OLED_DELAY;
	IIC_OLED_SDA_H;
	IIC_OLED_DELAY;
	IIC_OLED_SCL_H;
	IIC_OLED_DELAY;
	IIC_OLED_SCL_L;
	IIC_OLED_DELAY;
}


/**
  * @brief  �ȴ�IIC NACK�ź�
  * @param  void
  * @retval 0 ����ACK�ɹ�		1 ����ACKʧ�� 
  */
uint8_t IIC_OLED_WaitACK(void)
{
	IIC_OLED_SCL_L;
	IIC_OLED_DELAY;
	IIC_OLED_SDA_H;
	IIC_OLED_DELAY;
	IIC_OLED_SCL_H;
	IIC_OLED_DELAY;
	if(IIC_OLED_SDA_Read)
	{
		IIC_OLED_SCL_L;
		return 1;
	}
	IIC_OLED_SCL_L;
	return 0;
}


/**
  * @brief  IIC����һ��byte
  * @param  Ҫ���͵�byte
  * @retval void
  */
void IIC_OLED_SendByte(uint8_t byte)
{
	uint8_t i = 8;
	while(i--)
	{
		IIC_OLED_SCL_L;
		IIC_OLED_DELAY;
		if(byte & 0x80)
		{
			IIC_OLED_SDA_H;
		}
		else
		{
			IIC_OLED_SDA_L;
		}
		byte <<= 1;
		IIC_OLED_DELAY;
		IIC_OLED_SCL_H;
		IIC_OLED_DELAY;
	}
	IIC_OLED_SCL_L;
}


/**
  * @brief  IIC����һ��byte
  * @param  void
  * @retval ���յ���byte
  */
uint8_t IIC_OLED_ReadByte(void)
{
	uint8_t i = 8;
	uint8_t byte = 0;
	
	IIC_OLED_SDA_H;
	while(i--)
	{
		byte <<= 1;
		IIC_OLED_SCL_L;
		IIC_OLED_DELAY;
		IIC_OLED_SCL_H;
		IIC_OLED_DELAY;
		if(IIC_OLED_SDA_Read)
		{
			byte |= 0x01;
		}
	}
	IIC_OLED_SCL_L;
	return byte;
}







