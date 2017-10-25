#include "timer.h"
#include "all_head.h"
char str[40];
int count=0;
unsigned int timer_counter=0;

void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	//printf("__fileneme:%s,__line:%d",__FILE__,__LINE__);my_delay_ms(200);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	TIM3->CNT=0;
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx					 
}
//��ʱ��3�жϷ������,��������˷�������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	int left_rongliang=0;
	//printf("__fileneme:%s,__line:%d",__FILE__,__LINE__);my_delay_ms(200);
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{
			printf("kaigai=%d\n\n",kaigai);
			timer_counter++;			
			if(kaigai==1){
				TIM1->CCR2=240;
				TIM1->CCR3=0;
				delay_ms(1000);
				TIM1->CCR2=0;
				TIM1->CCR3=0;
				kaigai=0;
			}
			else{kaigai--;}
			
			if(1){
				timer_counter=1;
				TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //���TIMx�����жϱ�־
				DHT11_Read_Data(&temperature,&humidity);//��ȡ��ʪ��
				Ultra_Ranging(&distance);
				sprintf(str,"%s;%d;%d;%d;%.2f;%.2f;%s","$HARDWARE",100000+can_number,(int)(distance*2.3),(int)yiwei_flag,(float)temperature,(float)humidity,"\r\n\0");
				yiwei_flag=0;
				upload(str);		
			}
		zwdeng=PAin(5);
		}
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //���TIMx�����жϱ�־
}

/*

20:08:32 �յ����ݣ�$HARDWARE;100001;0;0;28.00;59.00;

20:08:47 �յ����ݣ�$HARDWARE;100001;0;0;27.00;58.00;

20:08:58 �յ����ݣ�$HARDWARE;100001;1;0;28.00;59.00;

*/











