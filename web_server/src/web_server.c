/******************** (C) COPYRIGHT 2011 ÃÔÄãÇ¶ÈëÊ½¿ª·¢¹¤×÷ÊÒ ********************
 * ÎÄ¼şÃû  £ºsimple_server.c
 * ÃèÊö    £ºweb·şÎñ³ÌĞòÓ¦ÓÃº¯Êı¿â
 *          
 * ÊµÑéÆ½Ì¨£ºÒ°»ğSTM32¿ª·¢°å
 * Ó²¼şÁ¬½Ó£º ------------------------------------
 *           |PB13         £ºENC28J60-INT (Ã»ÓÃµ½)|
 *           |PA6-SPI1-MISO£ºENC28J60-SO          |
 *           |PA7-SPI1-MOSI£ºENC28J60-SI          |
 *           |PA5-SPI1-SCK £ºENC28J60-SCK         |
 *           |PA4-SPI1-NSS £ºENC28J60-CS          |
 *           |PE1          £ºENC28J60-RST (Ã»ÓÃ)  |
 *            ------------------------------------
 * ¿â°æ±¾  £ºST3.0.0
 *
 * ×÷Õß    £ºfire  QQ: 313303034
 * ²©¿Í    £ºfirestm32.blog.chinaunix.net
**********************************************************************************/
#include "phy_mac_interface.h"
#include "ip_arp_udp_tcp.h"
#include "net.h"
#include "web_server.h"
#include "web_interface.h"
#include <string.h>



extern WebServerInterface *__wFace;

#define PSTR(s) s	

/* LED1 ÁÁÃğºê¶¨Òå */
#define LED1_ON()  GPIO_SetBits(GPIOC, GPIO_Pin_13);
#define LED1_OFF()  GPIO_ResetBits(GPIOC, GPIO_Pin_13);


/* macµØÖ·ºÍipµØÖ·ÔÚ¾ÖÓòÍøÄÚ±ØĞëÎ¨Ò»£¬·ñÔò½«ÓëÆäËûÖ÷»ú³åÍ»£¬µ¼ÖÂÁ¬½Ó²»³É¹¦ */
static unsigned char mymac[6] = {0x54,0x55,0x58,0x10,0x00,0x24};
static unsigned char myip[4] = {192,168,111,15};

/* ipµØÖ·(»òÕßÊÇDNSµÄÃû×Ö£¬Èç¹ûÓĞDNS·şÎñÆ÷µÄ»°)£¬ipµØÖ·±ØĞëÒÔ"/"½áÎ² */
static char baseurl[]="http://192.168.111.15/";

/* tcp/www¼àÌı¶Ë¿ÚºÅ£¬·¶Î§Îª:1-254 */
static unsigned int mywwwport = 80; 

/* udp ¼àÌı¶Ë¿ÚºÅ£¬¼´±¾µØ(¿ª·¢°å)¶Ë¿ÚºÅ */
static unsigned int myudpport = 1200; 

/* ·¢ËÍÊı¾İ»º³åÇø */
#define BUFFER_SIZE 1500
static unsigned char buf[BUFFER_SIZE+1];

/* ÃÜÂë,²»ÄÜ´óÓÚ9¸ö×Ö·û(Ö»ÓĞÃÜÂëµÄÇ°5Î»»á±»¼ì²â)£¬(×Ö·ûÏŞ¶¨Îª£ºa-z,0-9) */
static char password[]="123456"; 

/*
 * º¯ÊıÃû£ºverify_password
 * ÃèÊö  £ºÈ·ÈÏÃÜÂë
 * ÊäÈë  £ºstr
 * Êä³ö  £ºÎŞ
 * ·µ»Ø  £º
 */ 
unsigned char verify_password(char *str)
{
    // the first characters of the received string are
    // a simple password/cookie:
    if (strncmp(password,str,5)==0)
    {
        return(1);
    }
    return(0);
}

/*
 * º¯ÊıÃû£ºanalyse_get_url
 * ÃèÊö  £ºtakes a string of the form password/commandNumber and analyse it
 * ÊäÈë  £ºstr
 * Êä³ö  £º-1 invalid password, otherwise command number
 *         -2	no command given but password valid
 * ·µ»Ø  £º
 */
unsigned char analyse_get_url(char *str)
{
    unsigned char i=0;
    if (verify_password(str)==0)
    {
        return(-1);
    }
    // find first "/"
    // passw not longer than 9 char:
    while(*str && i<10 && *str >',' && *str<'{')
    {
        if (*str=='/')
        {
            str++;
            break;
        }
        i++;
        str++;
    }
    if (*str < 0x3a && *str > 0x2f)
    {
        // is a ASCII number, return it
        return(*str-0x30);
    }
    return(-2);
}

