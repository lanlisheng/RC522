#include "ESP8266.h"

//用户配置区

//连接端口号:8087,可自行修改为其他端口.
const uint8_t *portnum = "8087";

//WIFI STA模式,设置要去连接的路由器无线参数,请根据你自己的路由器设置,自行修改.
const uint8_t *wifista_ssid = "Optimus-Prime-ASUS-2.4G"; //路由器SSID号 //dachuan-RSU-No63
const uint8_t *wifista_encryption = "wpawpa2_aes";       //wpa/wpa2 aes加密方式
const uint8_t *wifista_password = "prime2020";           //连接密码 //dachuantek
const uint8_t *wifista_ip = "192.168.50.214";            //192.168.9.1
const uint8_t *wifista_port = "8899";

///////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t Uart_TxBuff[Uart_TxBuff_Size];
uint8_t Uart_Rxtemp;
uint8_t Uart_RxBuff[Uart_RxBuff_Size];
uint8_t ESP8266_Connect_Flag = 0;

uint8_t led0sta = 1;

void Uart_printf(UART_HandleTypeDef *huart, char *fmt, ...)
{
  uint16_t len;
  va_list ap;
  va_start(ap, fmt);
  vsprintf((char *)Uart_TxBuff, fmt, ap);
  va_end(ap);
  len = strlen((const char *)Uart_TxBuff); //此次发送数据的长度
  HAL_UART_Transmit(huart, Uart_TxBuff, len, 100);
}

//ATK-ESP8266发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果
//    其他,期待应答结果的位置(str的位置)
uint8_t *atk_8266_check_cmd(uint8_t *str)
{
  char *strx = 0;
  if (__HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE) == SET) //串口空闲中断标志，接收到一帧数据
  {
    strx = strstr((const char *)Uart_RxBuff, (const char *)str); //检查接收缓存Uart_RxBuff中是否有str数据
  }
  return (uint8_t *)strx;
}
//向ATK-ESP8266发送命令
//cmd:发送的命令字符串
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
uint8_t atk_8266_send_cmd(uint8_t *cmd, uint8_t *ack, uint16_t waittime)
{
  uint8_t res = 0;
  memset(Uart_RxBuff, 0, sizeof(Uart_RxBuff));
  Uart_printf(&huart3, "%s\r\n", cmd); //发送命令
  if (ack && waittime)                 //需要等待应答
  {
    HAL_UART_Receive(&huart3, Uart_RxBuff, Uart_RxBuff_Size, 10);
    while (--waittime) //等待倒计时
    {
      HAL_Delay(10);
      if (__HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE) == SET) //串口空闲中断标志，接收到一帧应答
      {
        if (atk_8266_check_cmd(ack)) //检查应答结果，如果结果无误，跳出循环
        {
          break; //得到有效数据
        }
        res = 1;
      }
    }
    if (waittime == 0)
      res = 1;
  }
  return res;
}
//向ATK-ESP8266发送指定数据
//data:发送的数据(不需要添加回车了)
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
uint8_t atk_8266_send_data(uint8_t *data, uint8_t *ack, uint16_t waittime)
{
  uint8_t res = 0;
  Uart_printf(&huart3, "%s", data); //发送命令
  if (ack && waittime)              //需要等待应答
  {
    HAL_UART_Receive(&huart3, Uart_RxBuff, Uart_RxBuff_Size, 100);
    while (--waittime) //等待倒计时
    {
      HAL_Delay(10);
      if (__HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE) == SET) //串口空闲中断标志，接收到应答
      {
        if (atk_8266_check_cmd(ack)) //检查应答结果，如果结果无误，跳出循环
        {
          break; //得到有效数据
        }
        res = 1;
      }
    }
    if (waittime == 0)
      res = 1;
  }
  return res;
}
//ATK-ESP8266退出透传模式
//返回值:0,退出成功;
//       1,退出失败
uint8_t atk_8266_quit_trans(void)
{
  while (__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC) == RESET)
    //等待发送完成
    ;
  // strcpy(Uart_TxBuff, "+++");
  // HAL_UART_Transmit(&huart3, Uart_TxBuff, 3, 100);
  Uart_printf(&huart3, "+++");
  // USART3->TDR = '+';
  // HAL_Delay(15); //大于串口组帧时间(10ms)
  // while ((USART3->ISR & 0X40) == 0)
  // 	; //等待发送空
  // USART3->TDR = '+';
  // HAL_Delay(15); //大于串口组帧时间(10ms)
  // while ((USART3->ISR & 0X40) == 0)
  // 	; //等待发送空
  // USART3->TDR = '+';

  HAL_Delay(1000);                          //等待1s
  return atk_8266_send_cmd("AT", "OK", 20); //退出透传判断.
}
//获取ATK-ESP8266模块的AP+STA连接状态
//返回值:0，未连接;1,连接成功
uint8_t atk_8266_apsta_check(void)
{
  if (atk_8266_quit_trans())
    return 0;                                 //退出透传
  atk_8266_send_cmd("AT+CIPSTATUS", ":", 50); //发送AT+CIPSTATUS指令,查询连接状态
  if (atk_8266_check_cmd("+CIPSTATUS:0") && atk_8266_check_cmd("+CIPSTATUS:1") && atk_8266_check_cmd("+CIPSTATUS:2") && atk_8266_check_cmd("+CIPSTATUS:4"))
    return 0;
  else
    return 1;
}
//获取ATK-ESP8266模块的连接状态
//返回值:0,未连接;1,连接成功
uint8_t atk_8266_consta_check(void)
{
  uint8_t *p;
  uint8_t res;
  if (atk_8266_quit_trans())
    return 0;                                 //退出透传
  atk_8266_send_cmd("AT+CIPSTATUS", ":", 50); //发送AT+CIPSTATUS指令,查询连接状态
  p = atk_8266_check_cmd("+CIPSTATUS:");
  res = *p; //得到连接状态
  return res;
}

