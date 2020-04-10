#ifndef HEADERS_HAWAII_H_
#define HEADERS_HAWAII_H_
#define LEA_STACK 200


DSPLIB_DATA(LEA_MEMORY,4)
_q15 LEA_MEMORY[2048-LEA_STACK];

_q15 FC_BUFF[900];
#pragma location = 0x1228C
#pragma PERSISTENT(CONV_BUFF)
_q15 CONV_BUFF[25][24][28]={[0 ... 24][0 ... 23][0 ... 27] = 0xffff};



//Footprint monitor (timer version)
typedef struct{
	_q15 *SA;
	_q15 *DA;
	volatile uint16_t* FOOTPRINT;
	uint16_t PERIOD;
	uint16_t BATCH;
}HAW_MONITOR;
HAW_MONITOR MONITOR;


#include "driverlib.h"

#include "DSPLib.h"
#include "libHAWAII/footprinting.h"
#include "libHAWAII/nonlinear.h"
#include "libHAWAII/fc.h"
#include "libHAWAII/convolution.h"
#include <stdlib.h>



void HAW_INFERENCE(HAW_NETWORK *net){

	int i;

	for(i = net->FOOTPRINT ; i < net->TOTAL_LAYERS ; i++){


		HAW_LAYER *LAYER = &net->LAYERS[net->FOOTPRINT];
		HAW_LAYER *NEXT_LAYER = &net->LAYERS[(net->FOOTPRINT+1) % (net->TOTAL_LAYERS)];

		NEXT_LAYER->FOOTPRINT = 0;

		LAYER->fun(LAYER);

		net->FOOTPRINT++;
	}
	net->FOOTPRINT=0;
}

#pragma vector=DMA_VECTOR
__interrupt void DMA_ISR(void)
{
  switch(__even_in_range(DMAIV,16))
  {
    case 0: break;
    case 2:                                 // DMA0IFG = DMA Channel 0
    	DMA0CTL &= ~DMAIFG;
    	__bic_SR_register_on_exit(LPM0_bits);
    	break;
    case 4: break;                          // DMA1IFG = DMA Channel 1
    case 6:                                 // DMA2IFG = DMA Channel 2
//    	DMA3CTL &= ~DMAIFG;
//    	__bic_SR_register_on_exit(LPM0_bits);
    	break;
    case 8:                                // DMA3IFG = DMA Channel 3
		DMA3CTL &= ~DMAIFG;
		__bic_SR_register_on_exit(LPM0_bits);
		break;
    case 10: break;                         // DMA4IFG = DMA Channel 4
    case 12: break;                         // DMA5IFG = DMA Channel 5
    case 14: break;                         // DMA6IFG = DMA Channel 6
    case 16: break;                         // DMA7IFG = DMA Channel 7
    default: break;
  }
}
#pragma vector=TIMER0_A0_VECTOR
__interrupt void MONITOR_ISR(void){
	TA0CCTL0 &= ~CCIFG;
	TA0CTL = MC__STOP ;
	DMA0SA = MONITOR.SA;
	DMA0DA = MONITOR.DA;
	MONITOR.SA += MONITOR.BATCH;
	MONITOR.DA += MONITOR.BATCH;
	DMA0SZ = MONITOR.BATCH;
	RESLO = 0;

	DMA0CTL |= DMAEN__ENABLE + DMAREQ;
	*MONITOR.FOOTPRINT -= MONITOR.BATCH;

	if(!(*MONITOR.FOOTPRINT)){
		TA0CTL = MC__STOP ;
		__bic_SR_register_on_exit(LPM0_bits);
	}
	else{
		MONITOR.BATCH = *MONITOR.FOOTPRINT < MONITOR.BATCH ? *MONITOR.FOOTPRINT : MONITOR.BATCH ;
		MPY = MONITOR.BATCH;
		OP2 = MONITOR.PERIOD;
		TA0CCR0 += RESLO;
		TA0CTL = TASSEL_2 + ID_3 + MC_1;
	}

}

#endif /* HEADERS_HAWAII_H_ */
//}
