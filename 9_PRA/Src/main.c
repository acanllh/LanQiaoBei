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
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "acan.h"
#include "stdio.h"
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
//按键
int Key_Long[3] = {0};
int Key_Short[3] = {0};
int flag[3] = {0};
int Key_State[5] = {0};
int Key_Count[5] = {0};

//LCD
char buff1[50], buff2[50];
int Lcd_flag = 1;

//LED
int led_flag = 0;

//EEPROM
uint8_t ch = 0;

//中断

//时间
uint8_t hour[6] = {0}, min[6] = {0}, sec[6] = {0};
int P_flag = -1, Setting_flag = 0;
uint8_t Cur_position = 1;
int flag_save = 0;
int flag_change = 0;
int flag_work = 0;
int flag_state = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Key_Scan(void);//按键扫描及处理
void Lcd_Show(void);//显示基础信息（界面）
void Tim_save(void);//将时间储存到EEPROM中
void Tim_setting(void);//时间设定（高亮）
void Key_count(void);//按键计数
void TIME_init(void);//时间初始化（***只进行一次**）
void Time_State(void);//状态显示
void Tim_work(void);//定时器运行
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
  MX_TIM3_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
	LCD_Init();
	LCD_Clear(Black);
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);
	Led_Off();
	TIME_init();//只进行一次
	HAL_TIM_Base_Start_IT(&htim1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		Tim_save();
		Key_Scan();
		Lcd_Show();
		Time_State();
		if (flag_work != 1)
			Led_Off();
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
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static int i = 0, j = 0, k = 0, m = 0;
	i++;
	j++;
	m++;

	Key_count();
	
	if (m == 1000)
	{
		Tim_work();	
		m = 0;
	}
	if (i == 400)
	{	
		i = 0;
		Tim_setting();
	}

	if (j == 400)
	{		
		//按键三长按
		if (Key_Count[3] > 1000)
		{
			if (P_flag == 0)
					{
						hour[Cur_position - 1]++;
						if (hour[Cur_position - 1] > 12)
							hour[Cur_position - 1] = 0;
					}
					else if (P_flag == 1)
					{
						min[Cur_position - 1]++;
						if (min[Cur_position - 1] > 60)
							min[Cur_position - 1] = 0;
					}
					else if (P_flag == 2)
					{
						sec[Cur_position - 1]++;
						if (sec[Cur_position - 1] > 60)
							sec[Cur_position - 1] = 0;
					}
		}
		j = 0;
	}
	if (flag_work == 1)
	{
		k++;
		if (k == 500)
		{
			if (led_flag == 0)
			{
				Led_bytes(0x01ff);
				led_flag = 1;
			}
			else if (led_flag == 1)
			{
				Led_bytes(0x00ff);
				led_flag = 0;
			}
			k = 0;
		}
	}
}
	
void Key_count(void)
{
	if (Key_State[1] != 0)
	{
		Key_Count[1]++;
	}
	if (Key_State[2] != 0)
	{
		Key_Count[2]++;
	}
	if (Key_State[3] != 0)
	{
		Key_Count[3]++;
	}
	if (Key_State[4] != 0)
	{
		Key_Count[4]++;
	}
}

