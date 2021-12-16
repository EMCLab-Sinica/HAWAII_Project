#ifndef MAIN_H_
#define MAIN_H_

#include <msp430.h>

#include "driverlib.h"
#include "myuart.h"
#include "DSPLib.h"


uint16_t UART_FG=0;
#include "libHAWAII/HAWAII.h"
#include "model.h"

unsigned int FreqLevel = 8;
int uartsetup=0;
volatile uint32_t cycleCount;

int boardSetup(){
    WDTCTL = WDTPW + WDTHOLD;    /* disable watchdog timer  */
    PM5CTL0 &= ~LOCKLPM5;       // Disable the GPIO power-on default high-impedance mode

    P1DIR=0xff;P1OUT=0x00;
    P2DIR=0xff;P2OUT=0x00;
    P3DIR=0xff;P3OUT=0x00;
    P4DIR=0xff;P4OUT=0x00;
    P5DIR=0xff;P5OUT=0x00;
    P6DIR=0xff;P6OUT=0x00;
    P7DIR=0xff;P7OUT=0x00;
    P8DIR=0xff;P8OUT=0x00;
    PADIR=0xff;PAOUT=0x00;
    PBDIR=0xff;PBOUT=0x00;
    PCDIR=0xff;PCOUT=0x00;
    PDDIR=0xff;PDOUT=0x00;

    P8DIR=0xfd;P8REN=GPIO_PIN1;


    // setup uart
    uartsetup=0;
    setFrequency(8);
    CS_initClockSignal( CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1 );
    uartinit();
    /* clock setup */

    // Configure one FRAM waitstate as required by the device datasheet for MCLK
    // operation beyond 8MHz _before_ configuring the clock system.
    FRCTL0 = FRCTLPW | NWAITS_1;

    // Clock System Setup
    CSCTL0_H = CSKEY_H;                     // Unlock CS registers
    CSCTL1 = DCOFSEL_0;                     // Set DCO to 1MHz
    // Set SMCLK = MCLK = DCO, ACLK = VLOCLK
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    // Per Device Errata set divider to 4 before changing frequency to
    // prevent out of spec operation from overshoot transient
//    CSCTL3 = DIVA__4 | DIVS__4 | DIVM__4;   // Set all corresponding clk sources to divide by 4 for errata
    CSCTL1 = DCOFSEL_4 | DCORSEL;           // Set DCO to 16MHz
    // Delay by ~10us to let DCO settle. 60 cycles = 20 cycles buffer + (10us / (1/4MHz))
    __delay_cycles(60);
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;   // Set all dividers to 1 for 16MHz operation
    CSCTL0_H = 0;

    PMM_unlockLPM5();
    _enable_interrupt();
    return 0;
};




#endif /* MAIN_H_ */

