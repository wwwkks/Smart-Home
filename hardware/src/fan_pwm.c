#include "pwm.h"



/*******************************************************************************
* 函 数 名         : TIM2_CH2_PWM_Init
* 函数功能		   : TIM2通道2 PWM初始化函数
* 输    入         : per:重装载值
					 psc:分频系数
* 输    出         : 无
*******************************************************************************/
void TIM4_CH1_PWM_Init(u16 per, u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* 开启时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    
    /* 配置GPIO的模式和IO口 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    TIM_TimeBaseInitStructure.TIM_Period = per;   // 自动装载值
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc; // 分频系数
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 设置向上计数模式
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);    
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // 通常使用高电平有效
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC1Init(TIM4, &TIM_OCInitStructure); // 输出比较通道1初始化
    
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable); // 使能TIMx在 CCR1 上的预装载寄存器
    TIM_ARRPreloadConfig(TIM4, ENABLE); // 使能预装载寄存器
    
    TIM_Cmd(TIM4, ENABLE); // 使能定时器
}


