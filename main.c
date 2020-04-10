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
		P1OUT^=0x1;
	}
	
	return 0;
}