/*
 * º¯ÊıÃû£ºprint_webpage
 * ÃèÊö  £º½«Êı¾İĞ´µ½tcpµÄ·¢ËÍ»º³åÇø(ÒÔÊµÏÖÒ»¸öÍøÒ³)
 * ÊäÈë  £º-buf
 *         -on_off
 * Êä³ö  £ºÎŞ
 * ·µ»Ø  £º-plen
 */
unsigned int print_webpage(unsigned char *buf,unsigned char on_off)
{
    unsigned int plen;
		/* ĞÂ½¨Ò»¸öÍøÒ³£¬¾ÍÏñĞÂ½¨Ò»¸öÎÄ¼şÒ»Ñù */
    plen=fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nPragma: no-cache\r\n\r\n"));
	
		/* ÔÚÍøÒ³ÕıÖĞÑëÌîĞ´ĞÅÏ¢ */
    plen=fill_tcp_data_p(buf,plen,PSTR("<center><p> Ñîñòîÿíèå ñâåòîäèîäà: "));

    if (on_off)
    {
        plen=fill_tcp_data_p(buf,plen,PSTR("<font color=\"#00FF00\"> Âêëş÷åí</font>"));
    }
    else
    {
        plen=fill_tcp_data_p(buf,plen,PSTR("Âûêëş÷åí"));
    }

    plen=fill_tcp_data_p(buf,plen,PSTR(" <small><a href=\""));
    plen=fill_tcp_data(buf,plen,baseurl);		 // ÒªÔÚä¯ÀÀÆ÷ÖĞÊäÈëµÄÍøÖ·
    plen=fill_tcp_data(buf,plen,password);	 // ½ô¸úÔÚÍøÖ·ºóÃæµÄÃÜÂë
    plen=fill_tcp_data_p(buf,plen,PSTR("\">[Âêëş÷èòü/Âûêëş÷èòü]</a></small></p>\n<p><a href=\""));
    // the url looks like this http://baseurl/password/command
    plen=fill_tcp_data(buf,plen,baseurl);
    plen=fill_tcp_data(buf,plen,password);
    if (on_off)
    {
        plen=fill_tcp_data_p(buf,plen,PSTR("/0\">Ñâåòîäèîä ¹1 Âûêëş÷èòü</a><p>"));
    }
    else
    {
        plen=fill_tcp_data_p(buf,plen,PSTR("/1\">Ñâåòîäèîä ¹1 Âêëş÷èòü</a><p>"));
    }
    plen=fill_tcp_data_p(buf,plen,PSTR("</center><hr><br><center>****************5.34****************\n</center>"));
    //plen=fill_tcp_data_p(buf,plen,PSTR("ÕâÊÇÒ»¸ö web ²âÊÔ³ÌĞò"));
    
    return(plen);
}

/*
 * º¯ÊıÃû£ºWeb_Server
 * ÃèÊö  £ºÔÚä¯ÀÀÆ÷ÉÏ´´½¨Ò»¸öweb·şÎñÆ÷£¬Í¨¹ıwebÀïÃæµÄÃüÁîÀ´¿ØÖÆ¿ª·¢°åÉÏµÄLEDµÄÁÁÃğ¡£
 * ÊäÈë  £ºÎŞ
 * Êä³ö  £ºÎŞ
 * ·µ»Ø  £º-0 ÔËĞĞ³É¹¦
 * Ó¦ÓÃ  £º1 ÔÚPC»úµÄDOS½çÃæÊäÈë£º ping 192.168.1.15 (¿´ÄÜ·ñpingÍ¨)
 *         2 ÔÚIEä¯ÀÀÆ÷ÖĞÊäÈë£ºhttp://192.168.1.15/123456 Ôò»á³öÏÖÒ»¸öÍøÒ³£¬Í¨¹ıÍøÒ³
 *           ÖĞµÄÃüÁî¿ÉÒÔ¿ØÖÆ¿ª·¢°åÖĞµÄLEDµÄÁÁÃğ
 */
