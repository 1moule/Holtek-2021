#include "main.h"


DHT11_Data_TypeDef DHT11_Data;//������ʪ�ȴ������ṹ��
extern dataPoint_t currentDataPoint;
float Light = 0;
int soil_moisture = 0;
bool water_manual = false;
bool windows_manual = false;
bool light_manual = false;

int i=0;
typedef long(*ArrayPointer)[10];
long arr_light[10] = { 0 };
long compute_arrange(ArrayPointer p);

ArrayPointer p = &arr_light;

UINT32 RX_Task_Handle;
UINT32 TX_Task_Handle;
UINT32 DTH11_Handle;
UINT32 oled_and_data_Handle;
UINT32 light_system_Handle;
UINT32 soil_system_Handle;
UINT32 Processing_data_Handle;

/*
TASK_1
*/
static void Led1Task(void)
{
    while(1) {
        printf("ready1\r\n");
        LOS_TaskDelay(1000);
        printf("ready1\r\n");
        LOS_TaskDelay(1000);
    }
}

static void Led2Task(void)
{
    while(1) {
        printf("ready2\r\n");
        LOS_TaskDelay(200);
        printf("ready2\r\n");
        LOS_TaskDelay(200);
    }
}


static void DHT11_task(void)
{
	printf("dth11\r\n");
    while(1) {
			printf("DHT11 working\r\n");
       		if( Read_DHT11(&DHT11_Data)==SUCCESS)
		{
			printf("\r\n ʪ��:%d,�¶�:%d \r\n" ,DHT11_Data.humi_int,DHT11_Data.temp_int);
			OLED_Num2(16,5,DHT11_Data.humi_int);
			OLED_Num2(16,3,DHT11_Data.temp_int);
     		
		}

        LOS_TaskDelay(300);
    }
}

static void oled_and_data_task(void)
{
	printf("oled\r\n");
    while(1) {
			
			printf("oled2\r\n");
	if (!i2c_CheckDevice(BH1750_Addr))
		{
			Light = LIght_Intensity();        //��ȡ����ǿ��
		}	
		
		soil_moisture=Get_Adc_Average(4,1000);
		OLED_Num2(16,6,soil_moisture);

		arr_light[i]=Light;

		i+=1;

        LOS_TaskDelay(1000);
    }
}


static void light_system_task(void)
{
	printf("light1\r\n");
		while(1)
		{
			printf("light2\r\n");
		if(i==10)
		{
			Light = compute_arrange(p);
			OLED_Num3(16,4,Light);
			
			//light
 		  if(Light<50)//����
			{
				
				if(light_manual==false){
				GPIO_SetBits(GPIOA,GPIO_Pin_3);	//����
				currentDataPoint.valueLight = 1; 	
				}
				
				if(windows_manual==false){
				TIM_SetCompare2(TIM2,190);//���ñȽ�ֵ   ��������
				currentDataPoint.valuewindows = 0;
				}
			}
		
			if(Light>=50&&Light<200)//����
			{
				if(light_manual==false){
				TIM_SetCompare2(TIM2,190);//���ñȽ�ֵ   ������
				currentDataPoint.valuewindows = 0;	
				}
				if(windows_manual==false){
				GPIO_ResetBits(GPIOA,GPIO_Pin_3);//�ص�
				currentDataPoint.valueLight = 0;
				}
			}
			
			if(Light>=200)//��ɹ
			{
				if(light_manual==false){
				TIM_SetCompare2(TIM2,180);//���ñȽ�ֵ   ��������
				currentDataPoint.valuewindows = 1;
				}
				if(windows_manual==false){
				GPIO_ResetBits(GPIOA,GPIO_Pin_3);//�ص�
				currentDataPoint.valueLight = 0;
				}
			}
		
			i=0;
		}
		
			LOS_TaskDelay(1000);
		}
	
}


static void soil_system_task(void)
{
	printf("soil1\r\n");
	while(1){
		printf("soil2\r\n");
if(soil_moisture<35&&water_manual==false)
			{
				TIM_SetCompare3(TIM2,0);  //PA2
				currentDataPoint.valuewater=1;
			}
		
			if(soil_moisture>=35&&water_manual==false)
			{
				TIM_SetCompare3(TIM2,199);   //PA2
				currentDataPoint.valuewater=0;
			}
			LOS_TaskDelay(1000);
		}

}

