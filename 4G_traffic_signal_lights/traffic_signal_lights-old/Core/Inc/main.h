/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Zigbee_TX_Pin GPIO_PIN_2
#define Zigbee_TX_GPIO_Port GPIOA
#define Zigbee_RX_Pin GPIO_PIN_3
#define Zigbee_RX_GPIO_Port GPIOA
#define ESP8266_TX_Pin GPIO_PIN_10
#define ESP8266_TX_GPIO_Port GPIOB
#define ESP8266_RX_Pin GPIO_PIN_11
#define ESP8266_RX_GPIO_Port GPIOB
#define LED_R_Pin GPIO_PIN_12
#define LED_R_GPIO_Port GPIOB
#define LED_Y_Pin GPIO_PIN_13
#define LED_Y_GPIO_Port GPIOB
#define LED_G_Pin GPIO_PIN_14
#define LED_G_GPIO_Port GPIOB
#define LED_R_Side_Pin GPIO_PIN_6
#define LED_R_Side_GPIO_Port GPIOC
#define LED_G_Side_Pin GPIO_PIN_7
#define LED_G_Side_GPIO_Port GPIOC
#define LED_Y_Side_Pin GPIO_PIN_8
#define LED_Y_Side_GPIO_Port GPIOC
#define RS485_TX_Pin GPIO_PIN_10
#define RS485_TX_GPIO_Port GPIOC
#define RS485_RX_Pin GPIO_PIN_11
#define RS485_RX_GPIO_Port GPIOC
#define DTU4G_TX_Pin GPIO_PIN_12
#define DTU4G_TX_GPIO_Port GPIOC
#define DTU4G_RX_Pin GPIO_PIN_2
#define DTU4G_RX_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */
#define MAX_REC_LENGTH 1024
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
