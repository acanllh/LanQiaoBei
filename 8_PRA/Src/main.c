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
#include "rtc.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "lcd.h"
#include "acan.h"
#include "stdlib.h "
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

//RTC
RTC_TimeTypeDef T;
RTC_DateTypeDef D;

//LCD
char buff1[50], buff2[50], buff3[50];

//IT
int i = 0, j = 0, k = 0, m = 0, n = 0, a = 0;
//楼层
int flag1 = 0, flag2 = 0, flag3 = 0, flag4 = 0;
int aim[4] = {0};
int temp[4] = {0};
int num = 0, num2 = 0, change = 0;
int floor_now = 1;
int motor_flag = 0;
int motor_go = 0;
int door_up_flag = 0, door_down_flag = 0, door_wait = 0;
int other_flag = 0;
int floor_lcd = 0;
int floor_shan = 0;
//按键
int flag_key = 0;
//LED
int led_up_flag = 0, led_down_flag = 0;
int led_off_flag = 0;
int led_break = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void Key_Control(void);
void Key_handel(void);
void Motor_Order(void);
void Motor_Work(void);
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
  MX_RTC_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  MX_TIM17_Init();
  /* USER CODE BEGIN 2 */
	LCD_Init();
	LCD_Clear(Black);
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1);
	//升降机门
	HAL_GPIO_WritePin(DO_IO_GPIO_Port, DO_IO_Pin, GPIO_PIN_SET);
	Init_Dis();
	Led_Off();
	HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		Rtc_Tim();
		Key_Control();
		if (motor_flag == 1)
		{
			Motor_Order();
			motor_flag = 0;
		}
		//ceshi
		sprintf(buff3, "num:%1d// %1d %1d %1d ch:%1d      ", num2, temp[0], temp[1], temp[2], change);
			LCD_DisplayStringLine(Line6, (u8 *)buff3);
		sprintf(buff3, "%d %d", floor_now, motor_flag);
			LCD_DisplayStringLine(Line7, (u8 *)buff3);
		//ceshi
		if (floor_lcd == 1)
		{
			sprintf(buff2, "       LEVEL    ");
			LCD_DisplayStringLine(Line2, (u8 *)buff2);
			sprintf(buff2, "         %d      ", floor_now);
			LCD_DisplayStringLine(Line5, (u8 *)buff2);
//			if (floor_shan >= 2)
//			{
//				floor_lcd = 0;
//				floor_shan = 0;
//			}
			
			if ((floor_shan < 2)&& (led_break == 0))// && (led_break < 3)
			{
//				if ((floor_now == aim[change]))
//					led_break++;
//				else led_break = 0;
				LCD_ClearLine(Line5);
			}
			else
			{
				led_break = 1;
				floor_lcd = 0;
				floor_shan = 0;
			}
		}
		
		//上下电梯指示灯
		if (led_up_flag == 1)
		{
			Led_Up();
		}
		else if (led_down_flag == 1)
		{
			Led_Dowm();
		}
		else if (led_off_flag == 1)
		{
			Led_Random_Off(floor_now);
		}
		else
		{
			led_stop();
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim2)
	{
		Key_handel();
		if (flag_key == 1)
		{
			j++;
		}
		if (j == 1000)
		{
			j = 0;
			flag_key = 0;
			motor_flag = 1;
		}
		if (motor_go == 1)
		{
			Motor_Work();
			i++;
			if (i == 6000)
			{
				i = 0;
				motor_flag = 0;
				led_off_flag = 1;
				led_down_flag = 0;
				led_up_flag = 0;
				door_up_flag = 1;
				motor_go = 0;
				floor_lcd = 1;
				
				if (temp[change] > floor_now)
				{
					floor_now++;
				}
				else if (temp[change] < floor_now)
				{
					floor_now--;
				}
			}
		}
		//开门
		if (door_up_flag == 1)
		{
			HAL_GPIO_WritePin(DO_IO_GPIO_Port, DO_IO_Pin, GPIO_PIN_SET);
			__HAL_TIM_SetCompare(&htim17, TIM_CHANNEL_1, 300);
			k++;
			if (k == 4000)
			{
				k = 0;
				if (temp[change + 1] != 0)
				{
					door_wait = 1;
				}
				else
				{
					door_down_flag = 1;
				}
				door_up_flag = 0;
			}
		}
		//停留
		if (door_wait == 1)
		{
			m++;
			if (m == 2000)
			{
				m = 0;
				door_down_flag = 1;
			}
		}
		//关门
		if (door_down_flag == 1)
		{
			HAL_GPIO_WritePin(DO_IO_GPIO_Port, DO_IO_Pin, GPIO_PIN_RESET);
			__HAL_TIM_SetCompare(&htim17, TIM_CHANNEL_1, 250);
			n++;
			if (n == 4000)
			{
				n = 0;
				door_down_flag = 0;
				if (change < num2)
				{
					motor_go = 1;
				}
			}
		}
		//闪灯
		if (floor_lcd == 1)
		{
			a++;
			if (a == 375)
			{
				a = 0;
				floor_shan++;
			}
		}
		
		
	}
}

