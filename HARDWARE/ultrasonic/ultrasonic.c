#include "stm32f10x.h"
#include "ultrasonic.h"
#include "sys.h"
#include "stm32f10x_rcc.h"
#include "all_head.h"

float distance=4.0;
unsigned int distance_count=0;
char full_flag=0;
char foul_smell=0;
/********************************************************************************************************************************************
@f_name: void Ultran_Init(void)
@brief:	 超声波硬件模块初始化
@param:	 None
@return: None
*********************************************************************************************************************************************/

void Ultran_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能GPIOA

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_5;//发送电平引脚PA5
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_6;		//返回电平引脚
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100M
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //PA6设置成输入，默认下拉	  
 	GPIO_Init(ULTRA_PORT, &GPIO_InitStructure);	
}
/********************************************************************************************************************************************
@f_name: void Ranging(void)
@brief:	 超声波测距，均值5次，实际 测量过程发现测量静态的比较稳定，动态的不稳定，3米范围内比较稳定
@param:	 float *p:测距缓存变量
@return: None
*********************************************************************************************************************************************/
void Ultra_Ranging(float *p)
{
	u8 i;
	u32 j=0;
	char acturlyCount=0;
	float Ultr_Temp=0;	
	*p=0;
	for(i=0;i<5;i++)
	{
		distance_count=0;
		GPIO_SetBits(ULTRA_PORT,ULTRA_TRIG);
		delay_ms(10);
		GPIO_ResetBits(ULTRA_PORT,ULTRA_TRIG);
		TIM2->CNT=0;
		TIM_Cmd(TIM2, ENABLE); //开启时钟
		while(!ECHO_Reci);//等于0的时候等待
		while(ECHO_Reci&&distance_count<15000);//等于1的时候计数
		distance_count=TIM2->CNT;
		
		
		if(distance_count<14997){
			acturlyCount++;//printf("CNT=%d Ultr_Temp=%f\n\n",distance_count,Ultr_Temp);
			Ultr_Temp=Ultr_Temp+(float)170.0*(float)distance_count/1000.0;//模块最大可测距3m
			
		}
		
		TIM_Cmd(TIM2, DISABLE); //关闭时钟
		distance_count=0;
		delay_ms(60);//防止发射信号对回响信号的影响
	}
	*p=Ultr_Temp/acturlyCount;
}

void TIM7_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //时钟使能
	
	//定时器TIM7初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //使能指定的TIM4中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = 55;  //TIM7中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM7, DISABLE);  //使能TIMx				
	
}
/*******************************************************
		
********************************************************/
void TIM7_IRQHandler(void)   //TIM3中断
{
	if(TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)  //检查TIM4更新中断发生与否
		{
			LED=0;
			TIM_ClearITPendingBit(TIM7, TIM_IT_Update  );  //清除TIMx更新中断标志 
		}
}
