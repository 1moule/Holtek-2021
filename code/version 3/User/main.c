#include "main.h"


DHT11_Data_TypeDef DHT11_Data;//������ʪ�ȴ������ṹ��
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
 		  if(Light<50)//����
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_3);	//����
				currentDataPoint.valueLight = 1; 		
				TIM_SetCompare2(TIM2,190);//���ñȽ�ֵ   ��������
				currentDataPoint.valuewindows = 0;
			}
		
			if(Light>=50&&Light<200)//����
			{
				TIM_SetCompare2(TIM2,190);//���ñȽ�ֵ   ������
				currentDataPoint.valuewindows = 0;				
				GPIO_ResetBits(GPIOA,GPIO_Pin_3);//�ص�
				currentDataPoint.valueLight = 0;
			}
			
			if(Light>=200)//��ɹ
			{
				TIM_SetCompare2(TIM2,180);//���ñȽ�ֵ   ��������
				currentDataPoint.valuewindows = 1;
				GPIO_ResetBits(GPIOA,GPIO_Pin_3);//�ص�
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

		
//		�ⰴ��
		userHandle();//�ɼ������ݴ浽�豸״̬�ṹ����
  	gizwitsHandle((dataPoint_t*)&currentDataPoint);//Э�鴦��

	}
}
		

		
			

