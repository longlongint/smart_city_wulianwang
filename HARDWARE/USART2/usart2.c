#include "delay.h"
#include "usart2.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	
#include "all_head.h"
//串口发送缓存区 	
__align(8) char USART2_TX_BUF[USART2_MAX_SEND_LEN]; 	//发送缓冲,最大USART2_MAX_SEND_LEN字节 	  
//串口接收缓存区 	
char USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//接收缓冲,最大USART2_MAX_RECV_LEN个字节.
unsigned char backstage=0;

//通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过10ms,则认为不是1次连续数据.也就是超过10ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
u16 USART2_RX_STA=0;  

void USART2_IRQHandler(void)
{
	u8 res;	    
	if(USART2->SR&(1<<5))//接收到数据
	{
		/*
		res=USART2->DR;
		
		if((USART2_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART2_RX_STA&0x4000)//接收到了0x0d
				{
				if(res!=0x0a)USART2_RX_STA=0;//接收错误,重新开始
				else{//接收完成了 
					if(backstage){
						analyse(USART2_RX_BUF);
						USART2_RX_STA=0;	
					}
					else{
						USART2_RX_STA|=0X8000;
					}
				}
				}
			else //还没收到0X0D
				{	
				if(res==0x0d)USART2_RX_STA|=0x4000;
				else
					{
						USART2_RX_BUF[USART2_RX_STA&0X3FFF]=res ;
						USART2_RX_STA++;
						if(USART2_RX_STA>(USART2_MAX_RECV_LEN-1))USART2_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
     }
*/	 
		res=USART2->DR; 			 
		if((USART2_RX_STA&(1<<15))==0)		//还可以接收数据
		{
			TIM4->CNT=0;         					//计数器清空
			if(USART2_RX_STA==0)TIM4_Set(1);	 	//使能定时器4的中断 
			USART2_RX_BUF[USART2_RX_STA++]=res;		//记录接收到的值	 
		}
		else 
		{
			USART2_RX_STA|=1<<15;					//强制标记接收完成
		} 
	}	
}


//初始化IO 串口2
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率	  
void USART2_Init(u32 pclk1,u32 bound)
{  	 		
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC->APB2ENR|=1<<2;   	//使能PORTA口时钟  
	GPIOA->CRL&=0XFFFF00FF;	//IO状态设置
	GPIOA->CRL|=0X00008B00;	//IO状态设置	 
	RCC->APB1ENR|=1<<17;  	//使能串口时钟 	 
	RCC->APB1RSTR|=1<<17;   //复位串口2
	RCC->APB1RSTR&=~(1<<17);//停止复位	   	   
	//波特率设置
 	USART2->BRR=(pclk1*1000000)/(bound);// 波特率设置	 
	USART2->CR1|=0X200C;  	//1位停止,无校验位.
	//使能接收中断 
	USART2->CR1|=1<<5;    	//接收缓冲区非空中断使能	
	#ifdef USART2_RX_EN		  	//如果使能了接收
	//使能接收中断
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断   
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	TIM4_Init(99,7199);		//10ms中断
	USART2_RX_STA=0;		//清零
	TIM4_Set(0);			//关闭定时器4
#endif											  	
}
//串口2,printf 函数
//确保一次发送数据不超过USART2_MAX_SEND_LEN字节
void u2_printf(char* fmt,...)  
{
	u16 i,j; 
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);
	printf("\n           串口2:");
	printf(USART2_TX_BUF);
	i=strlen((const char*)USART2_TX_BUF);		//此次发送数据的长度 
	for(j=0;j<i;j++)							//循环发送数据
	{
		while((USART2->SR&0X40)==0);			//循环发送,直到发送完毕   
		USART2->DR=USART2_TX_BUF[j];  
	}
}
//定时器4中断服务程序		    
void TIM4_IRQHandler(void)
{ 	
	if(TIM4->SR&0X01)//是更新中断
	{	 			   
		USART2_RX_STA|=1<<15;	//标记接收完成
		TIM4->SR&=~(1<<0);		//清除中断标志位	
		//if(backstage)
		//LED=!LED;
		analyse(USART2_RX_BUF);
		TIM4_Set(0);			//关闭TIM4  
	}
}
//设置TIM4的开关
//sta:0，关闭;1,开启;
void TIM4_Set(u8 sta)
{
	if(sta)
	{
    TIM4->CNT=0;         //计数器清空
		TIM4->CR1|=1<<0;     //使能定时器4
	}else TIM4->CR1&=~(1<<0);//关闭定时器4	   
}
//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数		 
void TIM4_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能//TIM4时钟使能    
	
	//定时器TIM4初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM4中断,允许更新中断

	 	  
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
}   
void analyse(char *p){
	//char *str=NULL;
	printf(USART2_RX_BUF);
	
}


















