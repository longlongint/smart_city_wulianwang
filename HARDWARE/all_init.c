#include "all_head.h"

void all_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	delay_init();
	uart_init(115200);	 		//串口初始化为115200
	USART2_Init(36,115200); //与A7模块通信
	TIM2_Int_Init(49999,719);//100Khz的计数频率，计数到50000为500ms，控制开盖时间
	//TIM7_Int_Init(50000,719); //10Khz的计数频率，计数到5000为500ms 
	LED_Init();
	gprs_init();
	TIM1_PWM_Init();
	hangwai_init();
	mq153_init();
	ziwaixian_init();
	while(DHT11_Init())	//DHT11初始化	
	{
		printf("dht11 init failed!\r\n");
		delay_ms(200);
	}
	printf("--------------------\n");
	Ultran_Init();
	TIM_Cmd(TIM3,DISABLE);
	while(1)
	{
		DHT11_Read_Data(&temperature,&humidity);//读取温湿度
		Ultra_Ranging(&distance);
		printf("\r\n温度:%d\n湿度:%d\n距离:%f cm\n",temperature,humidity,distance);
		delay_ms(1000);
	}
}

