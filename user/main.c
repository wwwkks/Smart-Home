/**
	************************************************************
	************************************************************
	************************************************************
	*	文件名： 	main.c
	*
	*	作者： 		张继瑞
	*
	*	日期： 		2017-05-08
	*
	*	版本： 		V1.0
	*
	*	说明： 		接入onenet，上传数据和命令控制
	*
	*	修改记录：	
	************************************************************
	************************************************************
	************************************************************
**/

//单片机头文件
#include "stm32f10x.h"

//网络协议层
#include "onenet.h"

//网络设备
#include "esp8266.h"

//硬件驱动
#include "delay.h"
#include "usart.h"
#include "beep.h"
#include "key.h"
#include "led.h"
#include "dht11.h"
#include "tftlcd.h"
#include "lsens.h"
#include "smoke.h"
#include "pwm.h"
#include "fan_pwm.h"
#include "gradien.h"
#include "ws2812.h"

//C库
#include <string.h>


#define ESP8266_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"

u8 temp,humi,humi_th=100,temp_th=100,lsens=0,ppm=0,lsens_th=99,ppm_th=99; 	
u8 bright = 0,sound =0, speed = 0,gramode =0,last_mode;//led亮度   0 1 2 3 4
u8 thresholds_flag = 0;

 
/*
************************************************************
*	函数名称：	Hardware_Init
*
*	函数功能：	硬件初始化
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		初始化单片机功能以及外接设备
************************************************************
*/
void Hardware_Init(void)
{
	Delay_Init();									//systick初始化
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断控制器分组设置
	
	Usart1_Init(115200);							//串口1，打印信息用
	
	Usart2_Init(115200);							//串口2，驱动ESP8266用
	
	LED_Init();
	
	Beep_Init();									//蜂鸣器初始化
	
	KEY_Init();
	
	Lsens_Init();
	
	Smoke_Init();
	
	//灯亮度
	TIM3_CH2_PWM_Init(500,72-1); //频率是2Kh
	LED_Set(LED_OFF);
	
	//风扇转速
	TIM4_CH1_PWM_Init(500,72-1);
	TIM_SetCompare1(TIM4,0);// 200-300-400-499
	
	//蜂鸣器响度
	TIM4_CH3_PWM_Init(500,72-1);
	TIM_SetCompare3(TIM4, 0);

  RGB_LED_Gradient_Start();
	
	
	TFTLCD_Init();			//LCD初始化
	FRONT_COLOR=BLACK;
	
	
	while(DHT11_Init())	//检测DS18B20是否纯在
	{
		//LCD_ShowString(130,50,tftlcd_data.width,tftlcd_data.height,16,"Error   ");
		UsartPrintf(USART_DEBUG, "DHT11 Check Error!\r\n");
		DelayXms(500);	
		
	}
	
	UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
	
	LCD_ShowString(10,10,tftlcd_data.width,tftlcd_data.height,12,"Hardware init OK...\r\n");
}

void data_pros()	//数据处理函数
{
	u8 temp_buf[3],humi_buf[3];
	//DHT11_Read_Data(&temp,&humi);
	temp_buf[0]=temp/10+0x30;	
	temp_buf[1]=temp%10+0x30;
	temp_buf[2]='\0';
	LCD_ShowString(10,30,tftlcd_data.width,tftlcd_data.height,24,"temperature:");
	LCD_ShowString(170,30,tftlcd_data.width,tftlcd_data.height,24,temp_buf);
	LCD_ShowString(200,30,tftlcd_data.width,tftlcd_data.height,24,"C");
		
	humi_buf[0]=humi/10+0x30;	
	humi_buf[1]=humi%10+0x30;
	humi_buf[2]='\0';
	LCD_ShowString(10,70,tftlcd_data.width,tftlcd_data.height,24,"humidity:");
	LCD_ShowString(170,70,tftlcd_data.width,tftlcd_data.height,24,humi_buf);
	LCD_ShowString(200,70,tftlcd_data.width,tftlcd_data.height,24,"%");
	
	
	humi_buf[0]=lsens/10+0x30;	
	humi_buf[1]=lsens%10+0x30;
	humi_buf[2]='\0';
	LCD_ShowString(10,110,tftlcd_data.width,tftlcd_data.height,24,"lsens:");
	LCD_ShowString(170,110,tftlcd_data.width,tftlcd_data.height,24,humi_buf);
	LCD_ShowString(200,110,tftlcd_data.width,tftlcd_data.height,24,"Lx");
	
	humi_buf[0]=ppm/10+0x30;	
	humi_buf[1]=ppm%10+0x30;
	humi_buf[2]='\0';
	LCD_ShowString(10,150,tftlcd_data.width,tftlcd_data.height,24,"smoke:");
	LCD_ShowString(170,150,tftlcd_data.width,tftlcd_data.height,24,humi_buf);
	LCD_ShowString(200,150,tftlcd_data.width,tftlcd_data.height,24,"ppm");
}

