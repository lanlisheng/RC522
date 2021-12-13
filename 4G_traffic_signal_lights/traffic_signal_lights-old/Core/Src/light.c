/*
 * Filename: Core\Src\light.c
 * Path: Core\Src
 * Created Date: Tuesday, August 4th 2020, 3:42:55 pm
 * Author: L.S.
 *
 * Copyright (c) 2020 op-icv
 */

#include "light.h"
#include "atk_m750.h"
#include "cJSON.h"
#include "tim.h"
#include <stdio.h>

uint8_t Light_time = 0;
Light_Data_Type Light = {.VIN = "OPTIMUSPRIMERYG01",
                         .Light_status = Light_Status_Red_ON,
                         .Light_time = 0,
                         .Light_Red_time = 40,
                         .Light_Yellow_time = 5,
                         .Light_Green_time = 35};

/* Light Data to be transmited */
uint8_t Transmit_Buff[24] = {
    0xA5,                                           // Header,2
    0x5A, 0x03,                                     // Version,2
    0x00, 0x03,                                     // Identify,2
    0x00, 0x02,                                     // Format, 2, json
    0x00, 0x00,                                     // Timestamp,8
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, // Command,4
    0xFF, 0xFF, 0xFF, 0x00,                         // Length,4
    0x00, 0x00, 0x00,
};
// uint8_t Transmit_Buff[65] = {0};

void Light_init(void) {
  strcpy(Light.VIN, "OPTIMUSPRIMERYG01");
  // strcpy(Light.VIN, "GCYLGSWC5GBL8A006");
  Light.Light_Red_time = 40;
  Light.Light_Yellow_time = 3;
  Light.Light_Green_time = 20;
  Light.Light_status = Light_Status_Red_ON;

  HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED_Y_GPIO_Port, LED_Y_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
  Light.Light_time = Light.Light_Red_time;
  HAL_TIM_Base_Start_IT(&htim6);
}

/* Transmit Data */
/**
 * @brief Transmit Light Data by zigbee
 */
