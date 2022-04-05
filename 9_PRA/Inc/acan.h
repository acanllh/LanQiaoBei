#ifndef __ACAN_H__
#define __ACAN_H__

#include "lcd.h"
#include "main.h"
#include "i2c.h"

//LED
void Led_Off(void);
void Led_On(void);
void Led_bytes(uint32_t data);

//EEPROM
void EE_Write(uint8_t add, uint8_t data);
uint8_t EE_Read(uint8_t add);



#endif


