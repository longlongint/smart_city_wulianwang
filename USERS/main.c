#include "all_head.h"
/*******************************************
	Ӳ����Դ��
		1. PB.12-------->����LED
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





