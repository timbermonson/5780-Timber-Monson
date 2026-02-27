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

void RCC_TIM2_CLK_Enable(void)
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
}

void RCC_TIM3_CLK_Enable(void)
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
}

void TIMx_SetPrescaleArr(TIM_TypeDef *TIMx, uint16_t prescale, uint16_t arr)
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

// I2C FUNCS

void I2C_StartTransaction(I2C_TypeDef *i2cPeriph,
                          uint8_t devAddr,
                          uint8_t numBytes,
                          uint8_t isWrite)
{
  // Set Device Address
  i2cPeriph->CR2 &= ~(I2C_CR2_SADD_Msk);
  i2cPeriph->CR2 |= (devAddr << 1);

  // Set numBytes
  i2cPeriph->CR2 &= ~(I2C_CR2_NBYTES_Msk);
  i2cPeriph->CR2 |= (numBytes << I2C_CR2_NBYTES_Pos);

  // Configure for write (writing register address)
  i2cPeriph->CR2 &= ~(I2C_CR2_RD_WRN_Msk);
  i2cPeriph->CR2 |= isWrite ? (1 << I2C_CR2_RD_WRN_Pos) : (0 << I2C_CR2_RD_WRN_Pos);

  // Start transaction
  i2cPeriph->CR2 |= I2C_CR2_START;
}

int I2C_WaitTXReady(I2C_TypeDef *i2cPeriph)
{
  while (!(i2cPeriph->ISR & I2C_ISR_TXIS_Msk))
  {
    if (i2cPeriph->ISR & I2C_ISR_NACKF_Msk)
    {
      return -1;
    }
  }

  return 0;
}

int I2C_WaitTComplete(I2C_TypeDef *i2cPeriph)
{
  while (!(i2cPeriph->ISR & I2C_ISR_TC_Msk))
  {
    if (i2cPeriph->ISR & I2C_ISR_NACKF_Msk)
    {
      return -1;
    }
  }

  return 0;
}

int I2C_WaitRXReady(I2C_TypeDef *i2cPeriph)
{
  while (!(i2cPeriph->ISR & I2C_ISR_RXNE_Msk))
  {
    if (i2cPeriph->ISR & I2C_ISR_NACKF_Msk)
    {
      return -1;
    }
  }

  return 0;
}

void I2C_Read(
    I2C_TypeDef *i2cPeriph, uint8_t devAddr, uint8_t regAddr, uint8_t numBytes, uint8_t *buf)
{
  // Start a transaction to write the register address
  I2C_StartTransaction(i2cPeriph, devAddr, 1, 1);

  // Send the address of the address to be read
  I2C_WaitTXReady(i2cPeriph);
  i2cPeriph->TXDR |= regAddr;
  I2C_WaitTComplete(i2cPeriph);

  // Start a transaction to read the register value
  I2C_StartTransaction(i2cPeriph, devAddr, numBytes, 0);

  // Read the register contents in the buffer
  for (int i = 0; i < numBytes; i++)
  {
    I2C_WaitRXReady(i2cPeriph);
    buf[i] = i2cPeriph->RXDR;
  }
  I2C_WaitTComplete(I2C2);

  // Release the bus
  i2cPeriph->CR2 |= I2C_CR2_STOP;
}

void I2C_Write(
    I2C_TypeDef *i2cPeriph, uint8_t devAddr, uint8_t regAddr, uint8_t numBytes, uint8_t *buf)
{
  // Start a transaction to write the register address
  I2C_StartTransaction(i2cPeriph, devAddr, numBytes + 1, 1);

  // Send the address of the address to be written
  I2C_WaitTXReady(i2cPeriph);
  i2cPeriph->TXDR |= regAddr;

  // Write out the buffer contents
  for (int i = 0; i < numBytes; i++)
  {
    I2C_WaitTXReady(i2cPeriph);
    i2cPeriph->TXDR |= buf[i];
  }
  I2C_WaitTComplete(I2C2);

  // Release the bus
  i2cPeriph->CR2 |= I2C_CR2_STOP;
}