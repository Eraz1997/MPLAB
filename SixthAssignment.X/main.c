/*
 * File:   main.c
 * Author: gianl
 *
 * Created on 1 novembre 2020, 18.01
 */


#include "xc.h"
#include <stdio.h>
#include <string.h>

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

void exercise1() {
    ADCON3bits.ADCS = 8;
    ADCON1bits.ASAM = 0;
    ADCON1bits.SSRC = 0;
    ADCON2bits.CHPS = 0; 
    ADCHSbits.CH0SA = 2;
    ADCHSbits.CH0NA = 0;
    ADPCFG = 0xFFFF;
    ADPCFGbits.PCFG2 = 0;
    ADCON1bits.ADON = 1;
    
    SPI1CONbits.MSTEN = 1;
    SPI1CONbits.MODE16 = 0;
    SPI1CONbits.PPRE = 3;
    SPI1CONbits.SPRE = 3;
    SPI1STATbits.SPIEN = 1;
    char str[12];
    
    
    while(1) {
        
        ADCON1bits.SAMP = 1;
        tmr_wait_ms(TIMER1, 1);  
        ADCON1bits.SAMP = 0;
        while(ADCON1bits.DONE == 0);
        ADCON1bits.DONE = 0;
        
        int c = ADCBUF0;
        double v = ((double)c / 1024.0 * 5.0);
        sprintf(str, "%.1f", v);
        
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
    }
}


void exercise2() {
    ADCON3bits.ADCS = 8;
    ADCON1bits.ASAM = 0;
    ADCON1bits.SSRC = 7;
    ADCON3bits.SAMC = 1;
    ADCON2bits.CHPS = 0; 
    ADCHSbits.CH0SA = 2;
    ADCHSbits.CH0NA = 0;
    ADPCFG = 0xFFFF;
    ADPCFGbits.PCFG2 = 0;
    ADCON1bits.ADON = 1;
    
    SPI1CONbits.MSTEN = 1;
    SPI1CONbits.MODE16 = 0;
    SPI1CONbits.PPRE = 3;
    SPI1CONbits.SPRE = 3;
    SPI1STATbits.SPIEN = 1;
    char str[12];
    
    
    while(1) {
        
        ADCON1bits.SAMP = 1;        
        while(ADCON1bits.DONE == 0);
        ADCON1bits.DONE = 0;
        
        int c = ADCBUF0;
        
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
    }
}

void exercise3() {
    ADCON3bits.ADCS = 8;
    ADCON1bits.ASAM = 1;
    ADCON1bits.SSRC = 7;
    ADCON3bits.SAMC = 1;
    ADCON2bits.CHPS = 1; 
    ADCHSbits.CH0SA = 2;
    ADCHSbits.CH0NA = 0;
    ADCHSbits.CH123SA = 1;
    ADCHSbits.CH123NA = 0;
    ADPCFG = 0xFFFF;
    ADPCFGbits.PCFG2 = 0;
    ADPCFGbits.PCFG3 = 0;
    ADCON1bits.ADON = 1;
    
    SPI1CONbits.MSTEN = 1;
    SPI1CONbits.MODE16 = 0;
    SPI1CONbits.PPRE = 3;
    SPI1CONbits.SPRE = 3;
    SPI1STATbits.SPIEN = 1;
    char str[12];
    
    
    while(1) {
        
        while(ADCON1bits.DONE == 0);
        ADCON1bits.DONE = 0;
        
        int temp = ADCBUF1;
        int bits = ADCBUF0;
        
        sprintf(str, "%d %d", bits, temp);
        
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
    }
}

int main(void) {
    
    tmr_wait_ms(TIMER1, 1000);    
    
    exercise3();
    
    
}
