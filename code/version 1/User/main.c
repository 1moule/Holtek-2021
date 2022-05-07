//����ʹ��

//����						PB10 PB11			TX��PB10 	RX��PB11
//��Ļ						PB8 PB9   		SCL��PB8  SDA��PB9
//�¶�ʪ�� 				PB6
//���						PA2 PA3 			���ö�ʱ��2��1��2ͨ��

#include "main.h"  

DHT11_Data_TypeDef DHT11_Data;//������ʪ�ȴ������ṹ��
extern dataPoint_t currentDataPoint;
float Light = 0;
int soil_moisture = 0;
unsigned int light_count = 0,soil_count = 0,loop_count = 0;

//Э���ʼ��
void Gizwits_Init(void)
{	
	gizwitsInit();//��������ʼ��
	TIM3_Int_Init(9,7199);//1MSϵͳ��ʱ
  usart3_init(9600);//WIFI��ʼ��
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//�豸״̬�ṹ���ʼ��
	
}


int main(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  uart_init(115200);//��ʼ������1  PA9,PA10 115200							//
	delay_init();		
	
	OLED_Init();		 //PB8 PB9 																	//���
	OLED_Clear();	
	oled_first_show();
	
	My_TIM2_Init(199,7199);//����Ϊ20ms arr+1)(psc+1)/Tclk  PA6ͨ��2    PA7ͨ��1   ���Ҫ�õ�
		
	Adc_Init();		
	DHT11_GPIO_Config();//PB6 	
																							//���	//
	BH1750_Init();          //��ʼ��BH1750 PA6 PA7									//���
	
	
	//Key_GPIO_Config();   //PA12
	LED_Init();  //PC13

	

	userInit();		
  Gizwits_Init();			//������Э���ʼ��
  gizwitsSetMode(WIFI_SOFTAP_MODE);
		

	
	while(1)
	{		

		//Key_Scan1(GPIOA,12);
		
		if( Read_DHT11(&DHT11_Data)==SUCCESS)
		{
			printf("\r\n ʪ��:%d,�¶�:%d \r\n" ,DHT11_Data.humi_int,DHT11_Data.temp_int);
			OLED_Num2(16,5,DHT11_Data.humi_int);
			OLED_Num2(16,3,DHT11_Data.temp_int);
      delay_ms(100);		
		}

		
//			//���ڴ�ӡ��oled���
//	
		if (!i2c_CheckDevice(BH1750_Addr))
		{
			Light = LIght_Intensity();        //��ȡ����ǿ��
			OLED_Num2(16,4,Light);
		}	
		
		soil_moisture=Get_Adc_Average(4,1000);
		OLED_Num2(16,6,soil_moisture);


//		�ⰴ��
		userHandle();//�ɼ������ݴ浽�豸״̬�ṹ����
  	gizwitsHandle((dataPoint_t*)&currentDataPoint);//Э�鴦��

	 //delay_ms(250);
			//�ð������ƶ����ǰ����Ϊ�������ţ��������������ƶ���Ƕ� ��ȡ190-175
//			195  0��
//			190  45��
//			185  90��
//			180  135��
//			175  180��

		
		
		//�Զ����Ʋ���
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
//		else if(loop_count%10==0 && loop_count-light_count < 2)     //���11�Σ���9����������������/��ˮ
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
		

		
			

