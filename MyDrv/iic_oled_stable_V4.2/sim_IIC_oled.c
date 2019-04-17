//OLED专用模拟iic文件
//creae by WMD
//更新时间 17.11.4
//本模拟iic专门适配oled屏幕
#include "stm32f4xx_hal.h"
#include "main.h"
//当oled需要模拟iic来运行的时候只需要加下面这一句在iic_oled.c中就可以了
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

#define IIC_OLED_DELAY IIC_OLED_Delay(10);//如果发现屏幕不能输出 稍微调高 如果发现屏幕刷新过慢 稍微调低


/**
  * @brief  IIC向设备的某一个地址写入指定长度的数据
  * @param  设备地址
  * @param  寄存器地址
  * @param  数据长度
  * @param  要传输的数据内存地址
  * @retval 0 成功		1 失败
  */
uint8_t IIC_OLED_SendBuffer(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data)
{
	uint8_t i;
	
	if(IIC_OLED_Start())	//启动失败
	{
		return 1;
	}
	//IIC_OLED_SendByte(addr << 1);//发送设备地址 这个是原版的需要移位 但是我的不需要移位
	IIC_OLED_SendByte(addr);//发送设备地址
	if(IIC_OLED_WaitACK())
	{
		IIC_OLED_Stop();
		return 1;
	}
	IIC_OLED_SendByte(reg);//发送内存地址
	IIC_OLED_WaitACK();
	for(i = 0; i < len; i++)//按字节发送指定长度的数据
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
uint8_t IIC_OLED_Send_normal(uint8_t addr, uint8_t reg, uint8_t len, uint8_t data)//这个函数和上面的区别是data不自加
{
	uint8_t i;
	
	if(IIC_OLED_Start())	//启动失败
	{
		return 1;
	}
	//IIC_OLED_SendByte(addr << 1);//发送设备地址 这个是原版的需要移位 但是我的不需要移位
	IIC_OLED_SendByte(addr);//发送设备地址
	if(IIC_OLED_WaitACK())
	{
		IIC_OLED_Stop();
		return 1;
	}
	IIC_OLED_SendByte(reg);//发送内存地址
	IIC_OLED_WaitACK();
	for(i = 0; i < len; i++)//按字节发送指定长度的数据
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

//用于us级延时
void IIC_OLED_Delay(uint32_t time)
{
	while(time--);
}


/**
  * @brief  产生IIC起始信号
  * @param  void
  * @retval 0 启动成功		1 启动失败
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
  * @brief  产生IIC停止信号
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
  * @brief  产生IIC ACK信号
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
  * @brief  产生IIC NACK信号
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
  * @brief  等待IIC NACK信号
  * @param  void
  * @retval 0 接收ACK成功		1 接收ACK失败 
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
  * @brief  IIC发送一个byte
  * @param  要发送的byte
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
  * @brief  IIC接收一个byte
  * @param  void
  * @retval 接收到的byte
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







