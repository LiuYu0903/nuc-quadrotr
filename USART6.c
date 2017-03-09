#include "USART6.h"

extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
//��ӡ����DR�е�����
void Usart6_SendBuffer(float * rx_buf,uint8_t len)
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

//����6����һ���ֽڣ�c:Ҫ���͵��ַ�
void Usart6_Send_Char(u8 c)
{
	while(USART6->SR == 0); //�ȴ���һ�η������
	USART6->DR = c;
}

//�������ݵ���λ��
//fun:������.0XA0~0XAF; data:���ݻ����������28���ֽ�
//len:data����Ч���ݸ���
void Usart6_Send_To_Shangweiji(u8 fun,u8 * data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len > 28) return;	//���28�ֽ�����
	send_buf[len+3] = 0;	//У��������
	send_buf[0] = 0X88;		//֡ͷ
	send_buf[1] = fun;		//������
	send_buf[2] = len;		//���ݳ���
	for(i = 0;i < len;i++) send_buf[3+i] = data[i];					//��������
	for(i = 0;i < len + 3;i++) send_buf[len+3] += send_buf[i];		//����У���
	for(i = 0;i < len + 4;i++) Usart6_Send_Char(send_buf[i]);		//�������ݵ�����6
}

