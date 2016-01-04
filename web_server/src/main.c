/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名  ：main.c
 * 描述    ：在浏览器上创建一个web服务器，通过web里面的命令来控制开发板上的LED的亮灭。
 * 应用    ：1 在PC机的DOS界面输入： ping 192.168.1.15 ，看能否ping通。
 *           2 在IE浏览器中输入：http://192.168.1.15/123456 则会出现一个网页，通过网页
 *             中的命令可以控制开发板中的LED的亮灭。
 *          
 * 实验平台：野火STM32开发板（纯手工制作^_^）
 * 库版本  ：ST3.0.0
 *
 * 作者    ：fire  QQ: 313303034 
 * 博客    ：firestm32.blog.chinaunix.net
**********************************************************************************/
#include "stm32f10x.h"
#include "usart1.h"
#include "led.h"
#include "spi_enc28j60.h"
#include "web_server.h"

/*
 * 函数名：main
 * 描述  ：主函数
 * 输入  ：无
 * 输出  ：无	
 */
int main (void)
{
  /* 配置系统时钟为72M */
  SystemInit();
	
	/* 配置 LED */ 
  LED_GPIO_Config();	
  
	/* ENC28J60 SPI 接口初始化 */
  SPI_Enc28j60_Init();
	
	/* ENC28J60 WEB 服务程序 */  
  Web_Server();

	//return 0;
}



/******************* (C) COPYRIGHT 2011 野火嵌入式开发工作室 *****END OF FILE****/
