#include "csb.h"
#include "usart.h"
#include "delay.h"
u8  TIM4CH1_CAPTURE_STA=0;	//���벶��״̬		    				
u16	TIM4CH1_CAPTURE_VAL;	//���벶��ֵ

//RX-PD14  ����Echo  		TIM4_CH3
//TX-PD15  ���ƶ�trigger 	TIM4_CH4

void Tim4_CH3_Capture_Init(u16 arr,u16 psc)
{
	RCC->AHB1ENR|=1<<3;					//ʹ��PORTD
	RCC->APB1ENR|=1<<2; 				//ʹ��TIM4ʱ��
	GPIO_Set(GPIOD,1<<15,1,0,3,2);      //PD15,��ͨ���������,100Mhz,�������
	GPIO_Set(GPIOD,1<<14,0,0,3,2);		//PD14,���룬 ����
	GPIO_AF_Set(GPIOD,14,2);            //PD14,AF2
	
	TIM4->ARR=arr;
	TIM4->PSC=psc;
	TIM4->CCMR2|=1<<0;     //CC3S=01 	ѡ������� IC3ӳ�䵽TI3��
	TIM4->CCMR2|=0<<4;    //IC3F=0000 ���������˲��� ���˲�
	TIM4->CCMR2|=0<<2;    //IC3PS=00 	���������Ƶ,����Ƶ 
	TIM4->CCER|=0<<9;     //CC3P=0	�����ز���
	TIM4->CCER|=1<<8;     //CC3E=1 	�������������ֵ������Ĵ���(CCR3)��
	TIM4->EGR|=1<<3;	  //������Ʋ��������¼���ʹд��PSC��ֵ������Ч
	TIM4->DIER|=1<<3;	  //������3�ж�
	TIM4->DIER|=1<<0;	  //��������ж�
	TIM4->CR1|=1<<0;	  //ʹ�ܶ�ʱ��4
	
	MY_NVIC_Init(2,0,TIM4_IRQn,2);//��ռ2�������ȼ�0����2

}

//����״̬
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û���񵽵͵�ƽ;1,�Ѿ����񵽵͵�ƽ��.
//[5:0]:����͵�ƽ������Ĵ���(����32λ��ʱ����˵,1us��������1,���ʱ��:4294��)
u8  TIM4CH3_CAPTURE_STA=0;	//���벶��״̬		    				
u32	TIM4CH3_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)
//��ʱ��4�жϷ������	 
void TIM4_IRQHandler(void)
{
	u16 tsr=TIM4->SR;
	if((TIM4CH3_CAPTURE_STA&0x80)==0)	//��δ�ɹ�����
	{
		if(tsr&0x01)//���
		{
			if(TIM4CH3_CAPTURE_STA&0x40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM4CH3_CAPTURE_STA&0x3F)==0x3F)//�ߵ�ƽ̫����
				{
					TIM4CH3_CAPTURE_STA|=0x80;//��ǳɹ�������һ��
					TIM4CH3_CAPTURE_VAL=0xffff;
				}else TIM4CH3_CAPTURE_STA++;
			}
		}
		if(tsr&0x10)//����4���������¼�
		{
			if(TIM4CH3_CAPTURE_STA&0x40) //����һ���½��� 		
			{
				TIM4CH3_CAPTURE_STA|=0x80;  //��ǳɹ�����һ�θߵ�ƽ����
				TIM4CH3_CAPTURE_VAL=TIM4->CCR3;//��ȡ��ǰ�Ĳ���ֵ.
				TIM4->CCER&=~(1<<13);			//CC4P=0 ����Ϊ�����ز���
			}else								//��δ��ʼ,��һ�β���������
				{
					TIM4CH3_CAPTURE_STA=0;		//���
					TIM4CH3_CAPTURE_VAL=0;
					TIM4CH3_CAPTURE_STA|=0x40;	//��ǲ�����������
					TIM4->CR1&=~(1<<0);			//ʧ�ܶ�ʱ��3
					TIM4->CNT=0;				//���������
					TIM4->CCER|=1<<9;			//CC3P=1 ����Ϊ�½��ز���
					TIM4->CR1|=0X01;			//ʹ�ܶ�ʱ��3
				}
		}
	}
	TIM4->SR=0;	//����жϱ�־
}

/*
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
	{
		USART_SendData(USART2,USART_ReceiveData(USART2));
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
	}
	
}
*/
//����6����ʾ����߶ȵ�����
void Csb_Test()
{	
		u32 temp=0;
		float length;//length����Ϊ��������(ʵ��)
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	 while(1)
	 {
		
		GPIO_SetBits(GPIOD,GPIO_Pin_15);//��PB7(TRIG ����)�ܽ��ø�	
		delay_us(20);//���߳���10us,���䳬����
		GPIO_ResetBits(GPIOD,GPIO_Pin_14);
		//�ȴ�ECHO PD14�Ÿߵ�ƽ		
		if(TIM4CH1_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{
			temp=TIM4CH1_CAPTURE_STA&0X3F;
			temp*=65536;					//���ʱ���ܺ�
			temp+=TIM4CH1_CAPTURE_VAL;		//�õ��ܵĸߵ�ƽʱ��
			printf("echo�ߵ�ƽʱ��Ϊ:%d us\r\n",temp);	//��ӡ�ܵĸߵ�ƽʱ��
			
			length=temp*0.0172;
			printf("���Ծ���=%f cm\r\n",length);
			
 			TIM4CH1_CAPTURE_STA=0;			//������һ�β���
			 
 		}
		   
		delay_ms(20);
		
	} 
}