/**
  ******************************************************************************
  * @file    bsp_key.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ����Ӧ��bsp��ɨ��ģʽ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103-�Ե� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "bsp_key.h"  
#include "main.h"

/**
  * @brief  ���ð����õ���I/O��
  * @param  ��
  * @retval ��
  */
void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*���������˿ڵ�ʱ��*/
	RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK|KEY2_GPIO_CLK,ENABLE);
	
	//ѡ�񰴼�������
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN; 
	// ���ð���������Ϊ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	//ʹ�ýṹ���ʼ������
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	
	//ѡ�񰴼�������
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN; 
	//���ð���������Ϊ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	//ʹ�ýṹ���ʼ������
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);	
}

 /*
 * ��������Key_Scan
 * ����  ������Ƿ��а�������
 * ����  ��GPIOx��x ������ A��B��C��D���� E
 *		     GPIO_Pin������ȡ�Ķ˿�λ 	
 * ���  ��KEY_OFF(û���°���)��KEY_ON�����°�����
 */
uint8_t Key_Scan1(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/*����Ƿ��а������� */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
	{	 
		//gizwitsSetMode(WIFI_SOFTAP_MODE);
		GPIO_SetBits(GPIOA,GPIO_Pin_0);
		
		
		/*�ȴ������ͷ� */
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON)
		{
			
		}  
	}
	return 0;
}


//uint8_t Key_Scan2(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,uint16_t num)
//{			
//	/*����Ƿ��а������� */
//	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
//	{	 
//		delay_ms(500);
//		TIM_SetCompare1(TIM3,num);//���ñȽ�ֵ
//		delay_ms(500);
//		TIM_SetCompare2(TIM3,num);//���ñȽ�ֵ
//		/*�ȴ������ͷ� */
//		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);   
//		
//	}
//	
//}
/*********************************************END OF FILE**********************/
