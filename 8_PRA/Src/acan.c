#include "acan.h"

void Led_Off()
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_All, GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(LOCK_GPIO_Port, LOCK_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LOCK_GPIO_Port, LOCK_Pin, GPIO_PIN_RESET);
}


void Led_On()
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_All, GPIO_PIN_RESET);
	
	HAL_GPIO_WritePin(LOCK_GPIO_Port, LOCK_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LOCK_GPIO_Port, LOCK_Pin, GPIO_PIN_RESET);
}

void Led_Random(uint8_t n)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8 << (n - 1), GPIO_PIN_RESET);
	
	HAL_GPIO_WritePin(LOCK_GPIO_Port, LOCK_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LOCK_GPIO_Port, LOCK_Pin, GPIO_PIN_RESET);
}

void Led_Random_Off(uint8_t n)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8 << (n - 1), GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(LOCK_GPIO_Port, LOCK_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LOCK_GPIO_Port, LOCK_Pin, GPIO_PIN_RESET);
}

void Led_Toggle(uint8_t n)
{
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8 << (n - 1)) == GPIO_PIN_RESET)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8 << (n - 1), GPIO_PIN_SET);
	}
	else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8 << (n - 1)) == GPIO_PIN_SET)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8 << (n - 1), GPIO_PIN_RESET);
	}

	HAL_GPIO_WritePin(LOCK_GPIO_Port, LOCK_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LOCK_GPIO_Port, LOCK_Pin, GPIO_PIN_RESET);
}

void Led_Up()
{
	uint8_t n;
	
	for(n = 5; n < 9; n++)
	{
		Led_Toggle(n);
		HAL_Delay(100);
		Led_Toggle(n);
	}
}

void Led_Dowm()
{
	uint8_t n;
	
	for(n = 8; n > 4; n--)
	{
		Led_Toggle(n);
		HAL_Delay(100);
		Led_Toggle(n);
	}
}

void led_stop(void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(LOCK_GPIO_Port, LOCK_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LOCK_GPIO_Port, LOCK_Pin, GPIO_PIN_RESET);
}

uint8_t Key_Scan(GPIO_TypeDef *GPIO, uint16_t Pin)
{
	if (HAL_GPIO_ReadPin(GPIO, Pin) == GPIO_PIN_RESET)
	{
		while (HAL_GPIO_ReadPin(GPIO, Pin) == GPIO_PIN_RESET);
		
		return 1;
	}
	else
	return 0;
}

void Rtc_Tim(void)
{
	char buff_1[50];
	
	HAL_RTC_GetTime(&hrtc, &T, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &D, RTC_FORMAT_BIN);
	
//	memset(buff_1, 0, sizeof(buff_1));
//	sprintf(buff_1, "Date: %02d-%02d-%02d ", D.Year, D.Month, D.Date);
//	LCD_DisplayStringLine(Line2, (uint8_t *) buff_1);
	sprintf(buff_1, "     %02d: %02d: %02d ", T.Hours, T.Minutes, T.Seconds);
	LCD_DisplayStringLine(Line8, (uint8_t *) buff_1);
}

void Init_Dis()
{
	char buff1[100];
	
	sprintf(buff1, "       LEVEL    ");
	LCD_DisplayStringLine(Line2, (u8 *)buff1);
	sprintf(buff1, "         %d      ", floor_now);
	LCD_DisplayStringLine(Line5, (u8 *)buff1);
}

