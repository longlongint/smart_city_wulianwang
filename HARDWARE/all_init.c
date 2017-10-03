#include "all_head.h"

void all_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	USART2_Init(36,115200);
	TIM2_Int_Init(49999,7199);//10Khz�ļ���Ƶ�ʣ�������50000Ϊ5�����ƿ���ʱ��
	LED_Init();
	gprs_init();
	TIM1_PWM_Init();
	hangwai_init();
}