int Web_Server(void)
{   
  unsigned int plen, i1 = 0;
  unsigned int dat_p;
  unsigned char i = 0;
  unsigned char cmd, *buf1;
  unsigned int payloadlen = 0;
  static PhyMacInterface ethernet;
  ethernet.init(mymac);
  init_ip_arp_udp_tcp(mymac,myip,mywwwport);

  while(1)
  {   
			// get the next new packet:			
      plen = ethernet.packetReceive(BUFFER_SIZE, buf);      
      
      // plen will ne unequal to zero if there is a valid packet (without crc error)			
      if(plen==0)
      {
          continue;
      }

      // arp is broadcast if unknown but a host may also
      // verify the mac address by sending it to 
      // a unicast address.		     
      if(eth_type_is_arp_and_my_ip(buf,plen))
      {
          make_arp_answer_from_request(buf);          
          continue;
      }
      
      // check if ip packets are for us:			
      if(eth_type_is_ip_and_my_ip(buf,plen)==0) 
      {
          continue;
      }      
      
      if(buf[IP_PROTO_P]==IP_PROTO_ICMP_V && buf[ICMP_TYPE_P]==ICMP_TYPE_ECHOREQUEST_V)
      {
          // a ping packet, let's send pong  DOS ÏÂµÄ ping ÃüÁî°ü		 
          make_echo_reply_from_request(buf, plen);          
          continue;
      }

/*-----------------tcp port www start, compare only the lower byte-----------------------------------*/
      if (buf[IP_PROTO_P]==IP_PROTO_TCP_V&&buf[TCP_DST_PORT_H_P]==0&&buf[TCP_DST_PORT_L_P]==mywwwport)
      {
          if (buf[TCP_FLAGS_P] & TCP_FLAGS_SYN_V)
          {
              make_tcp_synack_from_syn(buf);
              // make_tcp_synack_from_syn does already send the syn,ack
              continue;
          }
          if (buf[TCP_FLAGS_P] & TCP_FLAGS_ACK_V)
          {
              init_len_info(buf); // init some data structures
              // we can possibly have no data, just ack:
              dat_p=get_tcp_data_pointer();
              if (dat_p==0)
              {
                  if (buf[TCP_FLAGS_P] & TCP_FLAGS_FIN_V)
                  {
                      // finack, answer with ack
                      make_tcp_ack_from_any(buf);
                  }	  
                  // just an ack with no data, wait for next packet
                  continue;
              }
              if (strncmp("GET ",(char *)&(buf[dat_p]),4)!=0)
              {
                  // head, post and other methods:
                  //
                  // for possible status codes see:
                  // http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
                  plen=fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>200 OK</h1>"));
                  goto SENDTCP;
              }
              if (strncmp("/ ",(char *)&(buf[dat_p+4]),2)==0)
              {
                  plen=fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n"));
                  plen=fill_tcp_data_p(buf,plen,PSTR("<p>Usage: "));
                  plen=fill_tcp_data(buf,plen,baseurl);
                  plen=fill_tcp_data_p(buf,plen,PSTR("password</p>"));
                  goto SENDTCP;
              }
              cmd=analyse_get_url((char *)&(buf[dat_p+5]));
              // for possible status codes see:
              // http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
              if (cmd==-1)
              {
                  plen=fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 401 Unauthorized\r\nContent-Type: text/html\r\n\r\n<h1>401 Unauthorized</h1>"));
                  goto SENDTCP;
              } 
              if (cmd == 1)	           // ÓÃ»§³ÌĞò
              {			 
                  i=1;	__wFace->applySimpleCommand(cmd);							 // ÃüÁî = 1
              }
              if (cmd == 0)						 // ÓÃ»§³ÌĞò
              {													
                  i=0;	__wFace->applySimpleCommand(cmd);						 // ÃüÁî = 0
              }
              
              // if (cmd==-2) or any other value
              // just display the status:
              plen=print_webpage(buf,(i));
              
              SENDTCP:
              make_tcp_ack_from_any(buf);       // send ack for http get
              make_tcp_ack_with_data(buf,plen); // send data
              continue;
          }
      }
/*-------------------------------------- tcp port www end ---------------------------------------*/

/*--------------------- udp start, we listen on udp port 1200=0x4B0 -----------------------------*/
      if (buf[IP_PROTO_P]==IP_PROTO_UDP_V&&buf[UDP_DST_PORT_H_P]==4&&buf[UDP_DST_PORT_L_P]==0xb0)
      {
          payloadlen=	  buf[UDP_LEN_H_P];
          payloadlen=payloadlen<<8;
          payloadlen=(payloadlen+buf[UDP_LEN_L_P])-UDP_HEADER_LEN;
          //payloadlen=buf[UDP_LEN_L_P]-UDP_HEADER_LEN;
          
          ANSWER:
          //while(1){
          for(i1=0; i1<payloadlen; i1++) buf1[i1]=buf[UDP_DATA_P+i1];
          
          //make_udp_reply_from_request(buf,str,strlen(str),myudpport);
          make_udp_reply_from_request(buf, (char *)buf1,payloadlen,myudpport);          
          //}
      }
/*----------------------------------------udp end -----------------------------------------------*/
  }
          return (0);
}
/******************* (C) COPYRIGHT 2011 Ò°»ğÇ¶ÈëÊ½¿ª·¢¹¤×÷ÊÒ *****END OF FILE****/
