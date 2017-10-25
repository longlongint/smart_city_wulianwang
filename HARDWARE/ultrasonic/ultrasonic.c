#include "stm32f10x.h"
#include "ultrasonic.h"
#include "sys.h"
#include "stm32f10x_rcc.h"
#include "all_head.h"

float distance=4.0;
unsigned int distance_count=0;
char full_flag=0;
int max_shengdu=42;
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
//	u32 j=0;
	char acturlyCount=0;
	float Ultr_Temp=0;	
	*p=0;
	for(i=0;i<5;i++)
	{
		//printf("Ultra_start\n");
		distance_count=0;
		GPIO_SetBits(ULTRA_PORT,ULTRA_TRIG);
		my_delay_ms(10);
		GPIO_ResetBits(ULTRA_PORT,ULTRA_TRIG);
		
		TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
		TIM_Cmd(TIM2, ENABLE); //开启时钟
		TIM2->CNT=0;
		while(!ECHO_Reci);//等于0的时候等待
		while(ECHO_Reci&&TIM2->CNT<3500);//等于1的时候计数
		distance_count=TIM2->CNT;
		
		if(distance_count<3497&&distance_count>10){
			acturlyCount++;
			Ultr_Temp=Ultr_Temp+(float)170.0*(float)distance_count/1000.0;//模块最大可测距3m
		}
		TIM_Cmd(TIM2, DISABLE); //关闭时钟
		distance_count=0;
		my_delay_ms(60);//防止发射信号对回响信号的影响
	}
	//printf("Ultra_end\n");
	*p=Ultr_Temp/acturlyCount;
}
