#include "main.h"
#include "stm32f0xx_hal.h"
#include "otherDefs.h"
#include "hal_gpio.h"

void SystemClock_Config(void);

void TIM2_IRQHandler(void)
{
  My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
  My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9);
  TIM2->SR &= ~((uint16_t)1);
}

uint16_t prescaleFromArrHz(uint16_t arr, unsigned int desiredHz)
{
  static const int baseFreq = 8000000;
  return (baseFreq / (arr * desiredHz)) - 1;
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

  // Configure Timer clocks
  RCC_TIM2_CLK_Enable();
  RCC_TIM3_CLK_Enable();

  // Configure Timer 2
  TIMx_SetPrescaleArr(TIM2, prescaleFromArrHz(1000, 4), 1000);

  TIMx_EnableUpdateInterr(TIM2);
  TIMx_Enable(TIM2);

  // Configure Timer 3
  TIMx_SetPrescaleArr(TIM3, prescaleFromArrHz(1000, 800), 1000);

  TIM3->CCMR1 &= (uint16_t)~(0b11 | 0b11 << 8); // Set capture/compare 1/2 to output

  TIM3->CCMR1 &= (uint16_t)~(0b111 << 4 | 0b111 << 12); // Clear and set channel 1/2 PWM mode bits
  TIM3->CCMR1 |= (uint16_t)(0b111 << 4 | 0b110 << 12);

  TIM3->CCMR1 |= (uint16_t)(0b1 << 3 | 0b1 << 11); // Enable timer channel 1/2 preloads

  TIM3->CCER |= (uint16_t)(0b1 << 0 | 0b1 << 4); // Enable channel 1/2 outputs

  TIM3->CCR1 = (uint32_t)(200); // Set both channels to 20%
  TIM3->CCR2 = (uint32_t)(200);
  TIMx_Enable(TIM3);

  // Configure GPIO
  RCC_GPIOC_CLK_Enable();
  My_HAL_GPIO_InitLEDs();

  GPIOC->MODER &= (uint32_t)~(0b11 << 12 | 0b11 << 14);
  GPIOC->MODER |= (uint32_t)(0b10 << 12 | 0b10 << 14);

  GPIOC->AFR[0] &= ~(uint32_t)(0b1111 << 24 | 0b1111 << 28);

  My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
  My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);

  // Setup Interrupt
  NVIC_EnableIRQ(15);

  while (1)
  {
    HAL_Delay(500);
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
