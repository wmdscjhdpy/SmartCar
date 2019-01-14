#include "bsp.h"
#include "LDchip.h"
#include "Reg_RW.h"
/***********************************************************
* ��    �ƣ�static uint8_t spi_send_byte(uint8_t byte)
* ��    �ܣ� дSPI
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
**********************************************************/ 
static uint8_t spi_send_byte(uint8_t byte)
{
    uint8_t receive;
	HAL_SPI_TransmitReceive(&hspi1,&byte,&receive,1,0xff);
    return receive;
}
/***********************************************************
* ��    �ƣ�void LD_WriteReg(uint8_t data1,uint8_t data2)
* ��    �ܣ� дld3320�Ĵ���
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
**********************************************************/ 
void LD_WriteReg(uint8_t data1,uint8_t data2)
{
	LD_SPIS_L();
    
	LD_CS_L();
    LD3320_delay(5);
    
	spi_send_byte(0x04);
    LD3320_delay(1);

	spi_send_byte(data1);
    LD3320_delay(1);

	spi_send_byte(data2);
    LD3320_delay(1);

	LD_CS_H();

}
/***********************************************************
* ��    �ƣ�uint8_t LD_ReadReg(uint8_t reg_add)
* ��    �ܣ���ld3320�Ĵ���
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
**********************************************************/ 
uint8_t LD_ReadReg(uint8_t reg_add)
{
	LD_SPIS_L();
	uint8_t i;

	LD_CS_L();
    LD3320_delay(5);
    
	spi_send_byte(0x05);
    LD3320_delay(1);

	spi_send_byte(reg_add);
    LD3320_delay(1);
    
	i=spi_send_byte(0x00);	/*��SPI*/
    LD3320_delay(1);

	LD_CS_H();

	return(i);
}



