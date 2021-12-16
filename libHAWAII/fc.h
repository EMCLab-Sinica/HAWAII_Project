
#ifndef HEADERS_FC_H_
#define HEADERS_FC_H_

void HAW_FC(HAW_LAYER* LAYER);

void HAW_FC(HAW_LAYER* LAYER){
	MSP_LEA_MAC_PARAMS *leaParams;
	uint16_t c;
	uint32_t length;
	HAW_DATA *D_IN  = &LAYER->DATA_IN;
	HAW_DATA *D_OUT = &LAYER->DATA_OUT;
	HAW_PARA *PARA  = &LAYER->PARA;
	_q15 *D_IN_Ptr = D_IN->DATA;
	_q15 *D_OUT_Ptr = D_OUT->DATA;
	_q15 *BIAS_Ptr = PARA->BIAS;
	_q15 *WEIGHT_Ptr = PARA->WEIGHT;

	length = D_IN->CH;

	_q15 *LEA_BUFFER_Ptr = LEA_MEMORY;
	_q15 *LEA_BUFFER2_Ptr = (LEA_MEMORY+length);

	// Load Input
	DMA0CTL = DMADT_1 + DMADSTINCR_3 + DMASRCINCR_3 ;
	DMA0SA = D_IN_Ptr;
	DMA0DA = LEA_BUFFER_Ptr;
	DMA0SZ = length;
	DMA0CTL |= DMAEN__ENABLE + DMAREQ + DMAIE;
	__bis_SR_register(GIE+LPM0_bits);
    /* Allocate MSP_LEA_MAC_PARAMS structure. */
    leaParams = (MSP_LEA_MAC_PARAMS *)msp_lea_allocMemory(sizeof(MSP_LEA_MAC_PARAMS)/sizeof(uint32_t));
//    c=0;
	for(c = LAYER->FOOTPRINT ; c < D_OUT->CH ; c++){
		DMA0CTL = DMADT_1 + DMADSTINCR_3 + DMASRCINCR_3 ;
		DMA0SA = (WEIGHT_Ptr + c * length);
		DMA0DA = LEA_BUFFER2_Ptr;
		DMA0SZ = length;
		DMA0CTL |= DMAEN__ENABLE + DMAREQ + DMAIE;
		__bis_SR_register(GIE+LPM0_bits);

	    leaParams->input2 = MSP_LEA_CONVERT_ADDRESS(LEA_BUFFER2_Ptr);
	    leaParams->output = MSP_LEA_CONVERT_ADDRESS(LEA_BUFFER2_Ptr+length);
	    leaParams->vectorSize = length;

	    LEAPMS0 = MSP_LEA_CONVERT_ADDRESS(LEA_BUFFER_Ptr);
	    LEAPMS1 = MSP_LEA_CONVERT_ADDRESS(leaParams);

	    msp_lea_invokeCommand(LEACMD__MAC);

	    _q15 tmp = *(LEA_BUFFER2_Ptr+length) + *(BIAS_Ptr + c );
	    //Activation
	    tmp = tmp >> 2;
	    *(D_OUT_Ptr+c) = tmp < 0 ? 0 : tmp ;

	    LAYER->FOOTPRINT++;

	}

    /* Free MSP_LEA_MAC_PARAMS structure. */
    msp_lea_freeMemory(sizeof(MSP_LEA_MAC_PARAMS)/sizeof(uint32_t));
}




#endif /* HEADERS_FC_H_ */