static void Processing_data_task(void)
{
	printf("data1\r\n");
	while(1){
		printf("data2\r\n");
		
		
userHandle();//�ɼ������ݴ浽�豸״̬�ṹ����
  	gizwitsHandle((dataPoint_t*)&currentDataPoint);//Э�鴦��
		LOS_TaskDelay(500);
	}
}


static UINT32 AppTaskCreate(void)
{
	UINT32 uwRet = LOS_OK;
	
    TSK_INIT_PARAM_S task_init_param;	

	task_init_param.usTaskPrio = 4;
	task_init_param.pcName = "RxTask";
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)Led1Task;
	task_init_param.uwStackSize = 512;	
	uwRet = LOS_TaskCreate(&TX_Task_Handle, &task_init_param);
    if (uwRet != LOS_OK)
    {
        printf("Led1Task create failed,%X\r\n",uwRet);
        return uwRet;
    }
		else 
		{
		printf("led1\r\n");
		}
    
    task_init_param.usTaskPrio = 4;	
	task_init_param.pcName = "TxTask";
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)Led2Task;
	task_init_param.uwStackSize = 512;
	uwRet = LOS_TaskCreate(&TX_Task_Handle, &task_init_param);
    if (uwRet != LOS_OK)
    {
        printf("Led2Task create failed,%X\r\n",uwRet);
        return uwRet;
    } 
		
		//DTU11_task
		    task_init_param.usTaskPrio = 7;	
	task_init_param.pcName = "DTH11";
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)DHT11_task;
	task_init_param.uwStackSize = 512;
	uwRet = LOS_TaskCreate(&DTH11_Handle, &task_init_param);
    if (uwRet == LOS_OK)
    {
        printf("DHT11_task create success,%X\r\n",uwRet);
        
    } 
		    else if (uwRet != LOS_OK)
    {
        printf("DHT11_task create failed,%X\r\n",uwRet);
        return uwRet;
    } 
//		
//		
//		
//		
//				//oled_and_data
//		    task_init_param.usTaskPrio = 7;	
//	task_init_param.pcName = "oled_and_data";
//	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)oled_and_data_task;
//	task_init_param.uwStackSize = 512;
//	uwRet = LOS_TaskCreate(&oled_and_data_Handle, &task_init_param);
//    if (uwRet == LOS_OK)
//    {
//        printf("oled_and_data create success,%X\r\n",uwRet);
//        
//    } 
//		    else if (uwRet != LOS_OK)
//    {
//        printf("oled_and_data create failed,%X\r\n",uwRet);
//        return uwRet;
//    } 
//		
//		
//		
//						//light_system
//		    task_init_param.usTaskPrio = 7;	
//	task_init_param.pcName = "light_system";
//	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)light_system_task;
//	task_init_param.uwStackSize = 512;
//	uwRet = LOS_TaskCreate(&light_system_Handle, &task_init_param);
//    if (uwRet == LOS_OK)
//    {
//        printf("light_system create success,%X\r\n",uwRet);
//       
//    } 
//		    else if (uwRet != LOS_OK)
//    {
//        printf("light_system create failed,%X\r\n",uwRet);
//        return uwRet;
//    } 
//		
//		
//		
//		
//								//soil_system_task
//		    task_init_param.usTaskPrio = 7;	
//	task_init_param.pcName = "soil_system_task";
//	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)soil_system_task;
//	task_init_param.uwStackSize = 512;
//	uwRet = LOS_TaskCreate(&soil_system_Handle, &task_init_param);
//    if (uwRet == LOS_OK)
//    {
//        printf("soil_system_task create success,%X\r\n",uwRet);
//        
//    } 
//		    else if (uwRet != LOS_OK)
//    {
//        printf("soil_system_task create failed,%X\r\n",uwRet);
//        return uwRet;
//    } 
//		
//		
//		
//										//Processing_data_task
//		    task_init_param.usTaskPrio = 7;	
//	task_init_param.pcName = "Processing_data_task";
//	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)Processing_data_task;
//	task_init_param.uwStackSize = 512;
//	uwRet = LOS_TaskCreate(&Processing_data_Handle, &task_init_param);
//    if (uwRet == LOS_OK)
//    {
//        printf("Processing_data_task create success,%X\n",uwRet);
//        
//    } 
//		    else if (uwRet != LOS_OK)
//    {
//        printf("Processing_data_task create failed,%X\n",uwRet);
//        return uwRet;
//    } 
		
		
		
	return LOS_OK;
}


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

