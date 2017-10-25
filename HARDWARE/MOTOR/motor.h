#ifndef __MOTOR_H
#define __MOTOR_H

#include "all_head.h"

extern int kaigai;//¿ª¸Ç×´Ì¬±êÖ¾

void TIM1_PWM_Init(void);
static void TIM1_GPIO_Config(void);
static void TIM1_Mode_Config(void);
void hangwai_init(void);
void TIM2_Int_Init(u16 arr,u16 psc);	


#endif



