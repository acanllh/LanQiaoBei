#ifndef __ACAN_H__
#define __ACAN_H__


#include "main.h"
#include "string.h"
#include "stdio.h"
#include "rtc.h"
#include "lcd.h"

//LED
void Led_On(void);
void Led_Off(void);
void Led_Random(uint8_t n);
void Led_Toggle(uint8_t n);
void Led_Up(void);
void Led_Dowm(void);
void Led_Random_Off(uint8_t n);
void led_stop(void);

//KEY
uint8_t Key_Scan(GPIO_TypeDef *GPIO, uint16_t GPIO_Pin);

//RTC
extern RTC_TimeTypeDef T;
extern RTC_DateTypeDef D;
void Rtc_Tim(void);
//display
void Init_Dis(void);
extern int floor_now;
#endif

