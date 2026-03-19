#include "main.h"
#include "stm32f0xx_hal.h"
#include "otherDefs.h"

void RCC_ADC_CLK_Enable(void)
{
  RCC->APB2ENR |= RCC_APB2ENR_ADCEN;
}

void RCC_GPIOC_CLK_Enable(void)
{
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
}

void RCC_GPIOB_CLK_Enable(void)
{
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
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

void RCC_I2C2_CLK_Enable(void)
{
  RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
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
void I2Cx_StartTransaction(I2C_TypeDef *i2cPeriph,
                           uint8_t devAddr,
                           uint8_t numBytes,
                           uint8_t isWrite)
{
  i2cPeriph->CR2 &= ~(I2C_CR2_SADD_Msk | I2C_CR2_NBYTES_Msk | I2C_CR2_RD_WRN_Msk);

  i2cPeriph->CR2 |= (numBytes << I2C_CR2_NBYTES_Pos);

  i2cPeriph->CR2 &= ~(I2C_CR2_RD_WRN_Msk);

  i2cPeriph->CR2 |= ((uint32_t)devAddr << 1);

  // Configure for write (writing register address)
  if (!isWrite)
  {
    i2cPeriph->CR2 |= (1 << I2C_CR2_RD_WRN_Pos);
  }

  // Start transaction
  i2cPeriph->CR2 |= I2C_CR2_START;
}

int I2Cx_WaitTXReady(I2C_TypeDef *i2cPeriph)
{
  while (!(i2cPeriph->ISR & I2C_ISR_TXIS_Msk))
  {
    if (i2cPeriph->ISR & I2C_ISR_NACKF_Msk)
    {
      return 0;
    }
  }

  return 1;
}

int I2Cx_WaitTComplete(I2C_TypeDef *i2cPeriph)
{
  while (!(i2cPeriph->ISR & I2C_ISR_TC_Msk))
  {
    if (i2cPeriph->ISR & I2C_ISR_NACKF_Msk)
    {
      return 0;
    }
  }

  return 1;
}

int I2Cx_WaitRXReady(I2C_TypeDef *i2cPeriph)
{
  while (!(i2cPeriph->ISR & I2C_ISR_RXNE_Msk))
  {
    if (i2cPeriph->ISR & I2C_ISR_NACKF_Msk)
    {
      return 0;
    }
  }

  return 1;
}

int I2Cx_Read(
    I2C_TypeDef *i2cPeriph, uint8_t devAddr, uint8_t regAddr, uint8_t numBytes, uint8_t *buf)
{
  I2Cx_StartTransaction(i2cPeriph, devAddr, 1, 1);

  if (!I2Cx_WaitTXReady(i2cPeriph))
  {
    i2cPeriph->CR2 |= I2C_CR2_STOP;
    return -1;
  }
  i2cPeriph->TXDR = regAddr;
  if (!I2Cx_WaitTComplete(I2C2))
  {
    i2cPeriph->CR2 |= I2C_CR2_STOP;
    return -1;
  }

  // Start a transaction to read the register value
  I2Cx_StartTransaction(i2cPeriph, devAddr, numBytes, 0);

  // Read the register contents in the buffer
  int bytesRead = 0;
  for (int i = 0; i < numBytes; i++)
  {
    if (!I2Cx_WaitRXReady(i2cPeriph))
    {
      break;
    };
    buf[i] = i2cPeriph->RXDR;
    bytesRead++;
  }
  I2Cx_WaitTComplete(I2C2);

  // Release the bus
  i2cPeriph->CR2 |= I2C_CR2_STOP;
  return bytesRead;
}

void I2Cx_Write(
    I2C_TypeDef *i2cPeriph, uint8_t devAddr, uint8_t regAddr, uint8_t numBytes, uint8_t *buf)
{
  // Start a transaction to write the register address
  I2Cx_StartTransaction(i2cPeriph, devAddr, numBytes + 1, 1);

  // Send the address of the address to be written
  I2Cx_WaitTXReady(i2cPeriph);
  i2cPeriph->TXDR = regAddr;

  // Write out the buffer contents
  for (int i = 0; i < numBytes; i++)
  {
    I2Cx_WaitTXReady(i2cPeriph);
    i2cPeriph->TXDR = buf[i];
  }
  I2Cx_WaitTComplete(I2C2);

  // Release the bus
  i2cPeriph->CR2 |= I2C_CR2_STOP;
}

void USART1_Setup()
{
  // Enable UART1 in RCC
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

  // Enable GPIO in RCC
  RCC_GPIOA_CLK_Enable();

  // PA9: TX, PA10: RX (Configure alt mode)
  GPIOA->MODER &= ~(0b11 << GPIO_MODER_MODER9_Pos | 0b11 << GPIO_MODER_MODER10_Pos);
  GPIOA->MODER |= (0b10 << GPIO_MODER_MODER9_Pos | 0b10 << GPIO_MODER_MODER10_Pos);

  GPIOA->AFR[1] &= ~(0b1111 << GPIO_AFRH_AFSEL9_Pos | 0b1111 << GPIO_AFRH_AFSEL10_Pos);
  GPIOA->AFR[1] |= (0b0001 << GPIO_AFRH_AFSEL9_Pos | 0b0001 << GPIO_AFRH_AFSEL10_Pos);

  // // Desired baud: 115200
  USART1->BRR &= (uint32_t)(0xffff0000);
  USART1->BRR |= (uint16_t)(HAL_RCC_GetHCLKFreq() / 115200);

  // Enable Transmit/Receive, and Receive interrupt.
  USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;

  USART1->CR1 |= USART_CR1_RXNEIE;
  NVIC_EnableIRQ(27);
  NVIC_SetPriority(27, 1);

  // Enable UART peripheral.
  USART1->CR1 |= USART_CR1_UE;
}

void UARTx_TXChar(USART_TypeDef *UartPeriph, char c)
{
  while ((UartPeriph->ISR & USART_ISR_TXE) == 0)
  {
  }

  UartPeriph->TDR = c;
}

void UARTx_TXString(USART_TypeDef *UartPeriph, char *str)
{
  for (char *c = str; *c != '\0'; c++)
  {
    UARTx_TXChar(UartPeriph, *c);
  }
}

// Debugging functions for examining the contents of a register over UART.
char halfbyteToHexChar(uint8_t byte)
{
  if (byte < 10)
    return '0' + byte;
  else if (byte < 16)
    return 'A' + (byte - 10);
  else
    return '?';
}

void UARTx_TXBytes(USART_TypeDef *UartPeriph, uint8_t *data, unsigned int numBytes)
{
  UARTx_TXString(UartPeriph, "0x");
  for (int i = 0; i < numBytes; i++)
  {
    UARTx_TXChar(UartPeriph, halfbyteToHexChar((data[i] >> 4) & 0b1111));
    UARTx_TXChar(UartPeriph, halfbyteToHexChar(data[i] & 0b1111));
  }
  UARTx_TXString(UartPeriph, "\r\n");
}

void UARTx_TXReg(USART_TypeDef *UartPeriph, uint32_t reg)
{
  UARTx_TXString(UartPeriph, "\r\nReg Contents: 0x");
  for (int i = 7; i >= 0; i--)
  {
    UARTx_TXChar(UartPeriph, halfbyteToHexChar((reg >> (i * 4)) & 0b1111));
  }
  UARTx_TXString(UartPeriph, "\r\n");
}