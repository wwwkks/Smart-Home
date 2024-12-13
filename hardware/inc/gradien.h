#ifndef __RGB_LED_H
#define __RGB_LED_H

#include "stm32f10x.h"
#define RGB_LED_TIME 1000  // 根据需要调整时间  10
// 定义颜色变化的周期
#define COLOR_CHANGE_PERIOD 30   //100
#define M_PI  3.14

void RGB_LED_Gradient_Start(void);
void RGB_LED_Gradient_Update(void);
void RGB_LED_Gradient_Lines(void);
void RGB_LED_Gradient_Horizontal(void);
void RGB_LED_Gradient_PerPoint(void);
void RGB_LED_close(void);

void RGB_LED_Gradient_PerPoint2(void);

void Timer_Config();

#endif // __RGB_LED_H
