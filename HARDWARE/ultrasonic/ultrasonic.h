#ifndef _ultrasonic_H
#define _ultrasonic_H
/*******************************************/
/**************
    超声波测量源程序
	  接口TRig （GPIOA,GPIO_Pin_5）
	  接口ECHO (GPIOA,GPIO_Pin_6)
	          *****************************/
/******************************************/

extern int max_shengdu;

//------------------------------------------------------------移植修改区-----------------------------------------------------------------------
#include "all_head.h"

#define ULTRA_PORT	GPIOB
#define ULTRA_CLK       RCC_APB2Periph_GPIOA
#define ULTRA_TRIG			GPIO_Pin_5
#define ULTRA_ECHO			GPIO_Pin_6

#define TRIG_Send  PBout(5)
#define ECHO_Reci  PBin(6)

//---------------------------------------------------------------------------------------------------------------------------------------------
extern float distance;
extern char full_flag;
extern unsigned int distance_count;

extern char  foul_smell_flag;
void Ultran_Init(void);
void Ultra_Ranging(float *p);



#endif

