/******************** (C) COPYRIGHT 2011 ����Ƕ��ʽ���������� ********************
 * �ļ���  ��spi.c
 * ����    ��ENC28J60(��̫��оƬ) SPI�ӿ�Ӧ�ú�����
 *          
 * ʵ��ƽ̨��Ұ��STM32������
 * Ӳ�����ӣ� ------------------------------------
 *           |PB13         ��ENC28J60-INT (û�õ�)|
 *           |PA6-SPI1-MISO��ENC28J60-SO          |
 *           |PA7-SPI1-MOSI��ENC28J60-SI          |
 *           |PA5-SPI1-SCK ��ENC28J60-SCK         |
 *           |PA4-SPI1-NSS ��ENC28J60-CS          |
 *           |PE1          ��ENC28J60-RST (û��)  |
 *            ------------------------------------
 * ��汾  ��ST3.0.0
 * ����    ��fire  QQ: 313303034
 * ����    ��firestm32.blog.chinaunix.net 
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

