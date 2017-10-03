#include "all_head.h"

void all_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();
	uart_init(115200);	 		//���ڳ�ʼ��Ϊ115200
	USART2_Init(36,115200); //��A7ģ��ͨ��
	TIM2_Int_Init(49999,719);//100Khz�ļ���Ƶ�ʣ�������50000Ϊ500ms�����ƿ���ʱ��
	//TIM7_Int_Init(50000,719); //10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms 
	LED_Init();
	gprs_init();
	TIM1_PWM_Init();
	hangwai_init();
	mq153_init();
	ziwaixian_init();
	while(DHT11_Init())	//DHT11��ʼ��	
	{
		printf("dht11 init failed!\r\n");
		delay_ms(200);
	}
	printf("--------------------\n");
	Ultran_Init();
	TIM_Cmd(TIM3,DISABLE);
	while(1)
	{
		DHT11_Read_Data(&temperature,&humidity);//��ȡ��ʪ��
		Ultra_Ranging(&distance);
		printf("\r\n�¶�:%d\nʪ��:%d\n����:%f cm\n",temperature,humidity,distance);
		delay_ms(1000);
	}
}

