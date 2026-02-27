#include "main.h"
#include "stm32f072xb.h"
#include "stm32f0xx_hal.h"
#include "otherDefs.h"
#include "hal_gpio.h"

void SystemClock_Config(void);
void rxChar(USART_TypeDef *UartPeriph, char *c, int numChars);
void txString(USART_TypeDef *UartPeriph, char *str);

char prompt[] = "CMD?\r\n";

void USART1_IRQHandler()
{
  char received[3];
  rxChar(USART1, received, 2);
  uint16_t pinToToggle;

  int valid = 1;

  switch (received[0])
  {
  case 'r':
    txString(USART1, "LED r: ");
    pinToToggle = GPIO_PIN_6;
    break;
  case 'b':
    txString(USART1, "LED b: ");
    pinToToggle = GPIO_PIN_7;
    break;
  case 'o':
    txString(USART1, "LED o: ");
    pinToToggle = GPIO_PIN_8;
    break;
  case 'g':
    txString(USART1, "LED g: ");
    pinToToggle = GPIO_PIN_9;
    break;
  default:
    txString(USART1, "Invalid LED!\r\n");
    valid = 0;
    break;
  }

  if (valid)
  {
    switch (received[1])
    {
    case '0':
      txString(USART1, "OFF\r\n");
      My_HAL_GPIO_WritePin(GPIOC, pinToToggle, GPIO_PIN_RESET);
      break;
    case '1':
      txString(USART1, "ON\r\n");
      My_HAL_GPIO_WritePin(GPIOC, pinToToggle, GPIO_PIN_SET);
      break;
    case '2':
      txString(USART1, "TOGGLE\r\n");
      My_HAL_GPIO_TogglePin(GPIOC, pinToToggle);
      break;
    default:
      txString(USART1, "Invalid LED Toggle!\r\n");
      return;
      break;
    }
  }

  txString(USART1, prompt);
}

void USART1_Setup()
{
  // Enable UART2 in RCC
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

void txChar(USART_TypeDef *UartPeriph, char c)
{
  while ((UartPeriph->ISR & USART_ISR_TXE) == 0)
  {
  }

  UartPeriph->TDR = c;
}

void txString(USART_TypeDef *UartPeriph, char *str)
{
  for (char *c = str; *c != '\0'; c++)
  {
    txChar(UartPeriph, *c);
  }
}

void rxChar(USART_TypeDef *UartPeriph, char *c, int numChars)
{
  for (int i = 0; i < numChars; i++)
  {
    while ((UartPeriph->ISR & USART_ISR_RXNE) == 0)
    {
    }

    c[i] = UartPeriph->RDR;
  }
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

  USART1_Setup();

  RCC_GPIOC_CLK_Enable();
  My_HAL_GPIO_InitLEDs();
  txString(USART1, prompt);

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
