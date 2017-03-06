#include "pwm.h"

/***������������TIM4CH4_CAPTURE_STA���㣬�ſ��Կ��� �ڶ��β�***/
//TIM1 PWM���ֳ�ʼ�� PE9--CH1,PE11--CH2,PE13--CH3,PE14--CH4
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void Tim1_PWM_Init(u16 arr,u16 psc)
{
	RCC->APB2ENR|=1<<0;		//TIM1ʱ��ʹ��  
	RCC->AHB1ENR|=1<<4;		//ʹ��PORTEʱ��	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;		//���ù���
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;		//�������
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP; 
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	TIM1->ARR=arr;
	TIM1->PSC=psc;
	TIM1->CCMR1|=6<<4;		//CH1 PWM1ģʽ
	TIM1->CCMR1|=6<<12;		//CH2 PWM1ģʽ
	TIM1->CCMR2|=6<<4;		//CH3 PWM1ģʽ
	TIM1->CCMR2|=6<<12;		//CH4 PWM1ģʽ
	
	TIM1->CCMR1|=1<<3;		//CH1 Ԥװ��ʹ�� 
	TIM1->CCMR1|=1<<11;		//CH2 Ԥװ��ʹ��
	TIM1->CCMR2|=1<<3;		//CH3 Ԥװ��ʹ��
	TIM1->CCMR2|=1<<11;		//CH4 Ԥװ��ʹ��
	
	TIM1->CCER|=1<<0;		//OC1 ���ʹ��
	TIM1->CCER|=1<<4;		//OC2 ���ʹ��
	TIM1->CCER|=1<<8;		//OC3 ���ʹ��
	TIM1->CCER|=1<<12;		//OC4 ���ʹ��
	
	TIM1->CCER|=1<<1;		//OC1 �͵�ƽ��Ч 
	TIM1->CCER|=1<<5;		//OC2 �͵�ƽ��Ч 
	TIM1->CCER|=1<<9;		//OC3 �͵�ƽ��Ч 
	TIM1->CCER|=1<<13;		//OC4 �͵�ƽ��Ч 
	
	TIM1->CR1|=1<<7;    	//ARPEʹ�� //�Զ�����Ԥװ��ʹ��
	TIM1->BDTR|=1<<15;  	//MOEλ�������ʹ��λ
	
	TIM1->CR1|=1<<0;    	//ʹ�ܶ�ʱ��1
	
}

