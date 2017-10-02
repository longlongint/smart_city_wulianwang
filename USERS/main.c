#include "all_head.h"
/*******************************************
	Ó²¼þ×ÊÔ´£º
		1. PB.12-------->°åÔØLED
		2. PA2		------>	A7.U_RXD
			 PA3		------>	A7.U_TXD
		3. 
*******************************************/
int main(void)
{	
	all_init();
	while(1){
		LED=!LED;
		my_delay_ms(1000);
	}
	while(1);
}





