#include "timer.h"
#include "all_head.h"
char str[40];
int count=0;
unsigned int timer_counter=0;

void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	//printf("__fileneme:%s,__line:%d",__FILE__,__LINE__);my_delay_ms(200);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	TIM3->CNT=0;
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}
//定时器3中断服务程序,向服务器端发送数据
void TIM3_IRQHandler(void)   //TIM3中断
{
	int left_rongliang=0;
	//printf("__fileneme:%s,__line:%d",__FILE__,__LINE__);my_delay_ms(200);
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
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
				TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除TIMx更新中断标志
				DHT11_Read_Data(&temperature,&humidity);//读取温湿度
				Ultra_Ranging(&distance);
				sprintf(str,"%s;%d;%d;%d;%.2f;%.2f;%s","$HARDWARE",100000+can_number,(int)(distance*2.3),(int)yiwei_flag,(float)temperature,(float)humidity,"\r\n\0");
				yiwei_flag=0;
				upload(str);		
			}
		zwdeng=PAin(5);
		}
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除TIMx更新中断标志
}

/*

20:08:32 收到数据：$HARDWARE;100001;0;0;28.00;59.00;

20:08:47 收到数据：$HARDWARE;100001;0;0;27.00;58.00;

20:08:58 收到数据：$HARDWARE;100001;1;0;28.00;59.00;

*/











