/*
 * File:   main.c
 * Author: eraz1
 *
 * Created on 8 novembre 2020, 17.43
 */


#include "xc.h"

#define TIMER1 1

void tmr_wait_ms(int timer, int ms) {
    // Fosc is MHz -> 5s means 1250K clock cycles > 16bit register (65536 num max)
    switch(timer) {
        case 1:
        T1CONbits.TCKPS = 2; // 1:64 prescaler -> 15625 clock cycles per secondd
        T1CONbits.TCS = 0;
        TMR1 = 0;
        PR1 = (unsigned int)((double)ms * 15.625);
        T1CONbits.TON = 1;
        while(!IFS0bits.T1IF);
        IFS0bits.T1IF = 0;
        break;
        case 2:
        T2CONbits.TCKPS = 2; // 1:64 prescaler -> 15625 clock cycles per second
        T2CONbits.TCS = 0;
        TMR2 = 0;
        PR2 = (unsigned int)((double)ms * 15.625);
        T2CONbits.TON = 1;
        while(!IFS0bits.T2IF);
        IFS0bits.T2IF = 0;
        break;
        case 3:
        T3CONbits.TCKPS = 2; // 1:64 prescaler -> 15625 clock cycles per second
        TMR3 = 0;
        PR3 = (unsigned int)((double)ms * 15.625);
        T3CONbits.TON = 1;
        while(!IFS0bits.T3IF);
        IFS0bits.T3IF = 0;
        break;
    }
}

int main(void) {
    ADCON3bits.ADCS = 8;
    ADCON1bits.ASAM = 0;
    ADCON1bits.SSRC = 0;
    ADCON2bits.CHPS = 0; 
    ADCHSbits.CH0SA = 2;
    ADCHSbits.CH0NA = 0;
    ADPCFG = 0xFFFF;
    ADPCFGbits.PCFG2 = 0;
    ADCON1bits.ADON = 1;
    
    PTCONbits.PTOPS = 0;
    TRISEbits.TRISE3 = 0;
    PORTEbits.RE3 = 0;
    PTCONbits.PTCKPS = 2; // 2;   //1:16 prescaler
    PTCONbits.PTMOD = 0;
    PWMCON1bits.PEN2H = 1;
    PTPER = 1249; //1249;  // 1MHz / (50Hz * 16)) - 1 => 20ms Tpwm
    PTMR = 0;
    PTCONbits.PTEN = 1;
    
    while(1) {
        
        ADCON1bits.SAMP = 1;
        tmr_wait_ms(TIMER1, 1);  
        ADCON1bits.SAMP = 0;
        while(ADCON1bits.DONE == 0);
        ADCON1bits.DONE = 0;
        
        int c = ADCBUF0;
        
        PDC2 = (int)((double)c / 1024.0 * ((double)PTPER / 20.0) + ((double)PTPER / 20.0)) * 2; // 1->2ms
        tmr_wait_ms(TIMER1, 20);  
    } 
}