void Key_Scan(void)
{
	
	//按键二
	if (HAL_GPIO_ReadPin(B2_GPIO_Port, B2_Pin) == GPIO_PIN_RESET)
	{
		//消抖
		HAL_Delay(120);
			if (HAL_GPIO_ReadPin(B2_GPIO_Port, B2_Pin) == GPIO_PIN_RESET)
			{
				Key_State[2] = 1;
				while(HAL_GPIO_ReadPin(B2_GPIO_Port, B2_Pin) == GPIO_PIN_RESET);
				Key_State[2] = 0;
				//长按
				if (Key_Count[2] > 700)
				{
					flag_save = 1;
					Setting_flag = 0;
					flag_state = 0;
					Lcd_flag = Cur_position;
					P_flag = -1;
				}
				//短按
				else
				{
					Lcd_flag = 0;
					Setting_flag = 1;
					flag_state = 1;
					P_flag++;
					if (P_flag > 2)
					P_flag = 0;
				}				
			Key_Count[2] = 0;
			}
		}
	//按键三
	if (HAL_GPIO_ReadPin(B3_GPIO_Port, B3_Pin) == GPIO_PIN_RESET)
	{
		//消抖
		HAL_Delay(120);
			if (HAL_GPIO_ReadPin(B3_GPIO_Port, B3_Pin) == GPIO_PIN_RESET)
			{
				Key_State[3] = 1;
				while(HAL_GPIO_ReadPin(B3_GPIO_Port, B3_Pin) == GPIO_PIN_RESET);
				Key_State[3] = 0;
				//长按
				if (Key_Count[3] > 700)
				{
					
				}
				//短按
				else
				{
					if (P_flag == 0)
					{
						hour[Cur_position - 1]++;
						if (hour[Cur_position - 1] > 12)
							hour[Cur_position - 1] = 0;
					}
					else if (P_flag == 1)
					{
						min[Cur_position - 1]++;
						if (min[Cur_position - 1] > 60)
							min[Cur_position - 1] = 0;
					}
					else if (P_flag == 2)
					{
						sec[Cur_position - 1]++;
						if (sec[Cur_position - 1] > 60)
							sec[Cur_position - 1] = 0;
					}
				}				
			Key_Count[3] = 0;
			}
		}
	//按键四
	if (HAL_GPIO_ReadPin(B4_GPIO_Port, B4_Pin) == GPIO_PIN_RESET)
	{
		//消抖
		HAL_Delay(120);
			if (HAL_GPIO_ReadPin(B4_GPIO_Port, B4_Pin) == GPIO_PIN_RESET)
			{
				Key_State[4] = 1;
				while(HAL_GPIO_ReadPin(B4_GPIO_Port, B4_Pin) == GPIO_PIN_RESET);
				Key_State[4] = 0;
				//长按
				if (Key_Count[4] > 700)
				{
					flag_work = 0;
					flag_state = 0;
				}
				//短按
				else
				{
					flag_work++; 
					if (flag_work > 2)
						flag_work = 1;
				}				
			Key_Count[4] = 0;
			}
		}
	//按键一
	if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET)
	{
		//消抖
		HAL_Delay(120);
			if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET)
			{
				Key_State[1] = 1;
				while(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET);
				Key_State[1] = 0;
				//长按
				if (Key_Count[1] > 700)
				{
					
				}
				//短按
				else
				{
					Lcd_flag++;
					if (Lcd_flag > 5)
						Lcd_flag = 1;
				}				
			Key_Count[1] = 0;
			}
		}
	
}

void Tim_work(void)
{
	if (flag_work == 1)
	{
		flag_state = 2;
		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
		__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 800);
		if (sec[Cur_position - 1] > 0)
		{
			sec[Cur_position - 1]--;
		}
		else if (min[Cur_position - 1] > 0)
		{
			sec[Cur_position - 1] = 60;
			min[Cur_position - 1]--;
		}		
		else if (hour[Cur_position - 1] > 0)
		{
			min[Cur_position - 1] = 60;
			hour[Cur_position - 1]--;
		}
	}
	else if (flag_work == 2)
	{
		flag_state = 3;
		__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 0);
	}
}

