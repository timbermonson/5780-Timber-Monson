#include "stm32f072xb.h"
#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>

void RCC_GPIOA_CLK_Enable(void);
void RCC_GPIOB_CLK_Enable(void);

void RCC_GPIOC_CLK_Enable(void);
void RCC_SYSCFG_CLK_Enable(void);

void RCC_TIM2_CLK_Enable(void);
void RCC_TIM3_CLK_Enable(void);

void RCC_I2C2_CLK_Enable(void);

void RCC_ADC_CLK_Enable(void);
void RCC_DAC_CLK_Enable(void);

void TIMx_SetPrescaleArr(TIM_TypeDef *TIMx, uint16_t prescale, uint16_t arr);
void TIMx_EnableUpdateInterr(TIM_TypeDef *TIMx);
void TIMx_Enable(TIM_TypeDef *TIMx);

void USART1_Setup();
void UARTx_TXBytes(USART_TypeDef *UartPeriph, uint8_t *data, unsigned int numBytes);
void UARTx_TXString(USART_TypeDef *UartPeriph, char *str);
void UARTx_TXReg(USART_TypeDef *UartPeriph, uint32_t reg);

int I2Cx_Read(
    I2C_TypeDef *i2cPeriph, uint8_t devAddr, uint8_t regAddr, uint8_t numBytes, uint8_t *buf);

void I2Cx_Write(
    I2C_TypeDef *i2cPeriph, uint8_t devAddr, uint8_t regAddr, uint8_t numBytes, uint8_t *buf);