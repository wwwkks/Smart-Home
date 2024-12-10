#include "led.h"
#include "pwm.h"

LED_INFO led_info = {LED_OFF};
LED_INFO led2_info = {LED_OFF};
extern u8 bright;

/*******************************************************************************
* 函 数 名         : LED_Init
* 函数功能		   : LED初始化函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义结构体变量
	
	RCC_APB2PeriphClockCmd(LED1_PORT_RCC|LED2_PORT_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=LED1_PIN;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(LED1_PORT,&GPIO_InitStructure); 	   /* 初始化GPIO */
	LED_Set(LED_OFF);											//初始化完成后，关闭
	
	GPIO_InitStructure.GPIO_Pin=LED2_PIN;  //选择你要设置的IO口
	GPIO_Init(LED2_PORT,&GPIO_InitStructure); 	   /* 初始化GPIO */
	GPIO_SetBits(LED2_PORT,LED2_PIN);   //将LED端口拉高，熄灭所有LED
}

void LED_Set(_Bool status)  //这里不用取反
{
	
	//GPIO_WriteBit(GPIOB, GPIO_Pin_5, status == LED_ON ? Bit_RESET : Bit_SET);		
	
	if(status == LED_OFF)   
	{
		TIM_SetCompare2(TIM3,0);
	}
	else
	{
		//TIM_SetCompare2(TIM3,499);
		
		switch(bright)
		{
			case 1:
				TIM_SetCompare2(TIM3,49);  
				break;
			case 2:
				TIM_SetCompare2(TIM3,199);  
				break;
			case 3:
				TIM_SetCompare2(TIM3,359);  
				break;
			case 4:
				TIM_SetCompare2(TIM3,499);  
				break;
		}
	}
	
	led_info.LED_Status = status;
}

void LED2_Set(_Bool status)
{
	
	GPIO_WriteBit(GPIOE, GPIO_Pin_5, status == LED_ON ? Bit_RESET : Bit_SET);		
	
	led2_info.LED_Status = status;

}

