/*
 * File:   main.c
 * Author: eraz1
 *
 * Created on 25 ottobre 2020, 19.05
 */

/*
 Per simulare:
 * Proprietà progetto->Simulator->UART2 ecc -> Enable UART2 (Window output)
 * Crea file di testo -> Stimulus -> Register Injection
 *  Il file di testo deve contenere una sequenza di byte in hexcode oppure una stringa tra doppi apici ("TEST")
 *  Se scrivi: wait 10 ms       lui aspetta 10 ms e poi esegue l'istruzione dopo
 */

#include "xc.h"

char data = 0x0;

void __attribute__((interrupt, auto_psv)) _U2RXInterrupt( void )               
{        
    data = U2RXREG;
    
    if (U2STAbits.OERR == 1)
    {
        U2STAbits.OERR = 0;
    }
    IFS1bits.U2RXIF = 0;
}

int main(void) {
    U2BRG = 11; // (7372800 / 4) / (16 * 9600) - 1
    U2MODEbits.UARTEN = 1; // enable UART
    U2STAbits.UTXEN = 1; // enable U1TX (must be after UARTEN)
    IEC1bits.U2RXIE = 1;
    
    SPI1CONbits.MSTEN = 1;
    SPI1CONbits.MODE16 = 0;
    SPI1CONbits.PPRE = 3;
    SPI1CONbits.SPRE = 3;
    SPI1STATbits.SPIEN = 1;    
    
    while(data == 0x0);
    
    while(SPI1STATbits.SPITBF == 1);
    SPI1BUF = data;
    U2TXREG = data;
    
    while(1);
}