void Tim_setting(void)
{
	int i = 0;
	
	if (Setting_flag == 1)
	{
		if (P_flag == 0)
		{
			//高亮 “时”
			sprintf(buff2, "    %02d: %02d: %02d", hour[Cur_position - 1], min[Cur_position - 1], sec[Cur_position - 1]);//6空白
			for (; i < 4; i++)
			{
				LCD_DisplayChar(Line4, 320 - (16 * i), buff2[i]);
			}
			LCD_SetBackColor(Yellow);
			LCD_SetTextColor(Black);
			for (i = 4; i < 6; i++)
			{
				LCD_DisplayChar(Line4, 320 - (16 * i), buff2[i]);
			}
			LCD_SetBackColor(Black);
			LCD_SetTextColor(White);
			for (i = 6; i < 14; i++)
			{
				LCD_DisplayChar(Line4, 320 - (16 * i), buff2[i]);
			}
		}
		else if (P_flag == 1)
		{
			//高亮“分”
			sprintf(buff2, "    %02d: %02d: %02d", hour[Cur_position - 1], min[Cur_position - 1], sec[Cur_position - 1]);//6空白
			for (; i < 8; i++)
			{
				LCD_DisplayChar(Line4, 320 - (16 * i), buff2[i]);
			}
			LCD_SetBackColor(Yellow);
			LCD_SetTextColor(Black);
			for (i = 8; i < 10; i++)
			{
				LCD_DisplayChar(Line4, 320 - (16 * i), buff2[i]);
			}
			LCD_SetBackColor(Black);
			LCD_SetTextColor(White);
			for (i = 10; i < 14; i++)
			{
				LCD_DisplayChar(Line4, 320 - (16 * i), buff2[i]);
			}
		}
		else if (P_flag == 2)
		{
			//高亮“秒”
			sprintf(buff2, "    %02d: %02d: %02d", hour[Cur_position - 1], min[Cur_position - 1], sec[Cur_position - 1]);//6空白
			for (; i < 12; i++)
			{
				LCD_DisplayChar(Line4, 320 - (16 * i), buff2[i]);
			}
			LCD_SetBackColor(Yellow);
			LCD_SetTextColor(Black);
			for (i = 12; i < 14; i++)
			{
				LCD_DisplayChar(Line4, 320 - (16 * i), buff2[i]);
			}
			LCD_SetBackColor(Black);
			LCD_SetTextColor(White);
		}
	}
}

void Lcd_Show(void)
{
	if (Lcd_flag == 1)
	{
		Cur_position = Lcd_flag;
		//显示储存位置1
		sprintf(buff1, "    %02d: %02d: %02d  ", hour[Cur_position - 1], min[Cur_position - 1], sec[Cur_position - 1]);//6空白
		LCD_DisplayStringLine(Line4, (u8 *)buff1);
		sprintf(buff1, "  No %d", Lcd_flag);
		LCD_DisplayStringLine(Line2, (u8 *)buff1);
	}
	else if (Lcd_flag == 2)
	{
		Cur_position = Lcd_flag;
		//显示储存位置2
		sprintf(buff1, "    %02d: %02d: %02d  ", hour[Cur_position - 1], min[Cur_position - 1], sec[Cur_position - 1]);//6空白
		LCD_DisplayStringLine(Line4, (u8 *)buff1);
		sprintf(buff1, "  No %d", Lcd_flag);
		LCD_DisplayStringLine(Line2, (u8 *)buff1);
	}
	else if (Lcd_flag == 3)
	{
		Cur_position = Lcd_flag;
		//显示储存位置3
		sprintf(buff1, "    %02d: %02d: %02d  ", hour[Cur_position - 1], min[Cur_position - 1], sec[Cur_position - 1]);//6空白
		LCD_DisplayStringLine(Line4, (u8 *)buff1);
		sprintf(buff1, "  No %d", Lcd_flag);
		LCD_DisplayStringLine(Line2, (u8 *)buff1);
	}
	else if (Lcd_flag == 4)
	{
		Cur_position = Lcd_flag;
		//显示储存位置4
		sprintf(buff1, "    %02d: %02d: %02d  ", hour[Cur_position - 1], min[Cur_position - 1], sec[Cur_position - 1]);//6空白
		LCD_DisplayStringLine(Line4, (u8 *)buff1);
		sprintf(buff1, "  No %d", Lcd_flag);
		LCD_DisplayStringLine(Line2, (u8 *)buff1);
	}
	else if (Lcd_flag == 5)
	{
		Cur_position = Lcd_flag;
		//显示储存位置5
		sprintf(buff1, "    %02d: %02d: %02d  ", hour[Cur_position - 1], min[Cur_position - 1], sec[Cur_position - 1]);//6空白
		LCD_DisplayStringLine(Line4, (u8 *)buff1);
		sprintf(buff1, "  No %d", Lcd_flag);
		LCD_DisplayStringLine(Line2, (u8 *)buff1);
	}
	
}
void Tim_save(void)
{
	if (flag_save == 1)
	{
		EE_Write((Cur_position - 1) * 3, hour[Cur_position - 1]);
		HAL_Delay(5);
		EE_Write((Cur_position - 1) * 3 + 1, min[Cur_position - 1]);
		HAL_Delay(5);
		EE_Write((Cur_position - 1) * 3 + 2, sec[Cur_position - 1]);
		HAL_Delay(5);
		flag_save = 0;
	}
}
void Time_State(void)
{
	if (flag_state == 0)
	{
		sprintf(buff1, "      Standby     ");
		LCD_DisplayStringLine(Line8, (u8 *)buff1);
	}
	else if (flag_state == 1)
	{
		sprintf(buff1, "      Setting     ");
		LCD_DisplayStringLine(Line8, (u8 *)buff1);
	}
	else if (flag_state == 2)
	{
		sprintf(buff1, "      Running     ");
		LCD_DisplayStringLine(Line8, (u8 *)buff1);
	}
	else if (flag_state == 3)
	{
		sprintf(buff1, "      Pause     ");
		LCD_DisplayStringLine(Line8, (u8 *)buff1);
	}
}

