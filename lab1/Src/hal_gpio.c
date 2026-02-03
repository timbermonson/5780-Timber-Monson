#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>

void My_HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
  int startPin = 6;
  int endPin = 9;
  uint32_t MODER_SET = 0;
  uint32_t OTYPER_SET = 0;
  uint32_t OSPEEDR_SET = 0;
  uint32_t PUPDR_SET = 0;

  // Set pins 6 thru 9 to "General Output"
  for (int i = startPin; i <= endPin; i++)
  {
    MODER_SET = (0b01 << 2 * i);

    // OTYPER_SET = // Unneeded, only need to set 0, which is the reset state
    // OSPEEDR_SET = // Same as above
    // PUPDR_SET_SET = // Same as above
  }

  GPIOx->MODER = MODER_SET;
  GPIOx->OTYPER = OTYPER_SET;
  GPIOx->OSPEEDR = OSPEEDR_SET;
  GPIOx->PUPDR = PUPDR_SET;

  GPIOA->MODER &= !(0b00 << 0);
  GPIOA->OSPEEDR &= !(0b00 << 0);
  GPIOA->PUPDR &= !(0b10 << 0);
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

/*
void My_HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
}
*/

/*
void My_HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
}
*/
