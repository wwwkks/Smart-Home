#ifndef _led_H
#define _led_H

#include "system.h"

/*  LED时钟端口、引脚定义 */
#define LED1_PORT 			GPIOB   
#define LED1_PIN 			GPIO_Pin_5
#define LED1_PORT_RCC		RCC_APB2Periph_GPIOB

#define LED2_PORT 			GPIOE   
#define LED2_PIN 			GPIO_Pin_5
#define LED2_PORT_RCC		RCC_APB2Periph_GPIOE


#define LED1 PBout(5)  	
#define LED2 PEout(5)  	

#define LED_ON 0

#define LED_OFF	1

typedef struct
{

	_Bool LED_Status;

} LED_INFO;

extern LED_INFO led_info;
extern LED_INFO led2_info;

void LED_Init(void);

void LED_Set(_Bool status);
void LED2_Set(_Bool status);


#endif
