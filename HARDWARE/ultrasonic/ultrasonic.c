#include "stm32f10x.h"
#include "ultrasonic.h"
#include "sys.h"
#include "stm32f10x_rcc.h"
#include "all_head.h"

float distance=4.0;
unsigned int distance_count=0;
char full_flag=0;
int max_shengdu=42;
char foul_smell=0;
/********************************************************************************************************************************************
@f_name: void Ultran_Init(void)
@brief:	 ������Ӳ��ģ���ʼ��
@param:	 None
@return: None
*********************************************************************************************************************************************/

void Ultran_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��GPIOA

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_5;//���͵�ƽ����PA5
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_6;		//���ص�ƽ����
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100M
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //PA6���ó����룬Ĭ������	  
 	GPIO_Init(ULTRA_PORT, &GPIO_InitStructure);	
}
/********************************************************************************************************************************************
@f_name: void Ranging(void)
@brief:	 ��������࣬��ֵ5�Σ�ʵ�� �������̷��ֲ�����̬�ıȽ��ȶ�����̬�Ĳ��ȶ���3�׷�Χ�ڱȽ��ȶ�
@param:	 float *p:��໺�����
@return: None
*********************************************************************************************************************************************/
void Ultra_Ranging(float *p)
{
	u8 i;
//	u32 j=0;
	char acturlyCount=0;
	float Ultr_Temp=0;	
	*p=0;
	for(i=0;i<5;i++)
	{
		//printf("Ultra_start\n");
		distance_count=0;
		GPIO_SetBits(ULTRA_PORT,ULTRA_TRIG);
		my_delay_ms(10);
		GPIO_ResetBits(ULTRA_PORT,ULTRA_TRIG);
		
		TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
		TIM_Cmd(TIM2, ENABLE); //����ʱ��
		TIM2->CNT=0;
		while(!ECHO_Reci);//����0��ʱ��ȴ�
		while(ECHO_Reci&&TIM2->CNT<3500);//����1��ʱ�����
		distance_count=TIM2->CNT;
		
		if(distance_count<3497&&distance_count>10){
			acturlyCount++;
			Ultr_Temp=Ultr_Temp+(float)170.0*(float)distance_count/1000.0;//ģ�����ɲ��3m
		}
		TIM_Cmd(TIM2, DISABLE); //�ر�ʱ��
		distance_count=0;
		my_delay_ms(60);//��ֹ�����źŶԻ����źŵ�Ӱ��
	}
	//printf("Ultra_end\n");
	*p=Ultr_Temp/acturlyCount;
}
