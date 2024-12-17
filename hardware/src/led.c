#include "led.h"
#include "pwm.h"

LED_INFO led_info = {LED_OFF};
LED_INFO led2_info = {LED_OFF};
extern u8 bright;

/*******************************************************************************
* �� �� ��         : LED_Init
* ��������		   : LED��ʼ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����ṹ�����
	
	RCC_APB2PeriphClockCmd(LED1_PORT_RCC,ENABLE); //|LED2_PORT_RCC
	
	GPIO_InitStructure.GPIO_Pin=LED1_PIN;  //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //�����������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(LED1_PORT,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */
	LED_Set(LED_OFF);											//��ʼ����ɺ󣬹ر�
	
//	GPIO_InitStructure.GPIO_Pin=LED2_PIN;  //ѡ����Ҫ���õ�IO��
//	GPIO_Init(LED2_PORT,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */
//	GPIO_SetBits(LED2_PORT,LED2_PIN);   //��LED�˿����ߣ�Ϩ������LED
}

void LED_Set(_Bool status)  //���ﲻ��ȡ��
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
			  case 0:
				  TIM_SetCompare2(TIM3,49);  
				  break;
			  case 1:
				  TIM_SetCompare2(TIM3,199);  
				  break;
			  case 2:
				  TIM_SetCompare2(TIM3,359);  
				  break;
			  case 3:
				  TIM_SetCompare2(TIM3,499);  
				default:
            // ������Ч�� speed ֵ
          TIM_SetCompare2(TIM3, 0);
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

