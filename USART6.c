#include "USART6.h"

extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
//��ӡ����DR�е�����
void usart6_SendBuffer(float * rx_buf,uint8_t len)
{
	u8 t;
	u16 times=0;
	if(USART_RX_STA&0x80)
		{
			len=USART_RX_STA&0x3fff;
			printf("\r\n�����͵���ϢΪ\r\n");
			for(t=0;t<len;t++)
			{
				USART6->DR=USART_RX_BUF[t];
				while((USART6->SR&0X40)==0);//�ȴ����ͽ���
			}
			printf("\r\n\r\n");
			USART_RX_STA=0;
		}else
		{
			times++;
			if(times%200==0)
				printf("���������ݣ��Իس�����");
		}
}
