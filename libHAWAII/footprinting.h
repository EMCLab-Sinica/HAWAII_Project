#ifndef HEADERS_FOOTPRINTING_H_
#define HEADERS_FOOTPRINTING_H_

typedef struct{
	_q15 *WEIGHT;
	uint16_t KERNEL_W;
	uint16_t KERNEL_H;
	_q15 *BIAS;
}HAW_PARA;

typedef struct{
	_q15 *DATA;
	uint16_t CH;
	uint16_t W;
	uint16_t H;
}HAW_DATA;

struct _hawnetwork;
typedef struct{
	void (*fun)();
	HAW_DATA DATA_IN;
	HAW_DATA DATA_OUT;
	volatile uint16_t FOOTPRINT;
	volatile uint16_t SUB_FOOTPRINT[2][3];
	HAW_PARA PARA;
	struct _hawnetwork* PARENT;
}HAW_LAYER;

typedef struct _hawnetwork{
	HAW_LAYER *LAYERS;
	volatile uint16_t FOOTPRINT;
	uint16_t TOTAL_LAYERS;
}HAW_NETWORK;

#endif /* HEADERS_FOOTPRINTING_H_ */