void Light_Data_Transmit(void) {
  uint32_t current_time = HAL_GetTick();
  uint32_t Data_length = 0;
  // uint8_t point = 0;

  Transmit_Buff[8] = (uint8_t)(current_time);
  Transmit_Buff[9] = (uint8_t)(current_time >> 8);
  Transmit_Buff[10] = (uint8_t)(current_time >> 16);
  Transmit_Buff[11] = (uint8_t)(current_time >> 24);

  cJSON *Light_Data_json = cJSON_CreateObject();

  cJSON_AddNumberToObject(Light_Data_json, "command", 0x09);
  cJSON_AddNumberToObject(Light_Data_json, "type", 0x03);
  cJSON_AddStringToObject(Light_Data_json, "vin", "OPTIMUSPRIMERYG01");
  // cJSON_AddNumberToObject(Light_Data_json, "signalStrength",
  //                         drf1609_signal.linkEndRouter_Singal);
  // cJSON_AddNumberToObject(Light_Data_json, "settingFlag", Setting_Flag);
  cJSON_AddNumberToObject(Light_Data_json, "runningStatus", Light.Light_status);
  cJSON_AddNumberToObject(Light_Data_json, "realTime", Light.Light_time);
  cJSON_AddNumberToObject(Light_Data_json, "redTime", Light.Light_Red_time);
  cJSON_AddNumberToObject(Light_Data_json, "greenTime", Light.Light_Green_time);

  char *out = cJSON_Print(Light_Data_json);

  Data_length = strlen(out) + 1;
  Transmit_Buff[20] = (uint8_t)(Data_length);
  Transmit_Buff[21] = (uint8_t)(Data_length >> 8);
  Transmit_Buff[22] = (uint8_t)(Data_length >> 16);
  Transmit_Buff[23] = (uint8_t)(Data_length >> 24);

  HAL_UART_Transmit(&huart1, Transmit_Buff,
                    (sizeof(Transmit_Buff) / sizeof(Transmit_Buff[0])), 1000);

  send_data_to_dtu(Transmit_Buff,
                   (sizeof(Transmit_Buff) / sizeof(Transmit_Buff[0]))); // DTU
  printf("%s\n", out);
  cJSON_Delete(Light_Data_json);
  // Transmit_Buff[point++] = 0xA5; // Header,2
  // Transmit_Buff[point++] = 0x5A;
  // Transmit_Buff[point++] = 0x03; // Version,1
  // Transmit_Buff[point++] = 0x00; // Reserved,2
  // Transmit_Buff[point++] = 0xFF;
  // Transmit_Buff[point++] = 0xFF; // Identify,2
  // Transmit_Buff[point++] = 0x02;
  // Transmit_Buff[point++] = 0x00; // Format,1
  // Transmit_Buff[point++] = 0xFF; // Timestamp,8
  // Transmit_Buff[point++] = 0xFF;
  // Transmit_Buff[point++] = 0xFF;
  // Transmit_Buff[point++] = 0xFF;
  // Transmit_Buff[point++] = 0xFF;
  // Transmit_Buff[point++] = 0xFF;
  // Transmit_Buff[point++] = 0xFF;
  // Transmit_Buff[point++] = 0xFF;
  // Transmit_Buff[point++] = 0x09; // Command,4
  // Transmit_Buff[point++] = 0x00;
  // Transmit_Buff[point++] = 0x00;
  // Transmit_Buff[point++] = 0x00;
  // Transmit_Buff[point++] = 41; // Length,4
  // Transmit_Buff[point++] = 0x00;
  // Transmit_Buff[point++] = 0x00;
  // Transmit_Buff[point++] = 0x00;

  // for (uint8_t i = 0; i < (sizeof(Light.VIN) / sizeof(Light.VIN[0])); i++) {
  //   // point += sprintf(Transmit_Buff + point, "%s", Light.VIN[i]);
  //   Transmit_Buff[point++] = Light.VIN[i];
  // }
  // // Transmit_Buff[point] = 0XFF;
  // Transmit_Buff[point++] = Light.Light_status; // sprintf补充/0，覆盖了
  // Transmit_Buff[point] = Light.Light_time;     //最后一个包

  // HAL_UART_Transmit(&huart1, Transmit_Buff,
  //                   (sizeof(Transmit_Buff) / sizeof(Transmit_Buff[0])),
  //                   1000);
  // // atk_8266_station_send(Transmit_Buff, (sizeof(Transmit_Buff) /
  // //                                       sizeof(Transmit_Buff[0]))); //
  // wifi

  // Transmit_Buff[20] = 41; // Length
  // // dtu_check_linkStatus();
  // send_data_to_dtu(Transmit_Buff,
  //                  (sizeof(Transmit_Buff) / sizeof(Transmit_Buff[0]))); //
  //                  DTU
  // HAL_UART_Transmit(&huart5, Transmit_Buff,
  //                   (sizeof(Transmit_Buff) / sizeof(Transmit_Buff[0])),
  //                   1000);
}

// /*
//  * Filename: Core\Src\light.c
//  * Path: Core\Src
//  * Created Date: Tuesday, August 4th 2020, 3:42:55 pm
//  * Author: L.S.
//  *
//  * Copyright (c) 2020 op-icv
//  */

// #include "light.h"
// #include <stdio.h>

// uint8_t Light_time = 0;
// Light_Data_Type Light;
// uint8_t Transmit_Buff[58] = {0};

// void Light_init(void) {
//   strcpy(Light.VIN, "OPTIMUSPRIMERYG01");
//   // strcpy(Light.VIN, "GCYLGSWC5GBL8A006");
//   Light.Light_Red_time = 40;
//   Light.Light_Yellow_time = 3;
//   Light.Light_Green_time = 20;
//   Light.Light_status = Light_Status_Red_ON;
//   Light.Light_time = Light.Light_Red_time;
//   HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);
//   HAL_GPIO_WritePin(LED_Y_GPIO_Port, LED_Y_Pin, GPIO_PIN_RESET);
//   HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
//   HAL_TIM_Base_Start_IT(&htim6);
// }

// void Light_Data_Transmit(void) {
//   uint8_t point = 0;