//获取Client ip地址
//ipbuf:ip地址输出缓存区
void atk_8266_get_wanip(uint8_t *ipbuf)
{
  uint8_t *p, *p1;
  if (atk_8266_send_cmd("AT+CIFSR", "OK", 5))
  {
    ipbuf[0] = 0; //获取WAN IP地址失败
    return;
  }
  p = atk_8266_check_cmd("\"");
  p1 = (uint8_t *)strstr((const char *)(p + 1), "\"");
  *p1 = 0;
  sprintf((char *)ipbuf, "%s", p + 1);
}

//获取AP+STA ip地址
//ipbuf:ip地址输出缓存区
uint8_t *atk_8266_get_ip(void)
{
  uint8_t *p;
  uint8_t *p1;
  uint8_t *p2;
  uint8_t *ipbuf;
  uint8_t *buf;
  p = malloc(32);     //申请32字节内存
  p1 = malloc(32);    //申请32字节内存
  p2 = malloc(32);    //申请32字节内存
  ipbuf = malloc(32); //申请32字节内存
  buf = malloc(32);   //申请32字节内存
  if (atk_8266_send_cmd("AT+CIFSR", "OK", 50))
  {
    *ipbuf = 0; //获取WAN IP地址失败
  }
  else
  {
    p = atk_8266_check_cmd("APIP,\"");
    p1 = (uint8_t *)strstr((const char *)(p + 6), "\"");
    p2 = p1;
    *p1 = 0;
    ipbuf = p + 6;
    sprintf((char *)buf, "AP IP:%s 端口:%s", ipbuf, (uint8_t *)portnum);
    p = (uint8_t *)strstr((const char *)(p2 + 1), "STAIP,\"");
    p1 = (uint8_t *)strstr((const char *)(p + 7), "\"");
    *p1 = 0;
    ipbuf = p + 7;
    sprintf((char *)buf, "STA IP:%s 端口:%s", ipbuf, (uint8_t *)portnum);
    free(p);   //释放内存
    free(p1);  //释放内存
    free(p2);  //释放内存
    free(buf); //释放内存
  }
  return ipbuf;
}

void atk_8266_init(void)
{
  while (atk_8266_send_cmd("AT", "OK", 20)) //检查WIFI模块是否在线
  {
    Uart_printf(&huart1, "\r\nConnecting 8266 fail\r\nRetrying...\r\n");
    atk_8266_quit_trans();                        //退出透传
    atk_8266_send_cmd("AT+CIPMODE=0", "OK", 200); //关闭透传模式
    Uart_printf(&huart1, "\r\nClose pass through mode\r\n");
    HAL_Delay(800);
  }
  Uart_printf(&huart1, "\r\nConnecting 8266 Successfully\r\n");
  while (atk_8266_send_cmd("ATE0", "OK", 20))
  {
    Uart_printf(&huart1, "\r\nERROR: Close commond echo\r\nRetrying...\r\n"); //关闭回显
    HAL_Delay(5);
  }
  Uart_printf(&huart1, "\r\nClose commond echo Successfully\r\n"); //关闭回显
  atk_8266_station_wifi_connect();
  atk_8266_station_client_tcp_connect();
}

