#include "all_head.h"
#include "motor.h"

int kaigai=0;//����״̬��־


/*���úͳ�ʼ��TIM1 */
void TIM1_PWM_Init(void)
{
	TIM1_GPIO_Config();
	TIM1_Mode_Config();	
}

/* ����TIM1�������PWMʱ�õ���I/O  */
static void TIM1_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* TIM1 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);//ʹ�ܶ�ʱ��1

  /* GPIOA and GPIOB clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/*����TIM1�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ� */
static void TIM1_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM�źŵ�ƽ����ֵ */
	//u16 CCR1_Val = 50000;        
	u16 CCR2_Val = 0;
	u16 CCR3_Val = 0;
	//u16 CCR4_Val = 125;

/* -----------------------------------------------------------------------
    TIM1 Configuration: generate 2 PWM signals with 2 different duty cycles:
    TIM1CLK = 72 MHz, Prescaler = 0x0, TIM1 counter clock = 36 MHz
    TIM3 ARR Register = 50000 => TIM1 Frequency = TIM1 counter clock/(ARR + 1)
    TIM3 Frequency = 1440Hz.
    TIM1 Channel1 duty cycle = (TIM1_CCR1/ TIM1_ARR)* 100 
    TIM1 Channel2 duty cycle = (TIM1_CCR2/ TIM1_ARR)* 100 
    
  ----------------------------------------------------------------------- */

 /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 300;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);


  /* PWM1 Mode configuration: Channel2 */
  
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;//������ͨ��
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;	  

  TIM_OC2Init(TIM1, &TIM_OCInitStructure);

  TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);	


  /* PWM1 Mode configuration: Channel3 */
  
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;//������ͨ��
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;	  

  TIM_OC3Init(TIM1, &TIM_OCInitStructure);

  TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);	

  

  TIM_ARRPreloadConfig(TIM1, ENABLE);

  /* TIM1 enable counter */
  TIM_Cmd(TIM1, ENABLE);
  TIM_CtrlPWMOutputs(TIM1, ENABLE);	
	TIM1->CCR2=0;
	TIM1->CCR3=0;

}
void TIM2_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM2��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM2�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx					 
}
//��ʱ��2�жϷ������
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM2�����жϷ������
		{
			printf("tim2\n");
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־
			//printf("__fileneme:%s,__line:%d",__FILE__,__LINE__);my_delay_ms(200);
			distance_count=16001;
			TIM_Cmd(TIM2,DISABLE); 
			TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);
			//printf("__fileneme:%s,__line:%d",__FILE__,__LINE__);my_delay_ms(200);
		}
}

void hangwai_init(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
 	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��GPIOB�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //LED0-->PB.5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
	GPIO_SetBits(GPIOA,GPIO_Pin_7);
	
  //GPIOA.7 �ж����Լ��жϳ�ʼ������   �½��ش���
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource7);

	EXTI_InitStructure.EXTI_Line=EXTI_Line7;	//KEY2
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	//��ռ���ȼ�2�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); 	
}

void EXTI9_5_IRQHandler(void)
{
	int i=0;
	my_delay_ms(50);//����
	if(EXTI_GetITStatus(EXTI_Line7) != RESET){
		LED=0;
		printf("__fileneme:%s,__line:%d,kaigai=%d",__FILE__,__LINE__,kaigai);my_delay_ms(200);	
			if(kaigai<=0){
			TIM1->CCR2=0;
			TIM1->CCR3=290;
			delay_ms(1000);
			TIM1->CCR2=0;
			TIM1->CCR3=0;
			//my_delay_ms(1000);
		}
		kaigai=4;
		EXTI_ClearITPendingBit(EXTI_Line7);  //���LINE4�ϵ��жϱ�־λ  
	}
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)
	{
		//printf("__fileneme:%s,__line:%d",__FILE__,__LINE__);my_delay_ms(200);
		//LED=0;
		//zwdeng=0;
		//yiwei_flag=1;
		EXTI_ClearITPendingBit(EXTI_Line5);  //���LINE4�ϵ��жϱ�־λ  
	}
}








