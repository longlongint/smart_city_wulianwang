#include "all_head.h"

int can_number=1;

void all_init(void)
{
	int i=0;
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	ziwaixian_init();PAout(4)=1;//�����ߵƳ�ʼ��
	uart_init(115200);	 		//���ڳ�ʼ��Ϊ115200
	USART2_Init(36,115200); //��wifiģ��ͨ��
	LED_Init();
	TIM1_PWM_Init();
	hangwai_init();
	esp8266_init();
	//esp8266_udppconnect();
	while(DHT11_Init())	//DHT11��ʼ��	
	{
		printf("dht11 init failed!\r\n");my_delay_ms(200);
		my_delay_ms(200);
	}
	Ultran_Init();
	TIM3_Int_Init(9999,7199);//10Khz�ļ���Ƶ�ʣ�������10000Ϊ1s
	TIM2_Int_Init(49999,719);TIM_Cmd(TIM2,DISABLE);//100Khz�ļ���Ƶ�ʣ�������50000Ϊ500ms�������������
	
	mq153_init();
	while(DHT11_Init())	//DHT11��ʼ��	
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

