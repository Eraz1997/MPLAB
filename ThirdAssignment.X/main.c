/*
 * File:   main.c
 * Author: eraz1
 *
 * Created on 3 ottobre 2020, 15.43
 */


#include "xc.h"

/*
 * NOOOOOOOOOOOOOO
 * VOLEVA:
 * 1. Che usassi D4 INSIEME a D3
 * 2. Che usassi _TXInterrupt come ISR, non un altro interrupt
 *  (usa gli interrupt delle timer flags) 
 */


void __attribute__((__interrupt__,__auto_psv__)) _T2Interrupt() {
    IFS0bits.T2IF = 0;
    LATBbits.LATB1 = !LATBbits.LATB1;
}

void blinkTMR() {
    T1CONbits.TCKPS = 2;
    T1CONbits.TCS = 0;
    TMR1 = 0;
    PR1 = (unsigned int)(15625.0 / 2.0);
    
    T2CONbits.TCKPS = 2;
    T2CONbits.TCS = 0;
    TMR2 = 0;
    PR2 = (unsigned int)(15625.0 / 4.0);
    
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    
    IEC0bits.T2IE = 1;
    T1CONbits.TON = 1;
    T2CONbits.TON = 1;
    
    LATBbits.LATB1 = 1;
    
    while(1) {
        LATBbits.LATB0 = 1;
        IFS0bits.T1IF = 0;
        while(!IFS0bits.T1IF);
        LATBbits.LATB0 = 0;
        IFS0bits.T1IF = 0;
        while(!IFS0bits.T1IF);
    }
}

void __attribute__((__interrupt__,__auto_psv__)) _INT0Interrupt() {
    IFS0bits.INT0IF = 0;
    LATBbits.LATB0 = !LATBbits.LATB0;
    IFS0bits.T1IF = 0;
}

void __attribute__((__interrupt__,__auto_psv__)) _INT1Interrupt() {
    IFS1bits.INT1IF = 0;
    LATBbits.LATB0 = !LATBbits.LATB0;
}

void blinkWithButton() {
    T1CONbits.TCKPS = 1; // 1:64 prescaler -> 3.906,25 clock cycles per second
    T1CONbits.TCS = 0;
    TMR1 = 0;
    PR1 = 500 * 31250 / 1000;
    
    TRISBbits.TRISB0 = 0;
    TRISEbits.TRISE8 = 1;
    T1CONbits.TON = 1;
    LATBbits.LATB0 = 1;
    
    int oldStatus = PORTEbits.RE8;
    int newStatus;
    
    while(1) {
        while(!IFS0bits.T1IF) {
            newStatus = PORTEbits.RE8;
            if(!newStatus && oldStatus) 
                IEC1bits.INT1IE = 1;
            oldStatus = newStatus;
        }
        LATBbits.LATB0 = !LATBbits.LATB0;
        IFS0bits.T1IF = 0;
    }
}

int main(void) {
    blinkTMR();
    return 0;
}
