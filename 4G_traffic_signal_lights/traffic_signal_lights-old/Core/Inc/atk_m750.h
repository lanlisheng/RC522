#ifndef __ATK_M750_H
#define __ATK_M750_H

#include "RingBuffer.h"
#include "usart.h"
#include <string.h>

/**
 * ****************************************************************************
 * @file            dtu.c
 * @author          正点原子团队（ALIENTEK）
 * @version         V1.0
 * @data            2020-04-14
 * @brief           DTU驱动代码
 * @copyright       Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 * ****************************************************************************
 * @attention
 *
 * 实验平台：正点原子STM32F103开发板    +   正点原子ATK-M750/ATK-M751（4G DTU产品）
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20200414
 * 第一次发布
 * ****************************************************************************
*/

/****************************************************************************/

#define DTU_RX_CMD_BUF_SIZE (512)

typedef enum
{
    DTU_WORKMODE_NET = 0,  /*网络透传模式*/
    DTU_WORKMODE_HTTP,     /*http透传模式*/
    DTU_WORKMODE_MQTT,     /*mqtt透传模式*/
    DTU_WORKMODE_ALIYUN,   /*阿里云透传模式*/
    DTU_WORKMODE_ONENET,   /*OneNET透传模式*/
    DTU_WORKMODE_BAIDUYUN, /*百度云透传模式*/
} _dtu_work_mode_eu;

typedef struct
{
    uint32_t timeout; /*指令等待超时时间，单位：100ms*/
    char *read_cmd;   /*查询参数指令      请参考DTU AT用户手册说明进行填写*/
    char *write_cmd;  /*配置参数指令      请参考DTU AT用户手册说明进行填写*/
} _dtu_atcmd_st;

extern RingBuffer *p_uart3_rxbuf;
extern uint8_t uart_temp;

int send_cmd_to_dtu(char *cmd, char *ask, uint32_t timeout);
void dtu_get_urc_info(uint8_t ch);
void send_data_to_dtu(uint8_t *data, uint32_t size);
int dtu_config_init(_dtu_work_mode_eu work_mode);
int dtu_check_linkStatus(void);

#endif
