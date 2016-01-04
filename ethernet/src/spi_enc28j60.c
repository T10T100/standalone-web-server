/******************** (C) COPYRIGHT 2011 迷你嵌入式开发工作室 ********************
 * 文件名  ：spi.c
 * 描述    ：ENC28J60(以太网芯片) SPI接口应用函数库
 *          
 * 实验平台：野火STM32开发板
 * 硬件连接： ------------------------------------
 *           |PB13         ：ENC28J60-INT (没用到)|
 *           |PA6-SPI1-MISO：ENC28J60-SO          |
 *           |PA7-SPI1-MOSI：ENC28J60-SI          |
 *           |PA5-SPI1-SCK ：ENC28J60-SCK         |
 *           |PA4-SPI1-NSS ：ENC28J60-CS          |
 *           |PE1          ：ENC28J60-RST (没用)  |
 *            ------------------------------------
 * 库版本  ：ST3.0.0
 * 作者    ：fire  QQ: 313303034
 * 博客    ：firestm32.blog.chinaunix.net 
**********************************************************************************/
#include "spi_enc28j60.h"
#include "init.h"


extern SPI_HandleTypeDef SpiHandle_Enc28j60;
unsigned char temp[4];
unsigned char SPI1_ReadWrite(unsigned char data)
{
    
    while ((SPI4->SR & SPI_FLAG_TXE) == 0);
    *(__IO uint8_t *)&SpiHandle_Enc28j60.Instance->DR = data;
    while ((SPI4->SR & SPI_FLAG_RXNE) == 0);
    data =  *(__IO uint8_t *)&SpiHandle_Enc28j60.Instance->DR;
    /*
    temp[0] = data;
    HAL_SPI_TransmitReceive(&SpiHandle_Enc28j60, temp, temp, 1, 10);
    data = temp[0];
    */
    return data;
}

