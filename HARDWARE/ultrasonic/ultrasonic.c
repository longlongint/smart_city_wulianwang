#include "stm32f10x.h"
#include "ultrasonic.h"
#include "sys.h"
#include "stm32f10x_rcc.h"
#include "all_head.h"

float distance=4.0;
unsigned int distance_count=0;
char full_flag=0;
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
	u32 j=0;
	char acturlyCount=0;
	float Ultr_Temp=0;	
	*p=0;
	for(i=0;i<5;i++)
	{
		distance_count=0;
		GPIO_SetBits(ULTRA_PORT,ULTRA_TRIG);
		delay_ms(10);
		GPIO_ResetBits(ULTRA_PORT,ULTRA_TRIG);
		TIM2->CNT=0;
		TIM_Cmd(TIM2, ENABLE); //����ʱ��
		while(!ECHO_Reci);//����0��ʱ��ȴ�
		while(ECHO_Reci&&distance_count<15000);//����1��ʱ�����
		distance_count=TIM2->CNT;
		
		
		if(distance_count<14997){
			acturlyCount++;//printf("CNT=%d Ultr_Temp=%f\n\n",distance_count,Ultr_Temp);
			Ultr_Temp=Ultr_Temp+(float)170.0*(float)distance_count/1000.0;//ģ�����ɲ��3m
			
		}
		
		TIM_Cmd(TIM2, DISABLE); //�ر�ʱ��
		distance_count=0;
		delay_ms(60);//��ֹ�����źŶԻ����źŵ�Ӱ��
	}
	*p=Ultr_Temp/acturlyCount;
}

void TIM7_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM7��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM4�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = 55;  //TIM7�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM7, DISABLE);  //ʹ��TIMx				
	
}
/*******************************************************
		
********************************************************/
void TIM7_IRQHandler(void)   //TIM3�ж�
{
	if(TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)  //���TIM4�����жϷ������
		{
			LED=0;
			TIM_ClearITPendingBit(TIM7, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		}
}