void BSP_INIT(void)
{

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
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

}
int main(void)
{
	

	
	
	
	
	 LOS_KernelInit();
		BSP_INIT();
		
			
    UINT32 uwRet = LOS_OK;
		
	uwRet = AppTaskCreate();
	
	printf("test beginning\r\n");
	
	  if(uwRet != LOS_OK) {
      printf("LOS Creat task failed_001\r\n");
		}
		
	LOS_Start();
	
//	while(1)
//	{		

////		Key_Scan1(GPIOA,12);
//		
//		if( Read_DHT11(&DHT11_Data)==SUCCESS)
//		{
//			printf("\r\n ʪ��:%d,�¶�:%d \r\n" ,DHT11_Data.humi_int,DHT11_Data.temp_int);
//			OLED_Num2(16,5,DHT11_Data.humi_int);
//			OLED_Num2(16,3,DHT11_Data.temp_int);
//      delay_ms(100);		
//		}

//		
////			//���ڴ�ӡ��oled���
////	
//		if (!i2c_CheckDevice(BH1750_Addr))
//		{
//			Light = LIght_Intensity();        //��ȡ����ǿ��
//		}	
//		
//		soil_moisture=Get_Adc_Average(4,1000);
//		OLED_Num2(16,6,soil_moisture);

//		arr_light[i]=Light;

//		i+=1;
//		
//		if(i==10)
//		{
//			Light = compute_arrange(p);
//			OLED_Num3(16,4,Light);
//			
//			//light
// 		  if(Light<50)//����
//			{
//				
//				if(light_manual==false){
//				GPIO_SetBits(GPIOA,GPIO_Pin_3);	//����
//				currentDataPoint.valueLight = 1; 	
//				}
//				
//				if(windows_manual==false){
//				TIM_SetCompare2(TIM2,190);//���ñȽ�ֵ   ��������
//				currentDataPoint.valuewindows = 0;
//				}
//			}
//		
//			if(Light>=50&&Light<200)//����
//			{
//				if(light_manual==false){
//				TIM_SetCompare2(TIM2,190);//���ñȽ�ֵ   ������
//				currentDataPoint.valuewindows = 0;	
//				}
//				if(windows_manual==false){
//				GPIO_ResetBits(GPIOA,GPIO_Pin_3);//�ص�
//				currentDataPoint.valueLight = 0;
//				}
//			}
//			
//			if(Light>=200)//��ɹ
//			{
//				if(light_manual==false){
//				TIM_SetCompare2(TIM2,180);//���ñȽ�ֵ   ��������
//				currentDataPoint.valuewindows = 1;
//				}
//				if(windows_manual==false){
//				GPIO_ResetBits(GPIOA,GPIO_Pin_3);//�ص�
//				currentDataPoint.valueLight = 0;
//				}
//			}
//		
//			i=0;
//		}
//		
//		  //soil
//			if(soil_moisture<35&&water_manual==false)
//			{
//				TIM_SetCompare3(TIM2,0);  //PA2
//				currentDataPoint.valuewater=1;
//			}
//		
//			if(soil_moisture>=35&&water_manual==false)
//			{
//				TIM_SetCompare3(TIM2,199);   //PA2
//				currentDataPoint.valuewater=0;
//			}

//		
////		�ⰴ��
//		userHandle();//�ɼ������ݴ浽�豸״̬�ṹ����
//  	gizwitsHandle((dataPoint_t*)&currentDataPoint);//Э�鴦��

//	}
}
		

		
			