//   Transmit_Buff[point++] = 0xA5; // Header,2
//   Transmit_Buff[point++] = 0x5A;
//   Transmit_Buff[point++] = 0x00; // Version,1
//   Transmit_Buff[point++] = 0xFF; // Reserved,2
//   Transmit_Buff[point++] = 0xFF;
//   Transmit_Buff[point++] = 0xFF; // Identify,2
//   Transmit_Buff[point++] = 0x03;
//   Transmit_Buff[point++] = 0x03; // Format,1
//   Transmit_Buff[point++] = 0xFF; // Timestamp,8
//   Transmit_Buff[point++] = 0xFF;
//   Transmit_Buff[point++] = 0xFF;
//   Transmit_Buff[point++] = 0xFF;
//   Transmit_Buff[point++] = 0xFF;
//   Transmit_Buff[point++] = 0xFF;
//   Transmit_Buff[point++] = 0xFF;
//   Transmit_Buff[point++] = 0xFF;
//   Transmit_Buff[point++] = 0xFF; // Command,4
//   Transmit_Buff[point++] = 0xFF;
//   Transmit_Buff[point++] = 0xFF;
//   Transmit_Buff[point++] = 0xFF;
//   Transmit_Buff[point++] = 19; // Length,4
//   Transmit_Buff[point++] = 0x00;
//   Transmit_Buff[point++] = 0x00;
//   Transmit_Buff[point++] = 0x00;

//   Transmit_Buff[point++] = 'c';
//   Transmit_Buff[point++] = 'o';
//   Transmit_Buff[point++] = 'm';
//   Transmit_Buff[point++] = 'm';
//   Transmit_Buff[point++] = 'a';
//   Transmit_Buff[point++] = 'n';
//   Transmit_Buff[point++] = 'd';
//   Transmit_Buff[point++] = 't';
//   Transmit_Buff[point++] = 'y';
//   Transmit_Buff[point++] = 'p';
//   Transmit_Buff[point++] = 'e';
//   Transmit_Buff[point++] = 0x03;
//   Transmit_Buff[point++] = 'v';
//   Transmit_Buff[point++] = 'i';
//   Transmit_Buff[point++] = 'n';
//   for (uint8_t i = 0; i < (sizeof(Light.VIN) / sizeof(Light.VIN[0])); i++) {
//     // point += sprintf(Transmit_Buff + point, "%s", Light.VIN[i]);
//     Transmit_Buff[point++] = Light.VIN[i];
//   }

//   // Transmit_Buff[point] = 0XFF;
//   Transmit_Buff[point++] = Light.Light_status; // sprintf补充/0，覆盖了
//   Transmit_Buff[point] = Light.Light_time;     //最后一个包

//   //  HAL_UART_Transmit(&huart1, Transmit_Buff,
//   //                    (sizeof(Transmit_Buff) / sizeof(Transmit_Buff[0])),
//   //                    1000);
//   // atk_8266_station_send(Transmit_Buff, (sizeof(Transmit_Buff) /
//   //                                       sizeof(Transmit_Buff[0]))); //
//   wifi

//   Transmit_Buff[20] = 18; // Length
//                           // dtu_check_linkStatus();
//                           // send_data_to_dtu(Transmit_Buff,
//   //                  (sizeof(Transmit_Buff) / sizeof(Transmit_Buff[0]))); //
//   //                  DTU
//   //  HAL_UART_Transmit(&huart5, Transmit_Buff,
//   //                     (sizeof(Transmit_Buff) / sizeof(Transmit_Buff[0])),
//   //                     1000);
// //  HAL_UART_Transmit(&huart3, Transmit_Buff,
// //                    (sizeof(Transmit_Buff) / sizeof(Transmit_Buff[0])),
// 1000);
//      send_data_to_dtu(Transmit_Buff,
//                         (sizeof(Transmit_Buff) / sizeof(Transmit_Buff[0])));
//                         //DTU
//   HAL_UART_Transmit(&huart1, Transmit_Buff,
//                     (sizeof(Transmit_Buff) / sizeof(Transmit_Buff[0])),
//                     1000);
// }
