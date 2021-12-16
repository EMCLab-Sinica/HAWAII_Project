#include <msp430.h> 
#include "main.h"

/**
 * main.c
 */
int main(void)
{
	boardSetup();
	msp_lea_init();
	while(1){
		HAW_INFERENCE(&network);


		_DBGUART("\r\n result : %d %d %d %d %d %d %d %d %d %d \r\n",result[0],result[1],result[2],result[3],result[4],
																    result[5],result[6],result[7],result[8],result[9]);
		P1OUT^=0x1;
	}
	
	return 0;
}
