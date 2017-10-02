#include "all_head.h"

void all_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
	USART2_Init(36,115200);
	TIM3_Int_Init(9999,7199);//10Khz的计数频率，计数到10000为1s 
	LED_Init();
	gprs_init();
}

