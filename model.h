#ifndef MODEL_H_
#define MODEL_H_
//MAX 0x044000
/* *
 * Input 28x28x1
 * L1: CONV: 20@5x5 24x24x20
 * L2: POOL: 2x2    12x12x20
 * L3: CONV: 40@5x5 8x8x40
 * L4: POOL: 2x2    4x4x40
 * L5: FC  : 640x64
 * L6: FC  : 64x10
 *
 * L1: 5x5x20 = 500
 * L3: 5x5*40 = 1000
 * L5: 640*64 = 40960
 * L6: 64*10  = 640
 * Weight : 84KB
 * */

#include <Parameters/conv1_dummy.h>
#include <Parameters/conv2_dummy.h>
#include"Parameters/fc1.h"

#include"Parameters/fc2.h"

#include"Parameters/_input9_(8).h"

#pragma PERSISTENT(result)
_q15 result[10]={[0 ... 9]=0};

#pragma PERSISTENT(temp_i)
_q15 temp_i[5][784]={[0 ... 4][0 ... 783]=0x1000};

//#pragma location = 0x1A5C
#pragma PERSISTENT(BUFF)
_q15 BUFF[30][576]={[0 ... 29][0 ... 575]=25};


#pragma PERSISTENT(network)
HAW_NETWORK network;

#pragma PERSISTENT(MNIST)
HAW_LAYER MNIST[6]={
		{
			.fun       = HAW_CONV,
			.DATA_IN   = (HAW_DATA){.DATA=(_q15*)input_,.W =28,.H=28,.CH=1},
			.DATA_OUT  = (HAW_DATA){.DATA=(_q15*)BUFF[0],.W =24,.H=24,.CH=20},
			.PARA      = (HAW_PARA){.KERNEL_W=5, .KERNEL_H=5, .WEIGHT = (_q15*)ConV_1, .BIAS = (_q15*)ConV_1_b},
			.FOOTPRINT =0,
			.PARENT = &network,
			.SUB_FOOTPRINT={{28*24,28*24,0},{28*24,28*24,0}}

		},
		{
			.fun       = HAW_POOL,
			.DATA_IN   = (HAW_DATA){.DATA=(_q15*)BUFF[0],.W =24,.H=24,.CH=20},
			.DATA_OUT  = (HAW_DATA){.DATA=(_q15*)BUFF[20],.W =12,.H=12,.CH=20},
			.PARA      = (HAW_PARA){.KERNEL_W=2,.KERNEL_H=2, .WEIGHT = 0, .BIAS = 0},
			.FOOTPRINT =0,
			.PARENT = &network,
			.SUB_FOOTPRINT={{28*24,28*24,0},{28*24,28*24,0}}

		},
		{
			.fun       = HAW_CONV,
			.DATA_IN   = (HAW_DATA){.DATA=(_q15*)BUFF[20],.W =12,.H=12,.CH=20},
			.DATA_OUT  = (HAW_DATA){.DATA=(_q15*)BUFF[0],.W =8,.H=8,.CH=40},
			.PARA      = (HAW_PARA){.KERNEL_W=5, .KERNEL_H=5, .WEIGHT = (_q15*)ConV_2, .BIAS = (_q15*)ConV_2_b},
			.FOOTPRINT =0,
			.PARENT = &network,
			.SUB_FOOTPRINT={{12*8,12*8,0},{12*8,12*8,0}}
		},
		{
			.fun       = HAW_POOL,
			.DATA_IN   = (HAW_DATA){.DATA=(_q15*)BUFF[0],.W =8,.H=8,.CH=40},
			.DATA_OUT  = (HAW_DATA){.DATA=(_q15*)BUFF[20],.W =4,.H=4,.CH=40},
			.PARA      = (HAW_PARA){.KERNEL_W=2,.KERNEL_H=2, .WEIGHT =0, .BIAS = 0},
			.PARENT = &network,
			.SUB_FOOTPRINT={{12*10,12*10,0},{12*10,12*10,0}}
		},
		{
			.fun       = HAW_FC,
			.DATA_IN   = (HAW_DATA){.DATA=(_q15*)BUFF[20],.W =1,.H=1,.CH=640},
			.DATA_OUT  = (HAW_DATA){.DATA=(_q15*)BUFF[0],.W =1,.H=1,.CH=64},
			.PARA      = (HAW_PARA){.WEIGHT = (_q15*)FC_1, .BIAS = (_q15*)FC_1_b},
			.FOOTPRINT =0,
			.PARENT = &network,
			.SUB_FOOTPRINT={{12*10,12*10,0},{12*10,12*10,0}}
		},
		{
			.fun       = HAW_FC,
			.DATA_IN   = (HAW_DATA){.DATA=(_q15*)BUFF[0],.W =1,.H=1,.CH=64},
			.DATA_OUT  = (HAW_DATA){.DATA=(_q15*)result,.W =1,.H=1,.CH=10},
			.PARA      = (HAW_PARA){.WEIGHT = (_q15*)FC_2, .BIAS = (_q15*)FC_2_b},
			.FOOTPRINT =0,
			.PARENT = &network,
			.SUB_FOOTPRINT={{12*10,12*10,0},{12*10,12*10,0}}
		}
};


#pragma PERSISTENT(network)
HAW_NETWORK network={
	.LAYERS 	  = MNIST,
	.TOTAL_LAYERS = 6,
	.FOOTPRINT 	  = 0
};




#endif /* MODEL_H_ */
