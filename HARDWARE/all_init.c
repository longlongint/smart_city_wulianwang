#include "all_head.h"

void all_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	USART2_Init(36,115200);
	TIM3_Int_Init(9999,7199);//10Khz�ļ���Ƶ�ʣ�������10000Ϊ1s 
	LED_Init();
	gprs_init();
}

