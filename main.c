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


		int i,j;
		_DBGUART("\r\n");
		for(i=0;i<28;i++){
			for(j=0;j<28;j++){
				if(input_[i][j] >0)
					_DBGUART("1");
				else
					_DBGUART("0");
			}
			_DBGUART("\r\n");
		}
		HAW_INFERENCE(&network);


		_DBGUART("\r\n result : %d %d %d %d %d %d %d %d %d %d \r\n\r\n",result[0],result[1],result[2],result[3],result[4],
															    result[5],result[6],result[7],result[8],result[9]);
		j = 0;
		for(i=0;i<10;i++){
			if(result[i]>result[j])j=i;
		}
		_DBGUART("\r\n predict : %d",j);
		P1OUT^=0x1;
	}
	
	return 0;
}
