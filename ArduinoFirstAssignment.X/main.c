/*
 * File:   main.c
 * Author: eraz1
 *
 * Created on 26 ottobre 2020, 21.42
 */

#include "xc.h"

void exercice1 () {
    DDRB |= 1 << PORTB5;
    PORTB |= 1 << PORTB5;
    while (1);
}

void exercice2 () {
    
    DDRB |= 1 << PORTB5;
    DDRB &= ~(1 << PORTB4);
    MCUCR &= ~(1 << PUD);
    PORTB |= 1 << PORTB4;
    
    while (1) {
        int i = PINB & (1 << PB4);
        if(i) {
            PORTB |= 1 << PORTB5;
        } else {
            PORTB &= ~(1 << PORTB5);
        }
    }
}

void exercice3 () {
    
    DDRB |= 1 << PORTB5;
    DDRB &= ~(1 << PORTB4);
    MCUCR &= ~(1 << PUD);
    PORTB |= 1 << PORTB4;
    
    int status = 0;
    int newVal, oldVal = PINB & (1 << PB4);
    PORTB &= ~(1 << PORTB5);
    
    while (1) {
        newVal = PINB & (1 << PB4);
        if(newVal && !oldVal) {
            status = !status;
        }
        if(status) {
            PORTB |= 1 << PORTB5;
        } else {
            PORTB &= ~(1 << PORTB5);
        }
        oldVal = newVal;
    }
}

int main(void)
{
    exercice3();
}