//STA模式下wifi连接
int atk_8266_station_wifi_connect(void)
{
  uint8_t *p;
  p = malloc(64); //申请32个字节的内存
  uint8_t ipbuff[15];

  if (atk_8266_send_cmd("AT+CWMODE_DEF=1", "OK", 2))
  {
    Uart_printf(&huart1, "\r\nERROR:Set 8266 in Wifi Station mode\r\n"); //设置WIFI STA模式 失败
    free(p);
    return -1;
  }
  Uart_printf(&huart1, "\r\nSet 8266 in Wifi Station mode Successfully\r\n"); //设置WIFI STA模式 成功

  //  if (atk_8266_send_cmd("AT+CWJAP_DEF?", "busy", 1)==0)			//连接目标路由器
  //  {
  //  		Uart_printf(&huart1, "\r\n8266 is busy\r\n");
  //  		return -2;
  //  }
  sprintf((char *)p, "\"%s\"", wifista_ssid);
  if (atk_8266_send_cmd("AT+CWJAP_DEF?", p, 2)) //检查是否连接
  {
    sprintf((char *)p, "AT+CWJAP_DEF=\"%s\",\"%s\"", wifista_ssid, wifista_password); //设置无线参数:ssid,密码
    if (atk_8266_send_cmd(p, "OK", 5))                                                //连接目标路由器
    {
      Uart_printf(&huart1, "\r\nERROR: 8266 connect wifi \"%s\" Password: \"%s\"\r\nRetrying...\r\n", wifista_ssid, wifista_password);
      free(p);
      return -2;
    }
  }

  atk_8266_get_wanip(ipbuff);
  if (ipbuff[0] == 0)
  {
    Uart_printf(&huart1, "\r\nERROR: 8266 can not get ip\r\n");
    free(p);
    return -3;
  }

  //	Uart_printf(&huart1, "\r\n8266 Rx_buff: \"%s\"\r\n", Uart_RxBuff);
  Uart_printf(&huart1, "8266 connect wifi Successfully\r\n"); //连接WIFI 成功
  free(p);
  return 0;
}

//STA模式下TCP连接
int atk_8266_station_client_tcp_connect(void)
{
  uint8_t *p;
  p = malloc(64); //申请64个字节的内存

  sprintf((char *)p, "AT+CIPSTART=\"TCP\",\"%s\",%s", wifista_ip, wifista_port); //配置目标TCP服务器,及ID号，STA模式为client时，为1
  //AT+CIPSTART="TCP","192.168.50.214",23333
  if (atk_8266_send_cmd(p, "CONNECT", 5))
  {
    //	if (atk_8266_send_cmd(p, "ALREADY CONNECTED", 200) == 0)
    //	{
    //	  Uart_printf(&huart1, "\r\n8266 has connected server already\r\n");
    //	  break;
    //	}
    Uart_printf(&huart1, "\r\nERROR: 8266 connect \"%s : %s\"\r\nTry again...\r\n", wifista_ip, wifista_port);
    free(p);
    return -1;
  }
  Uart_printf(&huart1, "8266 connect TCP server Successfully\r\n"); //连接服务器成功
  ESP8266_Connect_Flag = 1;
  free(p);
  return 0;
}

//STA模式下开启透传
void atk_8266_station_send(uint8_t *pData, uint8_t Size)
{
  uint8_t *p;
  char *busy = "busy";
  p = malloc(32); //申请32个字节的内存

  sprintf((char *)p, "AT+CIPSENDEX=%d", Size); //输入透传数据大小
  if (atk_8266_send_cmd(p, "OK", 20))          //开启透传,若ERROR则重连wifi和服务器
  {
    if (strstr((const char *)Uart_RxBuff, (const char *)busy) == NULL)
    {
      Uart_printf(&huart1, "\r\nERROR: 8266 transmit error\r\n");
      ESP8266_Connect_Flag = 0;

      sprintf((char *)p, "\"%s\"", wifista_ssid);
      if (atk_8266_send_cmd("AT+CWJAP_DEF?", p, 2)) //检查Wifi是否连接
      {
        Uart_printf(&huart1, "\r\nERROR: 8266 connect WIFI error\r\nReconnecting...\r\n");
        atk_8266_station_wifi_connect();
      }

      if (atk_8266_send_cmd("AT+CIPSTATUS", "STATUS:3", 2)) //检查TCP服务器是否连接
      {
        Uart_printf(&huart1, "\r\nERROR: 8266 connect TCP Server error\r\nReconnecting...\r\n");
        atk_8266_station_client_tcp_connect(); //检查TCP服务器是否连接
      }
    }
  }

  HAL_UART_Transmit(&huart3, pData, Size, 800);

  free(p);
}
