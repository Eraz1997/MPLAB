/*
 * File:   main.c
 * Author: gianl
 *
 * Created on 1 novembre 2020, 18.01
 */


#include "xc.h"
#include <stdio.h>
#include <string.h>

int main(void) {
    ADCON3bits.ADCS = 8;
    ADCON1bits.ASAM = 0;
    ADCON1bits.SSRC = 0;
    ADCON2bits.CHPS = 1; 
    ADCHSbits.CH0SA = 1;
    ADPCFGbits.PCFG2 = 0;
    ADCON1bits.ADON = 1;
    
    SPI1CONbits.MSTEN = 1;
    SPI1CONbits.MODE16 = 0;
    SPI1CONbits.PPRE = 3;
    SPI1CONbits.SPRE = 3;
    SPI1STATbits.SPIEN = 1;
    char str[12];
    
    
    while(1) {
        int c = ADCBUF2;
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
