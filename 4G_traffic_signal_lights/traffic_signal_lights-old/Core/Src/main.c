/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ESP8266.h"
#include "atk_m750.h"
#include "light.h"
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

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t UART5_Rx_Cnt = 0;
uint8_t UART5_RxBuff[MAX_REC_LENGTH] = {0}; // UART5 message frames buff
uint8_t UART5_Rx_Flag = 0;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM6_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM5_Init();
  /* USER CODE BEGIN 2 */
  Light_init();
  HAL_TIM_Base_Start_IT(&htim5);
  // atk_8266_init();

  //  /*
  //   *  @arg        DTU_WORKMODE_NET,       0,  缂冩垹绮堕柅蹇庣炊濡€崇础
  //   *  @arg        DTU_WORKMODE_HTTP,      1,  http闁繋绱跺Ο鈥崇础
  //   *  @arg        DTU_WORKMODE_MQTT,      2,  mqtt闁繋绱跺Ο鈥崇础
  //   *  @arg        DTU_WORKMODE_ALIYUN,    3,
  //   闂冨潡鍣锋禍鎴�????閿熸垝绱跺Ο鈥崇础
  //   *  @arg        DTU_WORKMODE_ONENET,    4,  OneNET闁繋绱跺Ο鈥崇础
  //   *  @arg        DTU_WORKMODE_BAIDUYUN,  5,
  //   閻ф儳瀹虫禍鎴嫹?閿熸垝绱跺Ο鈥崇础
  //   */
  //
  //  while (dtu_config_init(DTU_WORKMODE_NET) != 0)
  //  /*閸掓繂顫愰崠鏈孴U閻ㄥ嫬浼愭担婊冨棘閿燂拷?*/
  //  {
  //    //printf("ATK-DTU Init Fail ...\r\n");
  //    HAL_Delay(2000);
  //  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    Light_Data_Transmit();
    HAL_Delay(300);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
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
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enables the Clock Security System
  */
  HAL_RCC_EnableCSS();
}

/* USER CODE BEGIN 4 */
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM7 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */
  if (htim->Instance == TIM5) {
    if (Light.Light_status == Light_Status_Red_Flashing) {
      HAL_GPIO_TogglePin(LED_R_GPIO_Port, LED_R_Pin);

      /* Side LAMP */
      HAL_GPIO_TogglePin(LED_Y_Side_GPIO_Port, LED_Y_Side_Pin);
      HAL_GPIO_WritePin(LED_R_Side_GPIO_Port, LED_R_Side_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(LED_G_Side_GPIO_Port, LED_G_Side_Pin, GPIO_PIN_RESET);
    } else if (Light.Light_status == Light_Status_Yellow_Flashing) {
      HAL_GPIO_TogglePin(LED_Y_GPIO_Port, LED_Y_Pin);

      /* Side LAMP */
      HAL_GPIO_TogglePin(LED_R_Side_GPIO_Port, LED_R_Side_Pin);
    } else if (Light.Light_status == Light_Status_Green_Flashing) {
      HAL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin);
    }
    if (((Light.Light_time > Light.Light_Yellow_time) &&
         (Light.Light_time < Light.Light_Yellow_time + 4)) &&
        (Light.Light_status == Light_Status_Red_ON)) {

      /* Side LAMP */
      HAL_GPIO_TogglePin(LED_G_Side_GPIO_Port, LED_G_Side_Pin);
      HAL_GPIO_WritePin(LED_R_Side_GPIO_Port, LED_R_Side_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(LED_Y_Side_GPIO_Port, LED_Y_Side_Pin, GPIO_PIN_RESET);
    }
  }
  if (htim->Instance == TIM6) {
    Light.Light_time--;

    if (Light.Light_time > 0 &&
        Light.Light_time < (Light.Light_Yellow_time + 1)) {
      if (Light.Light_status == Light_Status_Red_ON) {
        Light.Light_status = Light_Status_Red_Flashing;
      } else if (Light.Light_status == Light_Status_Green_ON) {
        Light.Light_status = Light_Status_Green_Flashing;
      }
    }
    if (Light.Light_time == 0) {
      if (Light.Light_status == Light_Status_Red_Flashing) {
        Light.Light_status = Light_Status_Green_ON;
        Light.Light_time = Light.Light_Green_time;
        HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_Y_GPIO_Port, LED_Y_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_SET);

        /* Side LAMP */
        HAL_GPIO_WritePin(LED_R_Side_GPIO_Port, LED_R_Side_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_Y_Side_GPIO_Port, LED_Y_Side_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_G_Side_GPIO_Port, LED_G_Side_Pin, GPIO_PIN_RESET);
      } else if (Light.Light_status == Light_Status_Green_Flashing) {
        Light.Light_status = Light_Status_Yellow_Flashing;
        Light.Light_time = Light.Light_Yellow_time;
        HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_Y_GPIO_Port, LED_Y_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);

        /* Side LAMP */
        HAL_GPIO_WritePin(LED_R_Side_GPIO_Port, LED_R_Side_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_Y_Side_GPIO_Port, LED_Y_Side_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_G_Side_GPIO_Port, LED_G_Side_Pin, GPIO_PIN_RESET);
      } else if (Light.Light_status == Light_Status_Yellow_Flashing) {
        Light.Light_status = Light_Status_Red_ON;
        Light.Light_time = Light.Light_Red_time;
        HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_Y_GPIO_Port, LED_Y_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);

        /* Side LAMP */
        HAL_GPIO_WritePin(LED_R_Side_GPIO_Port, LED_R_Side_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_Y_Side_GPIO_Port, LED_Y_Side_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_G_Side_GPIO_Port, LED_G_Side_Pin, GPIO_PIN_SET);
      }
    }
  }

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM7) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart->Instance == UART5) {
    if (0xA5 == Transmit_Buff[0]) {
      UART5_Rx_Cnt = 0;
    }
    UART5_RxBuff[UART5_Rx_Cnt] = Transmit_Buff[0];
    UART5_Rx_Cnt++;

    if (UART5_Rx_Cnt == 40) {
      UART5_Rx_Cnt = 0;
      UART5_Rx_Flag = 1;
    }
    // if (UART5_Rx_Flag == 1) {
    // }
    if (UART5_Rx_Cnt >= MAX_REC_LENGTH) {
      UART5_Rx_Cnt = 0;
      UART5_Rx_Flag = 0;
      memset(UART5_RxBuff, 0x00, sizeof(UART5_RxBuff));
    }
    // RingBuffer_In(p_uart3_rxbuf, &uart_temp, 1);
    HAL_UART_Receive_IT(&huart5, &uart_temp, 1);
  }

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line
     number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line)
   */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
