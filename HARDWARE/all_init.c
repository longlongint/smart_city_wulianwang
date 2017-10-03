#include "all_head.h"

void all_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	delay_init();
	uart_init(115200);	 //串口初始化为115200
	USART2_Init(36,115200);
	TIM2_Int_Init(49999,7199);//10Khz的计数频率，计数到50000为5，控制开盖时间
	LED_Init();
	gprs_init();
	TIM1_PWM_Init();
	hangwai_init();
}

