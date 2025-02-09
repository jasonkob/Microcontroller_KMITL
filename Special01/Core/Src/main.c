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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdint.h>
#include <stdio.h>
uint8_t show_status = 0;
uint8_t RxData[100];
uint8_t inputstr [1];
uint8_t status = 1;
uint8_t showstr[100] = "";
//char showinstr[] = "Input :";
uint8_t recive_status = 1;
uint8_t transmit_status = 1;
uint8_t quit_mass[100] = "QUIT";
uint8_t setmyname_status = 0;
uint8_t setyourname_status = 0;
uint8_t Myname[100];
uint8_t Yourname[100];


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

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
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (status == 1) {
        if (recive_status == 1 && transmit_status == 0) {
            if (HAL_UART_Receive_IT(&huart1, RxData, 100) == HAL_OK) {
                if (strncmp((char *)RxData, "QUIT", 4) == 0) {  // ตรวจสอบว่าได้รับ "QUIT"
                    status = 0;  // ออกจากลูป
                    HAL_UART_Transmit(&huart3,"QUIT", 4, 1000);
                    return;
                }
                recive_status = 0;
                transmit_status = 1;
                setyourname_status += 1;
            }
        } else if (recive_status == 1 && transmit_status == 1) {
            if (HAL_UART_Receive_IT(&huart3, inputstr, 1) == HAL_OK) {
                Inputstr();
            }
        }
    }
}


void Inputstr(){
	HAL_UART_Transmit(&huart3,"\r", 2, 1000);
	showstatus();
	HAL_UART_Transmit(&huart3, showstr, 100, 1000);
	if(inputstr[0] == '\b'){
		showstr[strlen(showstr)-1] = '\000';
	}
	else if(inputstr[0] == '\r'){
		showstr[strlen(showstr)] = '\0';
		if(setmyname_status == 0){
			memcpy(Myname, showstr, sizeof(Myname));
			setmyname_status = 1;
		}
		HAL_UART_Transmit(&huart1,&showstr, 100, 100);
		HAL_UART_Transmit(&huart3,"\n\r", 2, 1000);
		memset(showstr, 0, sizeof(showstr));
		transmit_status = 0;
		show_status += 1;

	}
	else{
		HAL_UART_Transmit(&huart3,inputstr, 1, 1000);
		showstr[strlen(showstr)] = inputstr[0];
		if(showstr[0] == 'q'){
				HAL_UART_Transmit(&huart3,"\n\r", 2, 1000);
				HAL_UART_Transmit(&huart3,quit_mass,4, 1000);
				HAL_UART_Transmit(&huart1,quit_mass,100, 1000);
				status = 0;
		}
	//  HAL_UART_Transmit(&huart2, (uint8_t )&showstr, 1, 100);
	}


	memset(inputstr, 0, sizeof(inputstr));

}

void showstatus(){
	if(show_status == 0){
			HAL_UART_Transmit(&huart3,"Man From U.A.R.T.1!\n\r"
					  	  	  	  	  "Quit PRESS q \n\r", 35, 1000);
			show_status =1;
	}
	else if(show_status == 1){
			HAL_UART_Transmit(&huart3,"Name :", 6, 1000);
	}
	else if(show_status == 2){
			HAL_UART_Transmit(&huart3,Myname,100, 1000);
			HAL_UART_Transmit(&huart3," :", 2, 1000);

	}
	else if(show_status == 3){
			HAL_UART_Transmit(&huart3,Yourname,100, 1000);
			HAL_UART_Transmit(&huart3," :", 2, 1000);

	}
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
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  HAL_UART_Receive_IT(&huart1,RxData, 100);
  HAL_UART_Receive_IT(&huart3,inputstr, 1);
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

//	  HAL_UART_Receive(&huart1, RxData, 100, HAL_MAX_DELAY);
//	  HAL_UART_Transmit(&huart3, RxData, 100, HAL_MAX_DELAY);

//	  HAL_UART_Transmit(&huart1, (uint8_t *)".", 1, 100);
	  if(show_status == 0){
		  showstatus();
		  if(transmit_status == 1){
			  Inputstr();
		  }
	  }
	  if (recive_status == 0){
		  if(setyourname_status == 1){
			  memcpy(Yourname, RxData, sizeof(Yourname));
			  HAL_UART_Transmit(&huart3,"\r", 2, 1000);
			  HAL_UART_Transmit(&huart3, Yourname, 100, 1000);
			  HAL_UART_Transmit(&huart3, " is ready", 9, 1000);
			  HAL_UART_Transmit(&huart3,"\n\r", 2, 1000);
			  setyourname_status += 1;
			  show_status = 2;
			  memset(RxData, 0, sizeof(RxData));

		  }
		  else if(setyourname_status > 1){
			  HAL_UART_Transmit(&huart3,"\r", 2, 1000);
			  showstatus();
			  HAL_UART_Transmit(&huart3, RxData, 100, 1000);
			  HAL_UART_Transmit(&huart3,"\n\r", 2, 1000);
			  memset(RxData, 0, sizeof(RxData));
			  show_status -= 1;

		  }
		  if(transmit_status == 1){
			  Inputstr();
		  }
		  recive_status = 1;


	  }

	  if(transmit_status == 2){
	  		  HAL_UART_Transmit(&huart3,"\n\r", 2, 1000);
	  		  transmit_status = 0;
	  		  recive_status = 0;
	  }

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
