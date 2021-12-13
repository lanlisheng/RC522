#ifndef __ESP8266_H
#define __ESP8266_H

#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>
#include <string.h>
#include "usart.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////

#define Uart_TxBuff_Size 100
#define Uart_RxBuff_Size 100

void atk_8266_init(void);

void Uart_printf(UART_HandleTypeDef *huart, char *fmt, ...);

uint8_t* atk_8266_check_cmd(uint8_t *str);
uint8_t atk_8266_apsta_check(void);
uint8_t atk_8266_send_data(uint8_t *data,uint8_t *ack,uint16_t waittime);
uint8_t atk_8266_send_cmd(uint8_t *cmd,uint8_t *ack,uint16_t waittime);
uint8_t atk_8266_quit_trans(void);
uint8_t atk_8266_consta_check(void);

void atk_8266_get_wanip(uint8_t* ipbuf);

int atk_8266_station_client_tcp_connect(void);
int atk_8266_station_wifi_connect(void);
void atk_8266_station_send(uint8_t *pData, uint8_t Size);

//用户配置参数
extern const uint8_t* portnum;			//连接端口
extern const uint8_t* wifista_ssid;		//WIFI STA SSID
extern const uint8_t* wifista_encryption;//WIFI STA 加密方式
extern const uint8_t* wifista_password; 	//WIFI STA 密码

extern uint8_t Uart_Rxtemp;
extern uint16_t Uart_Rxcount;
extern uint8_t Uart_RxBuff[Uart_RxBuff_Size];
extern uint8_t ESP8266_Connect_Flag;

#endif
