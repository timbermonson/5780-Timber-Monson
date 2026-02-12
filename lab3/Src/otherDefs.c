#include "main.h"
#include "stm32f0xx_hal.h"
#include "otherDefs.h"

void RCC_GPIOC_CLK_Enable(void)
{
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
}

void RCC_GPIOA_CLK_Enable(void)
{
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
}

void RCC_SYSCFG_CLK_Enable(void)
{
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;
}
