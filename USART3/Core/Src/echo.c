#include "echo.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_it.h"

//启动超声波模块
void StartModule(void) // T1中断用来扫描数码管和计800MS启动模块
{
  HAL_GPIO_WritePin(HC_SR04_Trig_GPIO_Port, HC_SR04_Trig_Pin, 1);
  HAL_Delay(10); // 10ms延时
  HAL_GPIO_WritePin(HC_SR04_Trig_GPIO_Port, HC_SR04_Trig_Pin, 0);
}

//获得ECHO引脚状态
int GetStatus(void) {
  return HAL_GPIO_ReadPin(HC_SR04_Echo_GPIO_Port, HC_SR04_Echo_Pin);
}
