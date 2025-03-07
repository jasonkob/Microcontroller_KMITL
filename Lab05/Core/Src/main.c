/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "usart.h"
#include "gpio.h"
#include "stdio.h"
#include "string.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
volatile uint32_t adc_val = 0;
volatile uint32_t adc_avg_8 = 0;
volatile uint32_t adc_avg_16 = 0;
float voltage = 0;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void displayHEX(uint32_t num ,float voltage)
{
    char buffer[50];
    sprintf(buffer, "ADC1_CH10 0x%08lX Vin = %.2f V\r\n", num, voltage);
    HAL_UART_Transmit(&huart3, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
}
void displayLavel(uint32_t adcValue){
	uint8_t level = (adcValue*4) / 4095;
	HAL_GPIO_WritePin(GPIOG ,GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOG ,GPIO_PIN_14, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOF ,GPIO_PIN_15, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE ,GPIO_PIN_13, GPIO_PIN_RESET);

	switch(level){
		case 0:

		break;
		case 1:
			HAL_GPIO_WritePin(GPIOG ,GPIO_PIN_9, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOG ,GPIO_PIN_14, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOF ,GPIO_PIN_15, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE ,GPIO_PIN_13, GPIO_PIN_RESET);
		break;
		case 2:
			HAL_GPIO_WritePin(GPIOG ,GPIO_PIN_14, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOG ,GPIO_PIN_9, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOF ,GPIO_PIN_15, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE ,GPIO_PIN_13, GPIO_PIN_RESET);
		break;
		case 3:
			HAL_GPIO_WritePin(GPIOG ,GPIO_PIN_14, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE ,GPIO_PIN_13, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOG ,GPIO_PIN_9, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOF ,GPIO_PIN_15, GPIO_PIN_RESET);
		break;
		case 4:
			HAL_GPIO_WritePin(GPIOG ,GPIO_PIN_9, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOG ,GPIO_PIN_14, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOF ,GPIO_PIN_15, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE ,GPIO_PIN_13, GPIO_PIN_SET);
			HAL_Delay(300);
		break;
		default:
			break;
	}
}
int average_8(int x)
{
	static int samples[8];
	static int i = 0;
	static int total = 0;

	total += x - samples[i];
	samples[i] = x;
	i = (i==7 ? 0 : i+1);
	return total>>3;
}
int average_16(int x)
{
	static int samples[16];
	static int i = 0;
	static int total = 0;

	total += x - samples[i];
	samples[i] = x;

	i = (i == 15 ? 0 : i+1);
	return total >> 4;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_ADC_Start(&hadc1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	 adc_val = HAL_ADC_GetValue(&hadc1);
	 adc_avg_8 = average_8(adc_val);
	 adc_avg_16 = average_16(adc_val);
	 voltage = (adc_val * 3.3f) / 4095.0f;
	 displayHEX(adc_val, voltage);
	 displayLavel(adc_val);
	 HAL_Delay(300);
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

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
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
