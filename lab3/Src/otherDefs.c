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

void TIMx_SetPeriodVars(TIM_TypeDef *TIMx, uint16_t prescale, uint16_t arr)
{
  TIMx->PSC = prescale;
  TIMx->ARR = arr;
}

void TIMx_EnableUpdateInterr(TIM_TypeDef *TIMx)
{
  TIMx->DIER |= TIM_DIER_UIE;
}

void TIMx_Enable(TIM_TypeDef *TIMx)
{
  TIMx->CR1 |= TIM_CR1_CEN;
}