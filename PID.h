#ifndef _PID_H
#define _PID_H
//�⻷PID����
struct PID_OUT
{	
	float Kp_x_out;
	float Kp_y_out;
	float Kp_z_out;
	
	float Ki_x_out;
	float Ki_y_out;
	float Ki_z_out;
	
	float Kd_x_out;
	float Kd_y_out;
	float Kd_z_out;
};
//�ڻ�PID����
struct PID_IN
{	
	float Kp_x_in;
	float Kp_y_in;
	float Kp_z_in;
	
	float Ki_x_in;
	float Ki_y_in;
	float Ki_z_in;
	
	float Kd_x_in;
	float Kd_y_in;
	float Kd_z_in;	
};
float expect_X,expect_Y,expect_Z;								//����Ƕ��趨��������ֵ
float AX,AY,AZ;													//����Ƕ�ʵ��ֵ,A����actual-ʵ��
float err_ox,err_oy,err_oz;										//�����⻷��ֵ,o����outside-�⻷
float err_ix,err_iy,err_iz;										//�����ڻ���ֵ,i����in-�ڻ�
float err_last_x_out,err_last_y_out,err_last_z_out;				//�����⻷��һ�ε�ƫ��ֵ
float err_last_x_in,err_last_y_in,err_last_z_in;				//�����ڻ���һ�ε�ƫ��ֵ

float xi_out,yi_out,zi_out;										//�����⻷���֣�i-�������-integral��out-�����⻷
float xi_in,yi_in,zi_in;										//�����ڻ����֣�i-�������-integral��in-�����ڻ�

//���û��ַ����־λ��ʹϵͳ���ڸ��ȶ�	
char flag_x_out = 1,flag_y_out = 1,flag_z_out = 1;				//for �⻷
char flag_x_in  = 1,flag_y_in =  1,flag_z_in = 1;				//for �ڻ�

//�������⻷���
float Out_x_out,Out_y_out,Out_z_out;							//���-x|y|z-�⻷
float Out_x_in,Out_y_in,Out_z_in;								//���-x|y|z-�ڻ�

void Back_To_Middle(void);										//����ƽ���趨ֵ
void PID_Init(void);											//��ʼ��PID����
void PID_Control(float x,float y,float z,float gx,float gy,float gz);	//���⻷PID����

#endif
