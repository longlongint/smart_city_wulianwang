#ifndef ALL_HEAD_H
#define ALL_HEAD_H
/*
*	�̼��⡢ϵͳͷ�ļ�
*/
#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_tim.h"
#include "delay.h"
#include "usart.h"
#include "sys.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
//#include "malloc.h"

/*
*	��д���ļ�
*/
#include "led.h"
#include "usart2.h"
#include "timer.h"
#include "motor.h"
#include "mq153.h"
#include "dht11.h"
#include "ultrasonic.h"
#include "esp8266.h"

extern int can_number;


void all_init(void);

#endif

