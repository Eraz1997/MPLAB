/*
 * File:   main.c
 * Author: eraz1
 *
 * Created on 26 ottobre 2020, 21.42
 */

#include "xc.h"


int main(void)
{
    DDRB |= 0b00100000;
    // PORTB &= 0b11011111;
    PORTB |= 0b00100000;
    while (1);
}