#include "main.h"
#include "stm32f0xx_hal.h"
void HAL_RCC_GPIOC_CLK_Enable(void);
void My_HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init);