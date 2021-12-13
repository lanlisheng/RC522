/*
 * Filename: Core\Inc\light.h
 * Path: Core\Inc
 * Created Date: Tuesday, August 4th 2020, 3:43:01 pm
 * Author: L.S.
 *
 * Copyright (c) 2020 op-icv
 */

#ifndef __LIGHT_H
#define __LIGHT_H

#include "ESP8266.h"
#include "atk_m750.h"
#include "gpio.h"
#include "main.h"
#include "tim.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>

typedef enum {
  Light_Status_Red_ON = 0x00,
  Light_Status_Red_Flashing,
  Light_Status_Yellow_Flashing,
  Light_Status_Green_ON,
  Light_Status_Green_Flashing,
} Light_Status;

typedef struct {
  uint8_t VIN[17];
  uint8_t Light_Red_time;
  uint8_t Light_Yellow_time;
  uint8_t Light_Green_time;
  uint8_t Light_time;
  volatile Light_Status Light_status;
} Light_Data_Type;

// extern uint8_t Light_time;
extern Light_Data_Type Light;
extern uint8_t Transmit_Buff[24];

void Light_init(void);
void Light_Data_Transmit(void);

#endif

// /*
//  * Filename: Core\Inc\light.h
//  * Path: Core\Inc
//  * Created Date: Tuesday, August 4th 2020, 3:43:01 pm
//  * Author: L.S.
//  *
//  * Copyright (c) 2020 op-icv
//  */

// #ifndef __LIGHT_H
// #define __LIGHT_H

// #include "ESP8266.h"
// #include "atk_m750.h"
// #include "gpio.h"
// #include "main.h"
// #include "tim.h"
// #include "usart.h"
// #include <stdio.h>
// #include <string.h>

// typedef enum {
//   Light_Status_Red_ON = 0x00,
//   Light_Status_Red_Flashing,
//   Light_Status_Yellow_Flashing,
//   Light_Status_Green_ON,
//   Light_Status_Green_Flashing,
// } Light_Status;

// typedef struct {
//   uint8_t VIN[17];
//   uint8_t Light_Red_time;
//   uint8_t Light_Yellow_time;
//   uint8_t Light_Green_time;
//   uint8_t Light_time;
//   Light_Status Light_status;
// } Light_Data_Type;

// // extern uint8_t Light_time;
// extern Light_Data_Type Light;
// extern uint8_t Transmit_Buff[58];

// void Light_init(void);
// void Light_Data_Transmit(void);

// #endif