void TIME_init(void)
{
	if (EE_Read(15) != 255)
	{
		HAL_Delay(5);
		Cur_position = 1;
		hour[Cur_position - 1] = EE_Read((Cur_position - 1) * 3);
		HAL_Delay(5);
		min[Cur_position - 1] = EE_Read((Cur_position - 1) * 3 + 1);
		HAL_Delay(5);
		sec[Cur_position - 1] = EE_Read((Cur_position - 1) * 3 + 2);
		HAL_Delay(5);
		
		Cur_position = 2;
		hour[Cur_position - 1] = EE_Read((Cur_position - 1) * 3);
		HAL_Delay(5);
		min[Cur_position - 1] = EE_Read((Cur_position - 1) * 3 + 1);
		HAL_Delay(5);
		sec[Cur_position - 1] = EE_Read((Cur_position - 1) * 3 + 2);
		HAL_Delay(5);
		
		Cur_position = 3;
		hour[Cur_position - 1] = EE_Read((Cur_position - 1) * 3);
		HAL_Delay(5);
		min[Cur_position - 1] = EE_Read((Cur_position - 1) * 3 + 1);
		HAL_Delay(5);
		sec[Cur_position - 1] = EE_Read((Cur_position - 1) * 3 + 2);
		HAL_Delay(5);
		
		Cur_position = 4;
		hour[Cur_position - 1] = EE_Read((Cur_position - 1) * 3);
		HAL_Delay(5);
		min[Cur_position - 1] = EE_Read((Cur_position - 1) * 3 + 1);
		HAL_Delay(5);
		sec[Cur_position - 1] = EE_Read((Cur_position - 1) * 3 + 2);
		HAL_Delay(5);
		
		Cur_position = 5;
		hour[Cur_position - 1] = EE_Read((Cur_position - 1) * 3);
		HAL_Delay(5);
		min[Cur_position - 1] = EE_Read((Cur_position - 1) * 3 + 1);
		HAL_Delay(5);
		sec[Cur_position - 1] = EE_Read((Cur_position - 1) * 3 + 2);
		HAL_Delay(5);
		
		Cur_position = 1;
	}
	
	else
	{
		HAL_Delay(5);
		for (uint8_t i = 0; i < 15; i++)
		{
			EE_Write(i, 0);
			HAL_Delay(5);
		}
		EE_Write(15, 0);
		HAL_Delay(5);
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
