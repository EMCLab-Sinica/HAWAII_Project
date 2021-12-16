
#ifndef HEADERS_NONLINEAR_H_
#define HEADERS_NONLINEAR_H_
void HAW_ACTV(HAW_LAYER* LAYER);
void HAW_POOL(HAW_LAYER* LAYER);

void HAW_ACTV(HAW_LAYER* LAYER){
		uint16_t w,h,c;
		uint16_t temp;
		HAW_DATA *D_IN  = &LAYER->DATA_IN;
		HAW_DATA *D_OUT = &LAYER->DATA_OUT;
		_q15 *D_IN_Ptr = D_IN->DATA;
		_q15 *D_OUT_Ptr = D_OUT->DATA;
		_q15 *LEA_BUFFER_Ptr = LEA_MEMORY;


		c = LAYER->FOOTPRINT / (D_OUT->H * D_OUT->W);
		temp = LAYER->FOOTPRINT % (D_OUT->H * D_OUT->W);
		h = temp / D_OUT->W;
		w = temp % D_OUT->W;

		for(; c < D_IN->CH ; c++){
			//Load data from last attempt
			DMA0CTL = DMADT_1 + DMADSTINCR_3 + DMASRCINCR_3 ;
			DMA0SA = (D_IN_Ptr + c *(D_IN->H * D_IN->W) + h *  D_IN->W + w);
			DMA0DA =(LEA_BUFFER_Ptr + h *  D_IN->W + w);
			DMA0SZ = (D_IN->H * D_IN->W) - h *  D_IN->W + w;
			DMA0CTL |= DMAEN__ENABLE + DMAREQ;
			for(;h<D_OUT->H;h++){
				for(;w<D_OUT->W;w++){
					*(D_OUT_Ptr +
							c * (D_OUT->H * D_OUT->W) +
							h *  D_OUT->W + w)
					= *(LEA_BUFFER_Ptr+ h *  D_IN->W + w) < 0 ? 0 : *(LEA_BUFFER_Ptr+ h *  D_IN->W + w);
					LAYER->FOOTPRINT++;
				}
				w=0;
			}
			h=0;
			temp=0;
		}

}



void HAW_POOL(HAW_LAYER* LAYER){
		uint16_t w,h,c;
		uint16_t k1,k2,ks_w,ks_h;
		uint16_t temp;
		HAW_DATA *D_IN  = &LAYER->DATA_IN;
		HAW_DATA *D_OUT = &LAYER->DATA_OUT;
		HAW_PARA *PARA  = &LAYER->PARA;
		_q15 *D_IN_Ptr = D_IN->DATA;
		_q15 *D_OUT_Ptr = D_OUT->DATA;
		_q15 *LEA_BUFFER_Ptr = LEA_MEMORY;
		uint16_t cnt_t=0;
		ks_w = PARA->KERNEL_W;
		ks_h= PARA->KERNEL_H;


		c = LAYER->FOOTPRINT / (D_OUT->H * D_OUT->W * D_OUT->CH);
		temp = LAYER->FOOTPRINT % (D_OUT->H * D_OUT->W * D_OUT->CH);
		h = temp / D_OUT->W;
		w = temp % D_OUT->W;

		for(; c < D_OUT->CH ; c++){
			for( ;h<D_OUT->H;h++){
				for(;w<D_OUT->W;w++){
					_q15 max=0;
					for(k1 = 0 ; k1 < ks_h ; k1++){
					for(k2 = 0 ; k2 < ks_w ; k2++){
						_q15 t = D_IN_Ptr[c * D_IN->H*D_IN->W + (h * ks_h + k1)*D_IN->W  + w * ks_w + k2  ];
						max = max > t ? max : t;
					}}
					*(D_OUT_Ptr +
					  c * D_OUT->H * D_OUT->W +
					  h * D_OUT->W +
					  w) = max;
					LAYER->FOOTPRINT++;
				}w=0;
			}h=0;
		}

}

#endif /* HEADERS_NONLINEAR_H_ */
