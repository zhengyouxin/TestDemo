#include "delay.h"



void delay_ms(unsigned short timeCnt)
{
	unsigned short i,j;
	while(timeCnt--){
	 	for(i=200;i>0;i--){	  	// 1ms
		 	for(j=18;j>0;j--);	// 1us
		}
	}	
}



