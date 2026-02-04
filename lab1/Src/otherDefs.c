#include "main.h"
#include "stm32f0xx_hal.h"
#include "otherDefs.h"

void HAL_RCC_GPIOC_CLK_Enable(void)
{
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
}
void HAL_RCC_GPIOA_CLK_Enable(void)
{
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
}