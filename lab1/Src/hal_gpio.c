#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>

void My_HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
  // Set pins 6 thru 9 to "General Output"
  for (int i = 8; i <= 9; i++)
  {
    GPIOx->MODER |= (0b01 << 2 * i);

    GPIOx->OTYPER &= ~(0b1 << i);
    GPIOx->OSPEEDR &= ~(0b11 << 2 * i);
    GPIOx->PUPDR &= ~(0b11 << 2 * i);
  }

  GPIOA->MODER &= ~(0b11 << 0);
  GPIOA->OSPEEDR &= ~(0b11 << 0);
  GPIOA->PUPDR &= ~(0b10 << 0);
}

/*
void My_HAL_GPIO_DeInit(GPIO_TypeDef  *GPIOx, uint32_t GPIO_Pin)
{
}
*/

/*
GPIO_PinState My_HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    return -1;
}
*/

void My_HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
  // Use BSRR to set state atomically
  if (PinState == GPIO_PIN_SET)
  {
    GPIOx->BSRR = GPIO_Pin;
  }
  else
  {
    // "Resetting" in the BSRR means writing "1" to a 16-offset position.
    GPIOx->BSRR = (GPIO_Pin << 16);
  }
}

void My_HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  // Get current state
  GPIO_PinState state = (GPIOx->ODR >> GPIO_Pin) & 1;

  // Set to opposite state
  if (state == GPIO_PIN_SET)
  {
    My_HAL_GPIO_WritePin(GPIOx, GPIO_Pin, 0);
  }
  else
  {
    My_HAL_GPIO_WritePin(GPIOx, GPIO_Pin, 1);
  }
}
