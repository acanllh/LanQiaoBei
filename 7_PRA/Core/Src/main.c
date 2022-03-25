/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
int n = 0, k = 0;
uint8_t data = 0, count = 1;
//lcd
char buff1[100];
char buff2[100];
char buff3[100];
char buff4[100];
//串口
uint8_t temp[10];
uint8_t re_data;
char buff5[100];
//adc值
float value;
uint8_t level1 = 30, level2 = 50, level3 = 70, level_now = 0, level_old;
int change;
float height;
int flag3 = 0;
int flag4 = 0;
//按键
int flag1 = 0, flag2 = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

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
  MX_TIM1_Init();
  MX_ADC2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	I2CInit();
	LCD_Init();
	LCD_Clear(Black);
	
	//初始化一次
//	eeprom_wite(0x00, 10);
//	HAL_Delay(5);
//	eeprom_wite(0x01, 20);
//	HAL_Delay(5);
//	eeprom_wite(0x02, 70);
//	HAL_Delay(5);

	level1 = eeprom_read(0x00);
	HAL_Delay(5);
	level2 = eeprom_read(0x01);
	HAL_Delay(5);
	level3 = eeprom_read(0x02);
	
	led_off();
	LCD_SetTextColor(White);
	LCD_SetBackColor(Black);
	HAL_UART_Receive_IT(&huart1, (u8 *)temp, 1);
	value = adc_read() * 3.3/4096;
	HAL_TIM_Base_Start_IT(&htim1);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		//闪灯、adc读值并得到
		value = adc_read() * 3.3/4096;
		led_toggle(1);
		height = (100.0/3.3) * value;
		if(height <= level1)
			level_now = 0;
		else if ((height > level1) && (height <= level2))
			level_now = 1;
		else if ((height > level2) && (height <= level3))
			level_now = 2;
		else if (height > level3)
			level_now = 3;
		if (flag4 == 0)
		{
			level_old = level_now;
			flag4 = 1;
		}
		else
		{
			if(level_old != level_now)
			{
				if(level_old > level_now)
				{
					sprintf(buff5, "A:H%.0f+L%d+D\r\n", height, level_now);
					HAL_UART_Transmit(&huart1, (uint8_t *)buff5, strlen(buff5), 50);
				}
				else
				{
					sprintf(buff5, "A:H%.0f+L%d+U\r\n", height, level_now);
					HAL_UART_Transmit(&huart1, (uint8_t *)buff5, strlen(buff5), 50);
				}
			}
		}
		
		level_old = level_now;
			
		//lcd程序
		if(flag1 == 0)
		{
			sprintf(buff1, "  Liquid Level        ");
			LCD_DisplayStringLine(Line0, (u8 *)buff1);
			sprintf(buff2, "  Height:  %.0fcm        ", height);
			LCD_DisplayStringLine(Line3, (u8 *)buff2);
			sprintf(buff3, "  ADC:  %.2f        ", value);
			LCD_DisplayStringLine(Line5, (u8 *)buff3);
			sprintf(buff4, "  Level:  %d        ", level_now);
			LCD_DisplayStringLine(Line7, (u8 *)buff4);
		}
		else if(flag1 == 1)
		{
			sprintf(buff1, "  Parameter Setup        ");
			LCD_DisplayStringLine(Line0, (u8 *)buff1);
			
			if (flag2 == 1)
			{
				LCD_SetBackColor(Red);
				if ((level1 <= 95) && (level1 >= 5))
				{
					level1 += change;
					if (level1 > 95)
						level1 = 95;
					else if(level1 < 5)
						level1 = 5;
					eeprom_wite(0, level1);
					change = 0;
				}
			}
			sprintf(buff2, "  Threshold1:  %dcm        ", level1);
			LCD_DisplayStringLine(Line3, (u8 *)buff2);
			LCD_SetBackColor(Black);
			
			if (flag2 == 2)
			{
				LCD_SetBackColor(Red);
				if ((level2 <= 95) && (level2 >= 5))
				{
					level2 += change;
					if (level2 > 95)
						level2 = 95;
					else if(level2 < 5)
						level2 = 5;
					eeprom_wite(1, level2);
					change = 0;
				}
			}
			sprintf(buff3, "  Threshold1:  %dcm        ", level2);
			LCD_DisplayStringLine(Line5, (u8 *)buff3);		
			LCD_SetBackColor(Black);
			
			if (flag2 == 3)
			{
				LCD_SetBackColor(Red);
				if ((level3 <= 95) && (level3 >= 5))
				{
					level3 += change;
					if (level3 > 95)
						level3 = 95;
					else if(level3 < 5)
						level3 = 5;
					eeprom_wite(2, level3);
					change = 0;
				}
			}
			sprintf(buff4, "  Threshold1:  %dcm        ", level3);
			LCD_DisplayStringLine(Line7, (u8 *)buff4);
			LCD_SetBackColor(Black);
			
		}
		HAL_Delay(1000);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_ADC12;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12CLKSOURCE_SYSCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim1)
	{

		//按键切换
		if (Key_Scan(B1_GPIO_Port, B1_Pin) == 1)
		{
			flag1++;
			if(flag1 > 1)
				flag1 = 0;
		}
		else if (Key_Scan(B2_GPIO_Port, B2_Pin) == 1)
		{
			flag2++;
			if(flag2 > 3)
				flag2 = 0;
		}
		else if (Key_Scan(B3_GPIO_Port, B3_Pin) == 1)
		{
			change = 5;
		}
		else if (Key_Scan(B4_GPIO_Port, B4_Pin) == 1)
		{
			change = -5;
		}
		
		//串口操作
		if(temp[0] == 'C')
		{
			sprintf(buff5, "C:H%.0f+L%d\r\n", height, level_now);
			HAL_UART_Transmit(&huart1, (uint8_t *)buff5, strlen(buff5), 50);
			temp[0] = '0';
		}
		else if (temp[0] == 'S')
		{
			sprintf(buff5, "S:TL%d+TM%d+TH%d\r\n", level1, level2, level3);
			HAL_UART_Transmit(&huart1, (uint8_t *)buff5, strlen(buff5), 50);
			temp[0] = '0';
		}
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	HAL_UART_Receive_IT(&huart1, (u8 *)temp, 1);
}

/* USER CODE END 4 */

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