/*
************************************************************
*	函数名称：	main
*
*	函数功能：	
*
*	入口参数：	无
*
*	返回参数：	0
*
*	说明：		
************************************************************
*/


int main(void)
{
	
	unsigned short timeCount = 0;	//发送间隔变量
	int interval = 300; //300*10   12*250
	
		u16 i=0;  
	u8 fx=0;
	
	unsigned char *dataPtr = NULL;
	
	u8 key = 0;
	
	Hardware_Init();				//初始化外围硬件
	
	data_pros();
	
	
	ESP8266_Init();					//初始化ESP8266

	UsartPrintf(USART_DEBUG, "Connect MQTTs Server...\r\n");
	while(ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT"))
		DelayXms(500);
	
	UsartPrintf(USART_DEBUG, "MQTTs Server Connectted \r\n");
	
	while(OneNet_DevLink())			//接入OneNET
		DelayXms(500);
	
	OneNET_Subscribe();    //订阅平台，接收LED控制信息
				
	while(1)
	{	

		if(++timeCount >= interval)									//发送间隔500 * 10 
		{
			DHT11_Read_Data(&temp,&humi);
			lsens = Lsens_Get_Val();
			ppm = Smoke_Get_Val();
			
			UsartPrintf(USART_DEBUG, "temp=%d C  humi=%d %%RH  Lsens=%d lx  ppm=%d \r\n",temp,humi,lsens,ppm);
			UsartPrintf(USART_DEBUG, "%d_____%d_______%d_________%d \r\n",temp_th,humi_th,lsens_th,ppm_th); //调试，打印一下阈值
			
			UsartPrintf(USART_DEBUG, "LED:%d\r\n",led_info.LED_Status); 
			UsartPrintf(USART_DEBUG, "BEEP:%d\r\n",beep_info.Beep_Status); 
			UsartPrintf(USART_DEBUG, "\r\n"); 
			UsartPrintf(USART_DEBUG, "bright:%d\r\n",bright); 
			UsartPrintf(USART_DEBUG, "Sound:%d \r\n",sound); 
			UsartPrintf(USART_DEBUG, "Speed:%d\r\n",speed); 
			UsartPrintf(USART_DEBUG, "Mode:%d\r\n",gramode); 
			UsartPrintf(USART_DEBUG, "++++++++++++++++++++++++++++++++++\r\n"); 
			
			
			//UsartPrintf(USART_DEBUG, "OneNet_SendData\r\n");
			OneNet_SendData();									//发送数据(从stm32到onenet平台)
			
			data_pros();
			
			ESP8266_Clear();
			
			timeCount = 0;
		}
		
		dataPtr = ESP8266_GetIPD(0);
		if(dataPtr != NULL)
		{
				OneNet_RevPro(dataPtr);   //onenet平台到stm32
		}
		
		if(thresholds_flag == 1 && beep_info.Beep_Status==BEEP_OFF)
		{
				if(i==0)  i = 40; //延时10s后才开警告   250ms *
				else  i--;
		}
				
				
					//检测阈值
			if(temp>temp_th || humi>humi_th || lsens>lsens_th || ppm > ppm_th)
			{
					thresholds_flag = 1;
					LED_Set(led_info.LED_Status? LED_ON:LED_OFF);
				interval  =12;
					DelayMs(240);
					if(i==0)  Beep_Set(BEEP_ON);
			}else if(thresholds_flag == 1){
					i = 0;
					thresholds_flag = 0;
					LED_Set(LED_OFF);
					Beep_Set(BEEP_OFF);
				interval  =300;
			}
			
		
		if(led_info.LED_Status == LED_ON)
		{
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
				  break;
		  }
		}
		
		if(beep_info.Beep_Status == BEEP_ON)
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
            // 处理无效的 speed 值
            TIM_SetCompare3(TIM4, 0);
            break;
				}
		}
		
		switch (speed)
    {
        case 0:
            TIM_SetCompare1(TIM4, 0);
            break;
        case 1:
            TIM_SetCompare1(TIM4, 300);
            break;
        case 2:
            TIM_SetCompare1(TIM4, 400);
            break;
        case 3:
            TIM_SetCompare1(TIM4, 499);
            break;
        default:
            // 处理无效的 speed 值
            TIM_SetCompare1(TIM4, 0);
            break;
    }
		
		
		
		if(temp>temp_th) 
		{
				//UsartPrintf(USART_DEBUG, "temp out temp_th\r\n");
			  
		}
		
		if(humi>humi_th) 
 		{
			//UsartPrintf(USART_DEBUG, "humi out humi_th\r\n");
		}
		
		if(lsens>lsens_th) 
		{
				//UsartPrintf(USART_DEBUG, "lsens out lsens_th\r\n");
		}
		
		if(ppm > ppm_th) 
		{
			//UsartPrintf(USART_DEBUG, "ppm out ppm_th\r\n");
		}

		
		DelayMs(10);
	}

}
