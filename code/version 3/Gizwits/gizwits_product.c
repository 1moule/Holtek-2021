/**
************************************************************
* @file         gizwits_product.c
* @brief        Gizwits control protocol processing, and platform-related       hardware initialization 
* @author       Gizwits
* @date         2017-07-19
* @version      V03030000
* @copyright    Gizwits
* 
* @note         机智云.只为智能硬件而生
*               Gizwits Smart Cloud  for Smart Products
*               链接|增值ֵ|开放|中立|安全|自有|自由|生态
*               www.gizwits.com
*
***********************************************************/

#include <stdio.h>
#include <string.h>
#include "gizwits_product.h"
#include "common.h"
#include "usart3.h"
#include "led.h"
#include "main.h"
//#include "key.h"

static uint32_t timerMsCount;
uint8_t aRxBuffer;

/** User area the current device state structure*/
//用户区当前设备状态结构体
dataPoint_t currentDataPoint;
//extern keysTypedef_t keys;
extern u8 wifi_sta;
extern DHT11_Data_TypeDef DHT11_Data;
extern float Light;

//extern TIM_HandleTypeDef htim2;
//extern UART_HandleTypeDef huart1;
//extern UART_HandleTypeDef huart2;

/**@} */
/**@name Gizwits User Interface
* @{
*/

/**
* @brief Event handling interface

* Description:

* 1. Users can customize the changes in WiFi module status

* 2. Users can add data points in the function of event processing logic, such as calling the relevant hardware peripherals operating interface

* @param [in] info: event queue	事件队列
* @param [in] data: protocol data	协议数据
* @param [in] len: protocol data length	协议数据长度
* @return NULL
* @ref gizwits_protocol.h
*/
//手机指令处理

int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *gizdata, uint32_t len)
{
  uint8_t i = 0;
  dataPoint_t *dataPointPtr = (dataPoint_t *)gizdata;
  moduleStatusInfo_t *wifiData = (moduleStatusInfo_t *)gizdata;
  protocolTime_t *ptime = (protocolTime_t *)gizdata;
  
#if MODULE_TYPE
  gprsInfo_t *gprsInfoData = (gprsInfo_t *)gizdata;
#else
  moduleInfo_t *ptModuleInfo = (moduleInfo_t *)gizdata;
#endif

  if((NULL == info) || (NULL == gizdata))//没有指令或没有数据
  {
    return -1;
  }

  for(i=0; i<info->num; i++)//每次循环对应事件队列中的一个事件
  {
    switch(info->event[i])//事件队列中的第i个事件
    {
			//case...判断出是哪个事件
      case EVENT_Light://app点击补光系统开关
        currentDataPoint.valueLight = dataPointPtr->valueLight;//设备状态结构体中的valuelight值变成手机指令的valuelight的值
        GIZWITS_LOG("Evt: EVENT_Light %d \n", currentDataPoint.valueLight);
			if(0x01 == currentDataPoint.valueLight)//值为1
        {
          //user handle 这里调用自己写的led函数
				  GPIO_SetBits(GPIOA,GPIO_Pin_3);						 //PC13 输出高
				  //symbol_light = LIGHT_ON;

        }
        else
        {
          //user handle  
					GPIO_ResetBits(GPIOA,GPIO_Pin_3);						 //PC13 输出低
					//symbol_light = LIGHT_OFF;

        }
        break;
      case EVENT_water://灌溉系统
        currentDataPoint.valuewater = dataPointPtr->valuewater;
        GIZWITS_LOG("Evt: EVENT_water %d \n", currentDataPoint.valuewater);
        if(0x01 == currentDataPoint.valuewater)
        {
					TIM_SetCompare3(TIM2,150);   //PA2;//设置比较值        开水
          //user handle
        }
        else
        {
					TIM_SetCompare3(TIM2,199);   //PA2);//设置比较值			关水
          //user handle    
        }
        break;

     case EVENT_windows:
        currentDataPoint.valuewindows = dataPointPtr->valuewindows;
        GIZWITS_LOG("Evt:EVENT_windows %d\n",currentDataPoint.valuewindows);
        //user handle
		    if(0x01 == currentDataPoint.valuewindows)
        {
					
					//TIM_SetCompare2(TIM2,186);//设置比较值   开窗
					//delay_ms(200);
					TIM_SetCompare2(TIM2,180);//设置比较值   拉上窗
          //user handle
        }
        else
        {
					
					//TIM_SetCompare2(TIM2,183);//设置比较值   关窗
					//delay_ms(200);
					TIM_SetCompare2(TIM2,190);//设置比较值   拉开窗
          //user handle    
        }
        break;


      case WIFI_SOFTAP:
        break;
      case WIFI_AIRLINK:
        break;
      case WIFI_STATION:
        break;
      case WIFI_CON_ROUTER:
 
        break;
      case WIFI_DISCON_ROUTER:
 
        break;
      case WIFI_CON_M2M:
 
        break;
      case WIFI_DISCON_M2M:
        break;
      case WIFI_RSSI:
        GIZWITS_LOG("RSSI %d\n", wifiData->rssi);
        break;
      case TRANSPARENT_DATA:
        GIZWITS_LOG("TRANSPARENT_DATA \n");
        //user handle , Fetch data from [data] , size is [len]
        break;
      case WIFI_NTP:
        GIZWITS_LOG("WIFI_NTP : [%d-%d-%d %02d:%02d:%02d][%d] \n",ptime->year,ptime->month,ptime->day,ptime->hour,ptime->minute,ptime->second,ptime->ntp);
        break;
      case MODULE_INFO:
            GIZWITS_LOG("MODULE INFO ...\n");
      #if MODULE_TYPE
            GIZWITS_LOG("GPRS MODULE ...\n");
            //Format By gprsInfo_t
      #else
            GIZWITS_LOG("WIF MODULE ...\n");
            //Format By moduleInfo_t
            GIZWITS_LOG("moduleType : [%d] \n",ptModuleInfo->moduleType);
      #endif
    break;
      default:
        break;
    }
  }

  return 0;
}

