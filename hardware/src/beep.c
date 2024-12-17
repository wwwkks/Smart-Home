/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	led.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2016-11-23
	*
	*	�汾�� 		V1.0
	*
	*	˵���� 		BEEP��ʼ��������
	*
	*	�޸ļ�¼��	
	************************************************************
	************************************************************
	************************************************************
**/

//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//Ӳ������
#include "beep.h"


BEEP_INFO beep_info = {BEEP_OFF};
extern u8 sound;


/*
************************************************************
*	�������ƣ�	Beep_Init
*
*	�������ܣ�	��������ʼ��
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void Beep_Init(void)
{

	GPIO_InitTypeDef gpio_initstruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//��GPIOB��ʱ��
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	//��ֹJTAG����
	
	gpio_initstruct.GPIO_Mode = GPIO_Mode_Out_PP;				//����Ϊ���
	gpio_initstruct.GPIO_Pin = GPIO_Pin_8;						//����ʼ����Pin��
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;				//�ɳ��ص����Ƶ��
	
	GPIO_Init(GPIOB, &gpio_initstruct);							//��ʼ��GPIO
	
	Beep_Set(BEEP_OFF);											//��ʼ����ɺ󣬹رշ�����

}

/*
************************************************************
*	�������ƣ�	Beep_Set
*
*	�������ܣ�	����������
*
*	��ڲ�����	status�����ط�����
*
*	���ز�����	��
*
*	˵����		��-BEEP_ON		��-BEEP_OFF
************************************************************
*/
void Beep_Set(_Bool status)
{
	
	//GPIO_WriteBit(GPIOB, GPIO_Pin_8, status == BEEP_ON ? Bit_SET : Bit_RESET);		//���status����BEEP_ON���򷵻�Bit_SET�����򷵻�Bit_RESET
	
		if(status == BEEP_OFF)   
	{
		TIM_SetCompare3(TIM4, 0);
	}
	else
	{
	      switch(sound)
				{
					case 0:
						TIM_SetCompare3(TIM4, 50); 
						break;
					case 1:
						TIM_SetCompare3(TIM4, 200);  
						break;
					case 2:
						TIM_SetCompare3(TIM4, 300);  
						break;
					case 3:
						TIM_SetCompare3(TIM4, 400);  
						break;
					default:
            // ������Ч�� speed ֵ
            TIM_SetCompare3(TIM4, 0);
            break;
				}
	}
	
	beep_info.Beep_Status = status;

}
