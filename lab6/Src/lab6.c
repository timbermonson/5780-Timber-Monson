#include "main.h"
#include "stm32f072xb.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_gpio.h"
#include "hal_gpio.h"
#include "otherDefs.h"

void SystemClock_Config(void);

void configADC()
{
  ADC1->CFGR1 = 0;
  ADC1->CFGR1 |= 0b10 << ADC_CFGR1_RES_Pos; // 8-Bit Resolution
  ADC1->CFGR1 |= ADC_CFGR1_CONT;            // Continuous Measurement

  ADC1->CHSELR |= ADC_CHSELR_CHSEL10; // Enable channel 10

  ADC1->CR |= ADC_CR_ADCAL;           // Start self-cal
  while (ADC1->CR & ADC_CR_ADCAL_Msk) // Wait for complete
  {
  }

  ADC1->CR |= ADC_CR_ADEN; // Enable and start the ADC.
  ADC1->CR |= ADC_CR_ADSTART;
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

  RCC_GPIOC_CLK_Enable();
  My_HAL_GPIO_InitLEDs();

  GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR0_Msk;
  GPIOC->MODER |= GPIO_MODER_MODER0_Msk;

  RCC_ADC_CLK_Enable();

  configADC();

  const int adcMax = 0xff;
  while (1)
  {
    uint16_t adcData = ADC1->DR & ADC_DR_DATA;
    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, adcData > 1 * (adcMax / 5));
    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, adcData > 2 * (adcMax / 5));
    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, adcData > 3 * (adcMax / 5));
    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, adcData > 4 * (adcMax / 5));
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