/**
* User data acquisition

* Here users need to achieve in addition to data points other than the collection of data collection, can be self-defined acquisition frequency and design data filtering algorithm
*这里用户需要实现除数据点采集以外的数据采集，可以自定义采集频率并设计数据滤波算法
* @param none
* @return none
*/

//userhandle函数实现了传感器数据的上发
void userHandle(void)
{
 
    currentDataPoint.valueTemperature = DHT11_Data.temp_int;//Add Sensor Data Collection 温度
	  currentDataPoint.valueHumidity = DHT11_Data.humi_int;//Add Sensor Data Collection 湿度
	  currentDataPoint.valueLightintensity = Light;//Add Sensor Data Collection 光照强度
		currentDataPoint.valuesoil_moisture = soil_moisture;//Add Sensor Data Collection 土壤湿度
		//currentDataPoint.valuewater = ; //灌溉
	  //currentDataPoint.valueLight = symbol_light; //补光
  
}

/**
* Data point initialization function

* In the function to complete the initial user-related data
* @param none
* @return none
* @note The developer can add a data point state initialization value within this function
*/
void userInit(void)
{
    memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));
    
    /** Warning !!! DataPoint Variables Init , Must Within The Data Range **/ 

      currentDataPoint.valueLight = 0;
      currentDataPoint.valuewater = 0;
      currentDataPoint.valueTemperature = 0;
      currentDataPoint.valueHumidity = 0;
      currentDataPoint.valueLightintensity = 0;
			currentDataPoint.valuewindows = 0;
			currentDataPoint.valuesoil_moisture = 0;

}


/**
* @brief Millisecond timing maintenance function, milliseconds increment, overflow to zero

* @param none
* @return none
*/
void gizTimerMs(void)
{
    timerMsCount++;
}

/**
* @brief Read millisecond count

* @param none
* @return millisecond count
*/
uint32_t gizGetTimerCount(void)
{
    return timerMsCount;
}

/**
* @brief MCU reset function

* @param none
* @return none
*/
void mcuRestart(void)
{
    __set_FAULTMASK(1);
    NVIC_SystemReset();
}

/**@} */

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
//PUTCHAR_PROTOTYPE
//{
//  /* Place your implementation of fputc here */
//  /* e.g. write a character to the USART1 and Loop until the end of transmission */
//  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
// 
//  return ch;
//}

/**
  * @brief  Period elapsed callback in non blocking mode 
  * @param  htim : TIM handle
  * @retval None
  */
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	if(htim==&htim2)
//	{
//			keyHandle((keysTypedef_t *)&keys);
//			gizTimerMs();
//	}
//}

/**
* @brief Timer TIM2 init function

* @param none
* @return none
*/
//void timerInit(void)
//{
//	HAL_TIM_Base_Start_IT(&htim2);
//}

/**
  * @brief  This function handles USART IDLE interrupt.
  */
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef*UartHandle)  
//{  
////    if(UartHandle->Instance == USART2)  
////    {  
////				gizPutData((uint8_t *)&aRxBuffer, 1);

////        HAL_UART_Receive_IT(&huart2, (uint8_t *)&aRxBuffer, 1);//开启下一次接收中断  
////    }  
//	
//}  

/**
* @brief USART init function

* Serial communication between WiFi modules and device MCU
* @param none
* @return none
*/
//void uartInit(void)
//{
//	HAL_UART_Receive_IT(&huart2, (uint8_t *)&aRxBuffer, 1);//开启下一次接收中断  
//}

/**
* @brief Serial port write operation, send data to WiFi module //串口写操作，向WiFi模块发送数据,应该就是把传感器数据发送到8266还有命令它配网，
*
* @param buf      : buf address
* @param len      : buf length
*
* @return : Return effective data length;-1，return failure
*/
int32_t uartWrite(uint8_t *buf, uint32_t len)
{
//		uint8_t crc[1] = {0x55};
    uint32_t i = 0;
	
    if(NULL == buf)
    {
        return -1;
    }

    for(i=0; i<len; i++)
    {
//        HAL_UART_Transmit_IT(&huart2, (uint8_t *)&buf[i], 1);
//				while (huart2.gState != HAL_UART_STATE_READY);//Loop until the end of transmission

//        if(i >=2 && buf[i] == 0xFF)
//        {
//						HAL_UART_Transmit_IT(&huart2, (uint8_t *)&crc, 1);
//						while (huart2.gState != HAL_UART_STATE_READY);//Loop until the end of transmission
//        }
				USART_SendData(USART3,buf[i]);
        while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕
        if(i >=2 && buf[i] == 0xFF)
        {
					USART_SendData(USART3,0x55);
          while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕 
        }
    }

#ifdef PROTOCOL_DEBUG
    GIZWITS_LOG("MCU2WiFi[%4d:%4d]: ", gizGetTimerCount(), len);
    for(i=0; i<len; i++)
    {
        GIZWITS_LOG("%02x ", buf[i]);

        if(i >=2 && buf[i] == 0xFF)
        {
            GIZWITS_LOG("%02x ", 0x55);
        }
    }
    GIZWITS_LOG("\n");
#endif
		
		return len;
}  
