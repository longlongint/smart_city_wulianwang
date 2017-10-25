#include "all_head.h"

int can_number=1;

void all_init(void)
{
	int i=0;
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	ziwaixian_init();PAout(4)=1;//紫外线灯初始化
	uart_init(115200);	 		//串口初始化为115200
	USART2_Init(36,115200); //与wifi模块通信
	LED_Init();
	TIM1_PWM_Init();
	hangwai_init();
	esp8266_init();
	//esp8266_udppconnect();
	while(DHT11_Init())	//DHT11初始化	
	{
		printf("dht11 init failed!\r\n");my_delay_ms(200);
		my_delay_ms(200);
	}
	Ultran_Init();
	TIM3_Int_Init(9999,7199);//10Khz的计数频率，计数到10000为1s
	TIM2_Int_Init(49999,719);TIM_Cmd(TIM2,DISABLE);//100Khz的计数频率，计数到50000为500ms，超声波测距用
	
	mq153_init();
	while(DHT11_Init())	//DHT11初始化	
	{
		printf("dht11 init failed!\r\n");my_delay_ms(200);
		my_delay_ms(200);
	}
	printf("--------------------\n");my_delay_ms(100);
	
	while(1)
	{
		if(PAin(7)==0)
		{
			kaigai=5;
		}
		zwdeng=PAin(5);
		my_delay_ms(1);
	}
}

