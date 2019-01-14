#include "bsp.h"
#include "LDchip.h"
#include "Reg_RW.h"
/***********************************************************
* 名    称：static uint8_t spi_send_byte(uint8_t byte)
* 功    能： 写SPI
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
static uint8_t spi_send_byte(uint8_t byte)
{
    uint8_t receive;
	HAL_SPI_TransmitReceive(&hspi1,&byte,&receive,1,0xff);
    return receive;
}
/***********************************************************
* 名    称：void LD_WriteReg(uint8_t data1,uint8_t data2)
* 功    能： 写ld3320寄存器
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
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
* 名    称：uint8_t LD_ReadReg(uint8_t reg_add)
* 功    能：读ld3320寄存器
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
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
    
	i=spi_send_byte(0x00);	/*读SPI*/
    LD3320_delay(1);

	LD_CS_H();

	return(i);
}



