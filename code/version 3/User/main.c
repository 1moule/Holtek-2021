#include "main.h"


DHT11_Data_TypeDef DHT11_Data;//定义温湿度传感器结构体
extern dataPoint_t currentDataPoint;
float Light = 0;
int soil_moisture = 0;

int i=0;
typedef long(*ArrayPointer)[10];
long arr_light[10] = { 0 };

ArrayPointer p = &arr_light;


long compute_arrange(ArrayPointer p)
{
	long sum = 0;
	long average = 0;
	for (int j = 0; j < i; j++)
	{
		sum += ( * p)[j];
	}
	average = sum / 10;
	return average;
}

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
		}	
		
		soil_moisture=Get_Adc_Average(4,1000);
		OLED_Num2(16,6,soil_moisture);

		arr_light[i]=Light;

		i+=1;
		
		if(i==10)
		{
			Light = compute_arrange(p);
			OLED_Num3(16,4,Light);
			
			//light
 		  if(Light<50)//过暗
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_3);	//开灯
				currentDataPoint.valueLight = 1; 		
				TIM_SetCompare2(TIM2,190);//设置比较值   窗户拉开
				currentDataPoint.valuewindows = 0;
			}
		
			if(Light>=50&&Light<200)//适宜
			{
				TIM_SetCompare2(TIM2,190);//设置比较值   窗拉开
				currentDataPoint.valuewindows = 0;				
				GPIO_ResetBits(GPIOA,GPIO_Pin_3);//关灯
				currentDataPoint.valueLight = 0;
			}
			
			if(Light>=200)//过晒
			{
				TIM_SetCompare2(TIM2,180);//设置比较值   窗户拉上
				currentDataPoint.valuewindows = 1;
				GPIO_ResetBits(GPIOA,GPIO_Pin_3);//关灯
				currentDataPoint.valueLight = 0;
			}
		
			i=0;
		}
		
		  //soil
			if(soil_moisture<35)
			{
				TIM_SetCompare3(TIM2,0);  //PA2
				currentDataPoint.valuewater=1;
			}
		
			if(soil_moisture>=35)
			{
				TIM_SetCompare3(TIM2,199);   //PA2
				currentDataPoint.valuewater=0;
			}

		
//		测按键
		userHandle();//采集的数据存到设备状态结构体中
  	gizwitsHandle((dataPoint_t*)&currentDataPoint);//协议处理

	}
}
		

		
			

