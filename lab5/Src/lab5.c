#include "Legacy/stm32_hal_legacy.h"
#include "main.h"
#include "stm32f072xb.h"
#include "stm32f0xx_hal.h"
#include "otherDefs.h"
#include "stm32f0xx_hal_gpio.h"
#include "stm32f0xx_hal_gpio_ex.h"

void SystemClock_Config(void);

void Accel_Setup_I2CPins()
{
  // Enable necessary GPIO clocks
  RCC_GPIOB_CLK_Enable();
  RCC_GPIOC_CLK_Enable();

  // Set up I2C-select and address-select pins
  GPIO_InitTypeDef addrPin;
  addrPin.Pin = GPIO_PIN_14;
  addrPin.Mode = GPIO_MODE_OUTPUT_PP;
  addrPin.Pull = GPIO_NOPULL;
  addrPin.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &addrPin);

  GPIO_InitTypeDef selPin;
  selPin.Pin = GPIO_PIN_0;
  selPin.Mode = GPIO_MODE_OUTPUT_PP;
  selPin.Pull = GPIO_NOPULL;
  selPin.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &selPin);

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);

  // Set up SDA pin (PB11)
  GPIO_InitTypeDef sdaConfig;
  sdaConfig.Pin = GPIO_PIN_11;
  sdaConfig.Mode = GPIO_MODE_AF_OD;
  sdaConfig.Pull = GPIO_NOPULL;
  sdaConfig.Speed = GPIO_SPEED_FREQ_LOW;
  sdaConfig.Alternate = GPIO_AF1_I2C2;
  HAL_GPIO_Init(GPIOB, &sdaConfig);

  // Set up SCL pin (PB13)
  GPIO_InitTypeDef sclConfig;
  sclConfig.Pin = GPIO_PIN_13;
  sclConfig.Mode = GPIO_MODE_AF_OD;
  sclConfig.Pull = GPIO_NOPULL;
  sclConfig.Speed = GPIO_SPEED_FREQ_LOW;
  sclConfig.Alternate = GPIO_AF5_I2C2;
  HAL_GPIO_Init(GPIOB, &sclConfig);
}

void Accel_Setup_I2C2Init()
{
  RCC_I2C2_CLK_Enable();

  // Set up timings
  I2C2->TIMINGR |= (0x1 << I2C_TIMINGR_PRESC_Pos);
  I2C2->TIMINGR |= (0x13 << I2C_TIMINGR_SCLL_Pos);
  I2C2->TIMINGR |= (0x0F << I2C_TIMINGR_SCLH_Pos);
  I2C2->TIMINGR |= (0x2 << I2C_TIMINGR_SDADEL_Pos);
  I2C2->TIMINGR |= (0x4 << I2C_TIMINGR_SCLDEL_Pos);

  // Enable Peripheral
  I2C2->CR1 |= (0b1 << I2C_CR1_PE_Pos);
}

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();

  static const uint8_t accelI2CAddr = 0x69;
  static const uint8_t accelWhoAmIAddr = 0x0f;

  USART1_Setup();
  UARTx_TXString(USART1, "Hello World!\r\n");
  Accel_Setup_I2CPins();
  Accel_Setup_I2C2Init();

  uint8_t readBuf[8];
  I2Cx_Read(I2C2, accelI2CAddr, accelWhoAmIAddr, 1, readBuf);
  UARTx_TXBytes(USART1, readBuf, 1);

  while (1)
  {
    // UARTx_TXString(USART1, ".\r\n");
    HAL_Delay(1000);
  }
  return -1;
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* User can add their own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add their own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
#endif /* USE_FULL_ASSERT */
