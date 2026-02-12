#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>

void RCC_GPIOC_CLK_Enable(void);
void RCC_GPIOA_CLK_Enable(void);
void RCC_SYSCFG_CLK_Enable(void);

void RCC_TIM2_CLK_Enable(void);
void TIM2_SetPeriodVars(uint16_t prescale, uint16_t arr);
void TIM2_EnableUpdateInterr(void);
void TIM2_Enable(void);