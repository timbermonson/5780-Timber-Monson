#include "main.h"
#include "stm32f0xx_hal.h"
#include <assert.h>
#include "otherDefs.h"

void SystemClock_Config(void);

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

  HAL_RCC_GPIOC_CLK_Enable();

  GPIO_InitTypeDef initStr = {
      GPIO_PIN_8 | GPIO_PIN_9, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL};

  // HAL_GPIO_Init(GPIOC, &initStr);                     // Initialize pins PC8 & PC9
  My_HAL_GPIO_Init(GPIOC, &initStr);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET); // Start PC8 high

  /* ~~~Exercise 1.1~~~:
  - GPIOC_MODEr bits to change:
  PC6: bits 13/12 to 10
  PC7: bits 15/14 to 10
  PC8: bits 17/16 to 10
  PC9: bits 19/18 to 10

  - GPIOA_MODEr bits to change:
  PA0 to GP Out: bits 0/1 to 00
   */

  /* ~~~Execise 1.2~~~:
  - LED Pin setting PC6/7/8/9 to Low-speed push-pull with no pullups:
    * GPIOC_OTYPER: bits 6/7/8/9 to 0 for Push-Pull
    * GPIOC_OSPEEDR: bits (12,13) (14,15) (16,17) (18,19) each set to 00 for Low-Speed (technically
      x0)
    * GPIOC_PUPDR: bits (12,13) (14,15) (16,17) (18,19) each set to 00 for No-Pullup-No-Pulldown

  - Setting USER button:
  * GPIOA_OSPEEDR: bits (0,1) to 00 for Low-Speed (or x0)
    * GPIOA_PUPDR: bit (0,1) to 10 for Pull-Down
   */

  // ~~~Exercise 1.3~~~
  // PC8/9 are in output mode
  uint32_t GPIOC_MODER_EXP = (0b01 << 2 * 8) | (0b01 << 2 * 9);
  assert(GPIOC->MODER == GPIOC_MODER_EXP);

  // PC pullups aren't being used
  assert(GPIOC->PUPDR == 0);

  // Starts with PC8 turned on
  uint32_t GPIOC_ODR_EXP = 1 << 8;
  assert(GPIOC->ODR == GPIOC_ODR_EXP);

  while (1)
  {
    HAL_Delay(200); // Delay 200ms
    // Toggle the output state of both PC8and PC9
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8 | GPIO_PIN_9);
  }
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
