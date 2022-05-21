#include "pwm.h"
//PA1,PA2 TIM2CH23

void My_TIM2_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//分频因子
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_Period=arr;//自动重装载值
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;//预分频系数
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCNPolarity=TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	
  GPIO_Init(GPIOA, &GPIO_InitStruct);
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	TIM_OC2Init(TIM2,&TIM_OCInitStruct);//通道2     浇水1   PA1
	TIM_OC3Init(TIM2,&TIM_OCInitStruct);//通道3			浇水2		PA2
	TIM_OC4Init(TIM2,&TIM_OCInitStruct);//通道4			排水		PA3
	
	TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable);        //使能预装载寄存器
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM2,TIM_OCPreload_Enable);
	
  TIM_Cmd(TIM2,ENABLE);        //使能TIM2
	
//  TIM_SetCompare1(TIM2,1500);//设置比较值
//	TIM_SetCompare2(TIM2,1500);//设置比较值
	


}

