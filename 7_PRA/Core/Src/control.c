#include "control.h"


//led控制函数
void led_random(uint8_t n)
{	
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8 << (n - 1), GPIO_PIN_RESET);
	
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}

void led_toggle(uint8_t n)
{
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8 << (n - 1)) == GPIO_PIN_SET)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8 << (n - 1), GPIO_PIN_RESET);
	}
	else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8 << (n - 1)) == GPIO_PIN_RESET)
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8 << (n - 1), GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}

void led_off()
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_All, GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}

void led_on()
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_All, GPIO_PIN_RESET);
	
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}


//eeprom函数
void eeprom_wite(unsigned char addr, unsigned char data)
{
	I2CStart();
	I2CSendByte(0xa0);
	I2CWaitAck();
	
	I2CSendByte(addr);
	I2CWaitAck();
	I2CSendByte(data);
	I2CWaitAck();
	
	I2CStop();
}

uint8_t eeprom_read(unsigned char addr)
{
	uint8_t data;
	
	I2CStart();
	I2CSendByte(0xa0);
	I2CWaitAck();
	
	I2CSendByte(addr);
	I2CWaitAck();
	
	I2CStart();
	I2CSendByte(0xa1);
	I2CWaitAck();
	data = I2CReceiveByte();
	I2CWaitAck();
	I2CStop();
	
	return data;
}

uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)

{			

	/*检测是否有按键按下 */

	if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) == GPIO_PIN_RESET )  

	{	 

		/*等待按键释放 */

		while(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) == GPIO_PIN_RESET);   

		return 	1;	 

	}

	else

		return 0;

}

uint16_t adc_read(void)
{
	uint16_t value = 0;
	int i;
	
for(i = 0; i <10; i++)
	{
		HAL_ADC_Start(&hadc2);
		value += HAL_ADC_GetValue(&hadc2);
	}

	return value / 10;
}