void Motor_Work(void)
{
	if (change < num2)
	{
		if (temp[change] > floor_now)
		{
			led_up_flag = 1;
			__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 800);
			HAL_GPIO_WritePin(MA_IO_GPIO_Port, MA_IO_Pin, GPIO_PIN_SET);
//			floor_now++;
		}
		else if (temp[change] < floor_now)
		{
			led_down_flag = 1;
			__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 600);
			HAL_GPIO_WritePin(MA_IO_GPIO_Port, MA_IO_Pin, GPIO_PIN_RESET);
//			floor_now--;
		}
		else if (floor_now == temp[change])
		{
			if (temp[change + 1] != 0)
			{
				change++;
				led_break = 0;
			}
		}
	}
}

void Motor_Order(void)
{
	int i, value, j;
	
	for(i = 0; i < num-1; i++)
	{
		if (aim[i] < aim[i + 1])
		{
			value = aim[i];
			aim[i] = aim[i + 1];
			aim[i + 1] = value;
		}
	}
	for (j = 0; j < 3; j++)
	{
		temp[j] = aim[j];
	}
	num2 = num;
	num = 0;
	for (j = 0; j < 3; j++)
	{
		aim[j] = 0;
	}
	HAL_GPIO_WritePin(DO_IO_GPIO_Port, DO_IO_Pin, GPIO_PIN_RESET);
	door_down_flag = 1;
}

void Key_handel(void)
{
	if(Key_Scan(KEY1_GPIO_Port, KEY1_Pin) == 1)
		{
			flag_key = 1;
			flag1 = 1;
			j = 0;
		}
		else if (Key_Scan(KEY2_GPIO_Port, KEY2_Pin) == 1)
		{
			flag_key = 1;
			flag2 = 1;
			j = 0;
		}
		else if (Key_Scan(KEY3_GPIO_Port, KEY3_Pin) == 1)
		{
			flag_key = 1;
			flag3 = 1;
			j = 0;
		}
		else if (Key_Scan(KEY4_GPIO_Port, KEY4_Pin) == 1)
		{
			flag_key = 1;
			flag4 = 1;
			j = 0;
		}
}

void Key_Control(void)
{
	if (flag1 == 1)
		{
			if(floor_now != 1)
			{
				if (num < 3)
				aim[num++] = 1;
				Led_Random(1);
			}
			flag1 = 0;
		}
		else if (flag2 == 1)
		{
			if(floor_now != 2)
			{
				if (num < 3)
				aim[num++] = 2;
				Led_Random(2);
			}
			flag2 = 0;
		}
		else if (flag3 == 1)
		{
			if(floor_now != 3)
			{
				if (num < 3)
				aim[num++] = 3;
				Led_Random(3);
			}
			flag3 = 0;
		}
		else if (flag4 == 1)
		{
			if(floor_now != 4)
			{
				if (num < 3)
				aim[num++] = 4;
				Led_Random(4);
			}
			flag4 = 0;
		}
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
