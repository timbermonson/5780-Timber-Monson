#include "main.h"
#include "stm32f072xb.h"
#include "stm32f0xx_hal.h"
#include "otherDefs.h"

void SystemClock_Config(void);

void Accel_Setup_I2CPins()
{
  // Enable necessary GPIO clocks
  RCC_GPIOB_CLK_Enable();
  RCC_GPIOC_CLK_Enable();

  // Set up SDA pin (PB11)
  GPIOB->MODER &= ~(GPIO_MODER_MODER11_Msk);
  GPIOB->MODER |= (0b10 << GPIO_MODER_MODER11_Pos);

  GPIOB->OTYPER |= (0b1 << 11);

  GPIOB->AFR[1] &= ~(0b1111 << GPIO_AFRH_AFSEL11_Pos);
  GPIOB->AFR[1] |= (0b0001 << GPIO_AFRH_AFSEL11_Pos);

  // Set up SCL pin (PB13)
  GPIOB->MODER &= ~(GPIO_MODER_MODER13_Msk);
  GPIOB->MODER |= (0b10 << GPIO_MODER_MODER13_Pos);

  GPIOB->OTYPER |= (0b1 << 13);

  GPIOB->AFR[1] &= ~(0b1111 << GPIO_AFRH_AFSEL13_Pos);
  GPIOB->AFR[1] |= (0b0101 << GPIO_AFRH_AFSEL13_Pos);

  // Set up I2C-select and address-select pins
  GPIOB->MODER &= ~(GPIO_MODER_MODER14_Msk);
  GPIOB->MODER |= (0b01 << GPIO_MODER_MODER14_Pos);
  GPIOB->OTYPER &= ~(0b1 << 14);
  GPIOB->ODR |= (0b1 << 14);

  GPIOC->MODER &= ~(GPIO_MODER_MODER0_Msk);
  GPIOC->MODER |= (0b01 << GPIO_MODER_MODER0_Pos);
  GPIOC->OTYPER &= ~(0b1 << 0);
  GPIOC->ODR |= (0b1 << 0);
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

  // static const uint8_t i2cAddr = 0x6b;

  Accel_Setup_I2CPins();
  Accel_Setup_I2C2Init();

  while (1)
  {
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
