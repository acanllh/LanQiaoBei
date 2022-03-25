#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "main.h"
#include "i2c.h"
#include "gpio.h"
#include "adc.h"

void led_random(uint8_t n);//led
void led_off(void);
void led_on(void);
void led_toggle(uint8_t n);

uint8_t eeprom_read(unsigned char addr);//eeprom
void eeprom_wite(unsigned char addr, unsigned char data);

uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);//key

uint16_t adc_read(void);//adc

#endif
