//#ifndef __DHT11_H
//#define __DHT11_H 
//#include "stm32f10x.h"   
////////////////////////////////////////////////////////////////////////////////////	 
//									  
////////////////////////////////////////////////////////////////////////////////////

//#define DHT11_IO    GPIOB
//#define DHT11_PIN   GPIO_Pin_14
////IO��������
//#define DHT11_IO_IN()  {DHT11_IO->CRH&=0XF0FFFFFF;DHT11_IO->CRH|=8<<24;}
//#define DHT11_IO_OUT() {DHT11_IO->CRH&=0XF0FFFFFF;DHT11_IO->CRH|=3<<24;}
//////IO��������											   
//#define	DHT11_DQ_OUT PBout(14) //���ݶ˿�	PA0 
//#define	DHT11_DQ_IN  PBin(14)  //���ݶ˿�	PA0 


//u8 DHT11_Init(void);//��ʼ��DHT11
//u8 DHT11_Read_Data(u8 *temp,u8 *humi);//��ȡ��ʪ��
//u8 DHT11_Read_Byte(void);//����һ���ֽ�
//u8 DHT11_Read_Bit(void);//����һ��λ
//u8 DHT11_Check(void);//����Ƿ����DHT11
//void DHT11_Rst(void);//��λDHT11    
//#endif


#ifndef __DHT11_H
#define	__DHT11_H

#include "stm32f10x.h"
#include "bsp_SysTick.h"
#include "delay.h"

#define HIGH  1
#define LOW   0

#define DHT11_CLK     RCC_APB2Periph_GPIOB
#define DHT11_PIN     GPIO_Pin_6               
#define DHT11_PORT		GPIOB

//���κ꣬��������������һ��ʹ��,����ߵ�ƽ��͵�ƽ
#define DHT11_DATA_OUT(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_6);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_6)
 //��ȡ���ŵĵ�ƽ
#define  DHT11_DATA_IN()	   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)


typedef struct
{
	uint8_t  humi_int;		//ʪ�ȵ���������
	uint8_t  humi_deci;	 	//ʪ�ȵ�С������
	uint8_t  temp_int;	 	//�¶ȵ���������
	uint8_t  temp_deci;	 	//�¶ȵ�С������
	uint8_t  check_sum;	 	//У���
		                 
}DHT11_Data_TypeDef;

void DHT11_GPIO_Config(void);
static void DHT11_Mode_IPU(void);
static void DHT11_Mode_Out_PP(void);
uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data);
static uint8_t Read_Byte(void);

#endif /* __DHT11_H */
