#include "main.h"
#include "stm32f0xx_hal.h"

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

void RCC_TIM2_CLK_Enable(void)
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
}

void TIM2_SetPeriodVars(uint16_t prescale, uint16_t arr)
{
  TIM2->PSC = prescale;
  TIM2->ARR = arr;
}

void TIM2_EnableUpdateInterr(void)
{
  TIM2->DIER |= TIM_DIER_UIE;
}

void TIM2_Enable(void)
{
  TIM2->CR1 |= TIM_CR1_CEN;
}