#include "gradien.h"
#include "ws2812.h"
#include <math.h>
#include "usart.h"
#include "delay.h"
#include "led.h"

volatile uint8_t red = 0, green = 0, blue = 0;
volatile int8_t step = 1;
extern u8 gramode;



void RGB_LED_Gradient_Start(void)
{
    RGB_LED_Init();
	  gramode = 0;
	  Timer_Config();
	   RGB_LED_Clear();
	LED2 = 1;
}

void RGB_LED_close()
{
	u8 i,j;
	    for (i = 0; i < RGB_LED_XWIDTH; i++)
    {

        for (j = 0; j < RGB_LED_YHIGH; j++)
        {
            RGB_DrawDotColor(i, j, 0, 0);
        }
    }
}


void Timer_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseStructure.TIM_Prescaler = 7199; 
    TIM_TimeBaseStructure.TIM_Period = 9999;//14999;     // 设置周期为 14999，以实现 1.5 秒的定时
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM2, ENABLE);
}

extern u8 last_mode;
u8 last_option,lst2;

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        switch (gramode)
        {
            case 0:
                if (last_mode!=0) // 只有当灯未关闭时才清除
                {
                    RGB_LED_Clear();
                    last_mode = 0; // 设置标志表示灯已关闭
                }
                break;
            case 1:
								if(last_option==0)
								{
								    RGB_LED_Gradient_Lines(); 
									  last_option = 1;
								}else{
								  RGB_LED_Gradient_Horizontal();
									 last_option = 0;
								}
					
								
                last_mode = 1; // 重置标志
                break;
            case 2:
							 if(lst2==0)
							 {
							 		RGB_LED_Blue();
								  lst2 = 1;
							 }else if(lst2==1){

                   RGB_LED_Red();

								  lst2 = 2;
							 }else if(lst2==2){
								 		RGB_LED_Green();
							    lst2 = 0;
							 }
                last_mode = 2; // 重置标志    
                break;
            case 3:
								if(last_option==0)
								{
								    RGB_LED_Gradient_PerPoint();
									  last_option = 1;
								}else{
								  RGB_LED_Gradient_PerPoint2();
									 last_option = 0;
								}						
                last_mode = 3; // 重置标志
						     
                break;
        }

        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  
    }
     LED2 = 1;

}



void RGB_LED_Gradient_Lines(void)
{
    static uint16_t pwm_index = 0;
    uint8_t i,j;

    for (i = 0; i < RGB_LED_XWIDTH; i++)
    {
        // 使用正弦函数生成颜色值
        float angle = (pwm_index + i * 6) * (2 * M_PI / COLOR_CHANGE_PERIOD);
        uint8_t red = (sin(angle) * 127) + 128;
        uint8_t green = (sin(angle + 2 * M_PI / 3) * 127) + 128;
        uint8_t blue = (sin(angle + 4 * M_PI / 3) * 127) + 128;

        uint32_t color = (green << 16) | (red << 8) | blue;

        for (j = 0; j < RGB_LED_YHIGH; j++)
        {
            RGB_DrawDotColor(i, j, 1, color);
        }
    }
}



void RGB_LED_Gradient_Horizontal(void)
{
    static uint16_t pwm_index = 0;
    uint8_t i, j;

    for (i = 0; i < RGB_LED_XWIDTH; i++)
    {
        // 使用正弦函数生成颜色值，确保在 X 轴上渐变
        float angle = (pwm_index + i * 6) * (2 * M_PI / COLOR_CHANGE_PERIOD);
        uint8_t red = (sin(angle) * 127) + 128;
        uint8_t green = (sin(angle + 2 * M_PI / 3) * 127) + 128;
        uint8_t blue = (sin(angle + 4 * M_PI / 3) * 127) + 128;

        uint32_t color = (green << 16) | (red << 8) | blue;

        for (j = 0; j < RGB_LED_YHIGH; j++)
        {
            RGB_DrawDotColor(j, i, 1, color);
        }
    }
}


void RGB_LED_Gradient_PerPoint(void)
{
    static uint16_t pwm_index = 0;
    uint8_t i, j;

    for (i = 0; i < RGB_LED_XWIDTH; i++)
    {
        for (j = 0; j < RGB_LED_YHIGH; j++)
        {
            // 使用正弦函数生成每个点的颜色值
            float angle = (pwm_index + i * 6 + j * 6) * (2 * M_PI / COLOR_CHANGE_PERIOD);
            uint8_t red = (sin(angle) * 127) + 128;
            uint8_t green = (sin(angle + 2 * M_PI / 3) * 127) + 128;
            uint8_t blue = (sin(angle + 4 * M_PI / 3) * 127) + 128;

            uint32_t color = (green << 16) | (red << 8) | blue;

            RGB_DrawDotColor(i, j, 1, color);
        }
    }
		
		
}

void RGB_LED_Gradient_PerPoint2(void)
{
    static uint16_t pwm_index = 0;
    uint8_t i, j;

    for (i = 0; i < RGB_LED_XWIDTH; i++)
    {
        for (j = 0; j < RGB_LED_YHIGH; j++)
        {
            // 使用正弦函数生成每个点的颜色值
            float angle = (pwm_index + i * 6 + j * 6) * (2 * M_PI / COLOR_CHANGE_PERIOD);
            uint8_t green = (sin(angle) * 127) + 128;
            uint8_t blue = (sin(angle + 2 * M_PI / 3) * 127) + 128;
            uint8_t red = (sin(angle + 4 * M_PI / 3) * 127) + 128;

            uint32_t color = (green << 16) | (red << 8) | blue;

            RGB_DrawDotColor(j, i, 1, color);
        }
    }
		
		
}
