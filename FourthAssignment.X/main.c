/*
 * File:   main.c
 * Author: eraz1
 *
 * Created on 17 ottobre 2020, 11.48
 */


#include "xc.h"
#include <stdio.h>
#include <string.h>

void helloWorld() {
    char str[12];
    sprintf(str, "HELLO WORLD");
    
    SPI1CONbits.MSTEN = 1;
    SPI1CONbits.MODE16 = 0;
    SPI1CONbits.PPRE = 3;
    SPI1CONbits.SPRE = 3;
    SPI1STATbits.SPIEN = 1;
    
    int i;
    for(i = 0; i < 12; i++) {
        while(SPI1STATbits.SPITBF == 1);
        SPI1BUF = str[i];
    }
    while(1);
}

int c = 1;
int done = 0;

void counter() {
    T1CONbits.TCKPS = 2; 
    T1CONbits.TCS = 0;
    PR1 = 15625; // 1 sec
    
    SPI1CONbits.MSTEN = 1;
    SPI1CONbits.MODE16 = 0;
    SPI1CONbits.PPRE = 3;
    SPI1CONbits.SPRE = 3;
    SPI1STATbits.SPIEN = 1;    
    
    char str[17];
    
    IEC0bits.T1IE = 1;
    T1CONbits.TON = 1;
    
    TRISEbits.TRISE8 = 1;
    int oldVal = PORTEbits.RE8;
    int newVal;
    
    while(1) {
        sprintf(str, "%d", c);
        
        while(SPI1STATbits.SPITBF == 1);
        SPI1BUF = 0x80;

        int i;
        for(i = 0; i < strlen(str); i++) {
            while(SPI1STATbits.SPITBF == 1);
            SPI1BUF = str[i];
        }
        for(i = strlen(str); i < 16; i++) {
            while(SPI1STATbits.SPITBF == 1);
            SPI1BUF = ' ';
        }

        while(!done) {
            newVal = PORTEbits.RE8;
            if(!newVal && oldVal) {
                c = 0;
                TMR1 = 0;
            }
            oldVal = newVal;
        }
        done = 0;
        c++;
    }
}

void __attribute__((__interrupt__,__auto_psv__)) _T1Interrupt() {
    IFS0bits.T1IF = 0;
    done = 1;
}



int main(void) {
    counter();
    return 0;
}
