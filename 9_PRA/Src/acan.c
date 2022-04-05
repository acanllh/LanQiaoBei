#include "acan.h"

//LED
void Led_Off(void)
{
	GPIOC->BSRR = 0xff00;
	
	HAL_GPIO_WritePin(LOCK_GPIO_Port, LOCK_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LOCK_GPIO_Port, LOCK_Pin, GPIO_PIN_RESET);
}

void Led_On(void)
{
	GPIOC->BRR = 0xff00;
	
	HAL_GPIO_WritePin(LOCK_GPIO_Port, LOCK_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LOCK_GPIO_Port, LOCK_Pin, GPIO_PIN_RESET);
}

void Led_bytes(uint32_t data)
{
		GPIOC->BRR = data;
		GPIOC->BSRR = data ^ 0xff00;
	
		HAL_GPIO_WritePin(LOCK_GPIO_Port, LOCK_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LOCK_GPIO_Port, LOCK_Pin, GPIO_PIN_RESET);
}


//EEPROM
void EE_Write(uint8_t add, uint8_t data)
{
	I2CStart();
	I2CSendByte(0xa0);
	I2CWaitAck();
	
	I2CSendByte(add);
	I2CWaitAck();
	I2CSendByte(data);
	I2CWaitAck();
	I2CStop();
}

uint8_t EE_Read(uint8_t add)
{
	uint8_t data;
	
	I2CStart();
	I2CSendByte(0xa0);
	I2CWaitAck();
	I2CSendByte(add);
	I2CWaitAck();
	
	I2CStart();
	I2CSendByte(0xa1);
	I2CWaitAck();
	data = I2CReceiveByte();
	I2CWaitAck();
	I2CStop();
	
	return data;
}



