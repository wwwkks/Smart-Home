#include "smoke.h"
#include "delay.h"
#include <math.h>
#include "usart.h"

void Smoke_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);//ADC复位	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);//复位结束
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //分频因子6时钟为72M/6=12MHz
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//PA8 anolog输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1的全部寄存器重设为缺省值
	

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式: 独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器  

	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
}

//获得ADC1某个通道的值
//ch:通道值 0~16
//返回值:转换结果
u16 Get_ADC1(u8 ch)   
{
	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
} 

#define LSENS_READ_TIMES	10	//定义光敏传感器读取次数,读这么多次,然后取平均值

//读取Light Sens的值
//0~100:0,最暗;100,最亮 
u8 Smoke_Get_Val(void)
{
	u32 temp_val=0;
	float voltage = 0 ;
	float R0;
	float RS;
	float ppm;
	float RL = 0.5;//可调电阻值
	u8 t;
	for(t=0;t<LSENS_READ_TIMES;t++)
	{
		temp_val+=Get_ADC1(ADC_Channel_5);	//读取ADC值
		DelayMs(5);
	}
	temp_val/=LSENS_READ_TIMES;//得到平均值 
	
	voltage = (3.3 / 4096.0) * temp_val; // 将ADC值转换为电压值
	
    // 根据MQ2传感器的特性，将电压值转换为烟雾浓度值（PPM）
    // 基本公式：Rs/R0 = 11.5428*ppm^(-0.6549)；
    // Vrl：即AO口输出电压，Vc：回路电压（5V），Rl：Rl为可调电阻（500欧姆）????
    R0 = 6.64; // 正常空气情况下的R0值，需要根据实际情况校准
    RS = (5 - voltage) / voltage * RL; // 计算Rs
    ppm = pow(11.5428 * R0 / RS, 0.6549) ; // 计算PPM
	


    return ppm; // 返回烟雾浓度值
}
