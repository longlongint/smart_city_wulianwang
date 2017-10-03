#ifndef ALL_HEAD_H
#define ALL_HEAD_H
/*
*	固件库、系统头文件
*/
#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "delay.h"
#include "usart.h"
#include "sys.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
//#include "malloc.h"

/*
*	编写的文件
*/
#include "led.h"
#include "gprs.h"
#include "usart2.h"
#include "timer.h"
#include "motor.h"

void all_init(void);

#endif

