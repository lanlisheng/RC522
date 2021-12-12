/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
#include "spi.h"
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "rc522.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
// void ReaderCard(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
  /* USER CODE BEGIN 1 */
  // uint8_t th, dh;
  // uint8_t eeprom;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick.
   */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  printf("The USART Is Ok!!!\r\n");

  /*RC522 initialization*/
  RC522_PcdReset();
  /*Close the antenna*/
  RC522_PcdAntennaOff();
  /*Open the antenna*/
  RC522_PcdAntennaOn();

  printf("RFID-MFRC522 TEST\r\nFindCard Starting ...\r\n"); /*Test pin
                                                               initialization*/
  /* USER CODE END 2 */

  /* USER CODE BEGIN WHILE */
  while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    uint8_t Temp[4] = {0};
    if (RC522_PcdRequest(0x52, Temp) == MI_OK) {
      if (Temp[0] == 0x04 && Temp[1] == 0x00)
        printf("MFOne-S50");
      else if (Temp[0] == 0x02 && Temp[1] == 0x00)
        printf("MFOne-S70");
      else if (Temp[0] == 0x44 && Temp[1] == 0x00)
        printf("MF-UltraLight");
      else if (Temp[0] == 0x08 && Temp[1] == 0x00)
        printf("MF-Pro");
      else if (Temp[0] == 0x44 && Temp[1] == 0x03)
        printf("MF Desire");
      else
        printf("Unknown");
      printf("SUCCESS!");
    } else {
      printf("Failed!");
    }

    HAL_Delay(100);
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/**
 * @brief  IC测试函数
 * @param  �????
 * @retval �????
 */
void ReaderCard(void) {
  // uint8_t temp_value;
  while (1) {
    /*寻卡*/
    // if ((RC522_PcdRequest(PICC_REQALL, Temp)) == MI_OK) {
    //   printf("寻卡成功\n");
    //   /*若失败再次寻�????*/

    //   ucStatusReturn = RC522_PcdRequest(PICC_REQALL, ucArray_ID); //
    //   PICC_REQALL PICC_REQIDL
    // } else {
    //   printf("寻卡失败\n");
    // }

    // if (ucStatusReturn == MI_OK) {
    //   // printf ( "寻卡成功\n" );
    //   /*防冲撞（当有多张卡进入读写器操作范围时，防冲突机制会从其中�?�择�????张进行操作）*/
    //   if (RC522_PcdAnticoll(ucArray_ID) == MI_OK) {
    //     RC522_PcdSelect(ucArray_ID);

    //     RC522_PcdAuthState(PICC_AUTHENT1A, 0x11, KeyValue,
    //                        ucArray_ID); //校验密码
    //     // WriteAmount(0x11, writeValue); //写入金额
    //     // if (ReadAmount(0x11, &readValue) == MI_OK)
    //     //读取金额
    //     // {
    //     //   // writeValue +=100;
    //     //   sprintf(cStr, "The Card ID is: %02X%02X%02X%02X", ucArray_ID[0],
    //     //           ucArray_ID[1], ucArray_ID[2], ucArray_ID[3]);
    //     //   printf("%s\r\n", cStr); //打印卡片ID
    //     //   //ILI9341_DispStringLine_EN(LINE(0), cStr);

    //     //   printf("余额为：%d\r\n", readValue);
    //     //   sprintf(cStr, "TThe residual amount: %d", readValue);
    //     // ILI9341_DispStringLine_EN(LINE(1), cStr);
    //     RC522_PcdHalt();
    //     // }
    //   }
    // }
  }
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state
   */
  __disable_irq();
  while (1) {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line) {
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line
     number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line)
   */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
