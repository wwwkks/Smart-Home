#include "smoke.h"
#include "delay.h"
#include <math.h>
#include "usart.h"

void Smoke_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);//ADC��λ	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);//��λ����
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //��Ƶ����6ʱ��Ϊ72M/6=12MHz
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//PA8 anolog����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	ADC_DeInit(ADC1);  //��λADC1,������ ADC1��ȫ���Ĵ�������Ϊȱʡֵ
	

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ: ����ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���  

	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
}

//���ADC1ĳ��ͨ����ֵ
//ch:ͨ��ֵ 0~16
//����ֵ:ת�����
u16 Get_ADC1(u8 ch)   
{
	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
} 

#define LSENS_READ_TIMES	10	//���������������ȡ����,����ô���,Ȼ��ȡƽ��ֵ

//��ȡLight Sens��ֵ
//0~100:0,�;100,���� 
u8 Smoke_Get_Val(void)
{
	u32 temp_val=0;
	float voltage = 0 ;
	float R0;
	float RS;
	float ppm;
	float RL = 0.5;//�ɵ�����ֵ����������������
	u8 t;
	for(t=0;t<LSENS_READ_TIMES;t++)
	{
		temp_val+=Get_ADC1(ADC_Channel_5);	//��ȡADCֵ
		DelayMs(5);
	}
	temp_val/=LSENS_READ_TIMES;//�õ�ƽ��ֵ 
	
	voltage = (3.3 / 4096.0) * temp_val; // ��ADCֵת��Ϊ��ѹֵ
	
	UsartPrintf(USART_DEBUG, "vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvoltage:%.2f  \r\n",voltage);

    // ����MQ2�����������ԣ�����ѹֵת��Ϊ����Ũ��ֵ��PPM��
    // ������ʽ��Rs/R0 = 11.5428*ppm^(-0.6549)��
    // Vrl����AO�������ѹ��Vc����·��ѹ��5V����Rl��RlΪ�ɵ����裨500ŷķ��????
    R0 = 6.64; // ������������µ�R0ֵ����Ҫ����ʵ�����У׼
    RS = (5 - voltage) / voltage * RL; // ����Rs
    ppm = pow(11.5428 * R0 / RS, 0.6549) ; // ����PPM
	
	UsartPrintf(USART_DEBUG, "ppppppppppppppppppppppppppppppppppppppppppppm:%.2f  \r\n",ppm);

    return ppm; // ��������Ũ��ֵ
}
