/*
 * File:   main.c
 * Author: gianl
 *
 * Created on 28 novembre 2020, 16.47
 */


#include "xc.h"
#include "string.h"
#include "stdio.h"

#define STATE_DOLLAR (1) 
#define STATE_TYPE (2) 
#define STATE_PAYLOAD (3) 
#define NEW_MESSAGE (1) 
#define NO_MESSAGE (0) 
#define ERR (2) 

char data = 0x0;
int received = 0;

void __attribute__((interrupt, auto_psv)) _U1RXInterrupt( void ) {        
    data = U1RXREG;
    if (U1STAbits.OERR == 1)
        U1STAbits.OERR = 0;
    IFS0bits.U1RXIF = 0;
    received = 1;
}

void init_uart() {
    U1BRG = 11; // (7372800 / 4) / (16 * 9600) - 1
    U1MODEbits.UARTEN = 1; // enable UART
    U1STAbits.UTXEN = 1; // enable U1TX (must be after UARTEN)
    IEC0bits.U1RXIE = 1;
}

unsigned char read_byte() {
    while(!received);
    received = 0;
    return data;
}

void putch(unsigned char data) {
   U1TXREG = data;  
}

typedef struct {
    int state;
    char msg_type[3]; // type is 5 chars + string terminator
    char msg_payload[2]; // assume payload cannot be longer than
    int index_type;
    int index_payload;
} parser_state;

int parse_byte(parser_state* ps, char byte) {
    switch (ps->state) {
        case STATE_DOLLAR:
            if (byte == '$' ) {
                ps->state = STATE_TYPE;
                ps->index_type = 0;
            }
            break;
        case STATE_TYPE:
            if (byte == ',' ) {
                ps->state = STATE_PAYLOAD;
                ps->msg_type[ps->index_type] = '\0';
                ps->index_payload = 0; // initialize properly the index
            } else if (ps->index_type == 2) { // error!
                ps->state = STATE_DOLLAR;
                ps->index_type = 0;
                return ERR;
            } else {
                ps->msg_type[ps->index_type] = byte; // ok!
                ps->index_type++; // increment for the next time;
            }
            break;
        case STATE_PAYLOAD:
            if (byte == '*') {
                ps->state = STATE_DOLLAR; // get ready for a new message
                ps->msg_payload[ps->index_payload] = '\0';
                return NEW_MESSAGE;
            } else if (ps->index_payload == 1) { // error
                ps->state = STATE_DOLLAR;
                ps->index_payload = 0;
                return ERR;
            } else {
                ps->msg_payload[ps->index_payload] = byte; // ok!
                ps->index_payload++; // increment for the next time;
            }
            break;
    }
    return NO_MESSAGE;
}

void manage_error() {
    printf("ERR");
    return;
}

void manage_ok() {
    printf("OR");
    return;
}

int main() {
    init_uart();
    parser_state ps;
    ps.state = STATE_DOLLAR;
    ps.index_type = 0;
    ps.index_payload = 0;
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    LATBbits.LATB0 = 0;
    LATBbits.LATB1 = 0;
    while (1) {
        char byte = read_byte();
        int ret = parse_byte(&ps, byte);
        if (ret == NEW_MESSAGE) {
            if (strcmp(ps.msg_type, "L3") == 0) {
                if (strcmp(ps.msg_payload, "0") == 0) {
                    LATBbits.LATB0 = 0; //TURN OFF
                    manage_ok();
                } else if (strcmp(ps.msg_payload, "1") == 0) {
                    LATBbits.LATB0 = 1; //TURN ON
                    manage_ok();
                } else manage_error();
            } else if (strcmp(ps.msg_type, "L4") == 0) {
                if (strcmp(ps.msg_payload, "0") == 0) {
                    LATBbits.LATB1 = 0; //TURN OFF
                    manage_ok();
                } else if (strcmp(ps.msg_payload, "1") == 0) {
                    LATBbits.LATB1 = 1; //TURN ON
                    manage_ok();
                } else manage_error();
            } else manage_error();
        } else if (ret == NO_MESSAGE) {
            continue;
        } else {
            manage_error();
        }
    }
}