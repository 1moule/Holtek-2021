//引脚使用

//串口						PB10 PB11			TX—PB10 	RX—PB11
//屏幕						PB8 PB9   		SCL—PB8  SDA—PB9
//温度湿度 				PB6
//舵机						PA2 PA3 			复用定时器2，1和2通道

#include "main.h"  

DHT11_Data_TypeDef DHT11_Data;//定义温湿度传感器结构体
extern dataPoint_t currentDataPoint;
float Light = 0;
int soil_moisture = 0;
unsigned int light_count = 0,soil_count = 0,loop_count = 0;

//协议初始化
void Gizwits_Init(void)
{	
	gizwitsInit();//缓冲区初始化
	TIM3_Int_Init(9,7199);//1MS系统定时
  usart3_init(9600);//WIFI初始化
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//设备状态结构体初始化
	
}


int main(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  uart_init(115200);//初始化串口1  PA9,PA10 115200							//
	delay_init();		
	
	OLED_Init();		 //PB8 PB9 																	//完成
	OLED_Clear();	
	oled_first_show();
	
	My_TIM2_Init(199,7199);//周期为20ms arr+1)(psc+1)/Tclk  PA6通道2    PA7通道1   舵机要用的
		
	Adc_Init();		
	DHT11_GPIO_Config();//PB6 	
																							//完成	//
	BH1750_Init();          //初始化BH1750 PA6 PA7									//完成
	
	
	//Key_GPIO_Config();   //PA12
	LED_Init();  //PC13

	

	userInit();		
  Gizwits_Init();			//机智云协议初始化
  gizwitsSetMode(WIFI_SOFTAP_MODE);
		

	
	while(1)
	{		

		//Key_Scan1(GPIOA,12);
		
		if( Read_DHT11(&DHT11_Data)==SUCCESS)
		{
			printf("\r\n 湿度:%d,温度:%d \r\n" ,DHT11_Data.humi_int,DHT11_Data.temp_int);
			OLED_Num2(16,5,DHT11_Data.humi_int);
			OLED_Num2(16,3,DHT11_Data.temp_int);
      delay_ms(100);		
		}

		
//			//串口打印和oled输出
//	
		if (!i2c_CheckDevice(BH1750_Addr))
		{
			Light = LIght_Intensity();        //获取光照强度
			OLED_Num2(16,4,Light);
		}	
		
		soil_moisture=Get_Adc_Average(4,1000);
		OLED_Num2(16,6,soil_moisture);


//		测按键
		userHandle();//采集的数据存到设备状态结构体中
  	gizwitsHandle((dataPoint_t*)&currentDataPoint);//协议处理

	 //delay_ms(250);
			//用按键控制舵机，前两个为案件引脚，第三个参数控制舵机角度 可取190-175
//			195  0度
//			190  45度
//			185  90度
//			180  135度
//			175  180度

		
		
		//自动控制部分
//		
//	  if(Light < 60 )
//		{
//			light_count += 1;
//		}
//		if(soil_moisture < 5)
//		{
//			soil_count += 1;
//		}
//		
//		
//		else if(loop_count%10==0 && loop_count-light_count < 2)     //检测11次，有9次满足条件，补光/补水
//		{
//			GPIO_SetBits(GPIOA,GPIO_Pin_0);
//			light_count = 0;
//		}
//		else if(loop_count%10==0 && loop_count-soil_count < 2)
//		{
//			TIM_SetCompare4(TIM2,185);
//			loop_count = 0;
//			soil_count = 0;
//		}
//		
//		else
//		{
//		  GPIO_ResetBits(GPIOA,GPIO_Pin_0);
//			//TIM_SetCompare4(TIM2,190);
//		}
//		
//	
//		loop_count += 1;
	}
}
		

		
			

