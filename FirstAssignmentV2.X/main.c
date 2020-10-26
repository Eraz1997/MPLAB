/*
 * File:   main.c
 * Author: eraz1
 *
 * Created on 24 settembre 2020, 21.04
 */


#include "xc.h"
#include <stdio.h>

#define EXCERCISE 1

void excercise1() {
    TRISBbits.TRISB0 = 0;
    LATBbits.LATB0 = 1;
    while(1);
}

void excercise2() {
    TRISBbits.TRISB0 = 0;
    TRISEbits.TRISE8 = 1;
    while(1) {
        if(PORTEbits.RE8 == 1) {
            LATBbits.LATB0 = 0;
        } else {
            LATBbits.LATB0 = 1;
        } 
    }
}

void excercise3() {
    TRISBbits.TRISB0 = 0;
    TRISEbits.TRISE8 = 1;
    int oldVal = PORTEbits.RE8;
    int newVal;
    
    while(1) {
        newVal = PORTEbits.RE8;
        if(!newVal && oldVal) {
            LATBbits.LATB0 = !LATBbits.LATB0;
        }
        oldVal = newVal;
    }
}

int main(void) {
    switch(EXCERCISE) {
        case 1:
            excercise1();
            break;
        case 2:
            excercise2();
            break;
        case 3:
            excercise3();
            break;
    }
    return 0;
}
