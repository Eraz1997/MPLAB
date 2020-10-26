/*
 * File:   main.c
 * Author: eraz1
 *
 * Created on 3 ottobre 2020, 15.43
 */


#include "xc.h"


void tmr_setup_period(int timer, int ms) {
    // Fosc is MHz -> 1s means 250K clock cycles > 16bit register (65536 num max)
    switch(timer) {
        case 1:
        T1CONbits.TCKPS = 2; // 1:64 prescaler -> 15625 clock cycles per second <- LUI E' QUELLO GIUSTO :D
        T1CONbits.TCS = 0;
        TMR1 = 0;
        PR1 = (unsigned int)((double)ms * 15.625);
        break;
        case 2:
        T2CONbits.TCKPS = 2;
        T2CONbits.TCS = 0;
        TMR2 = 0;
        PR2 = (unsigned int)((double)ms * 15.625);
        break;
        case 3:
        T3CONbits.TCKPS = 2; 
        T3CONbits.TCS = 0;
        TMR3 = 0;
        PR3 = (unsigned int)((double)ms * 15.625);
        break;
    }
}

void tmr_wait_period(int timer) {
    switch(timer) {
        case 1:
            TMR1 = 0;
            T1CONbits.TON = 1;
            while(!IFS0bits.T1IF);
            IFS0bits.T1IF = 0;
            break;
        case 2:
            TMR2 = 0;
            T2CONbits.TON = 1;
            while(!IFS0bits.T2IF);
            IFS0bits.T2IF = 0;
            break;
        case 3:
            TMR3 = 0;
            T3CONbits.TON = 1;
            while(!IFS0bits.T3IF);
            IFS0bits.T3IF = 0;
            break;
    }
}

void blinkD31Hz() {
    TRISBbits.TRISB0 = 0;
    tmr_setup_period(1, 500);  // it should be 1000 but simulator can't handle too much
    while(1) {
        LATBbits.LATB0 = 0;
        tmr_wait_period(1);
        LATBbits.LATB0 = 1;
        tmr_wait_period(1);
    }
}

void tmr_wait_ms(int timer, int ms) {
    // Fosc is MHz -> 5s means 1250K clock cycles > 16bit register (65536 num max)
    switch(timer) {
        case 1:
        T1CONbits.TCKPS = 2; // 1:64 prescaler -> 3.906,25 clock cycles per second
        T1CONbits.TCS = 0;
        TMR1 = 0;
        PR1 = ms * 3906 / 1000;
        T1CONbits.TON = 1;
        while(!IFS0bits.T1IF);
        IFS0bits.T1IF = 0;
        break;
        case 2:
        T2CONbits.TCKPS = 2; // 1:64 prescaler -> 3.906,25 clock cycles per second
        T2CONbits.TCS = 0;
        TMR2 = 0;
        PR2 = ms * 3906 / 1000;
        T2CONbits.TON = 1;
        while(!IFS0bits.T2IF);
        IFS0bits.T2IF = 0;
        break;
        case 3:
        T3CONbits.TCKPS = 2; // 1:64 prescaler -> 3.906,25 clock cycles per second
        T3CONbits.TCS = 0;
        TMR3 = 0;
        PR3 = ms * 3906 / 1000;
        T3CONbits.TON = 1;
        while(!IFS0bits.T3IF);
        IFS0bits.T3IF = 0;
        break;
    }
}

void blink1vs5() {
    TRISBbits.TRISB0 = 0;
    while(1) {
        LATBbits.LATB0 = 1;
        tmr_wait_ms(1, 2);  // 1000
        LATBbits.LATB0 = 0;
        tmr_wait_ms(1, 10); // 5000
        LATBbits.LATB0 = 1;
        tmr_wait_ms(1, 1);  // 500
    }
}

void blink2Timers() {
    TRISBbits.TRISB0 = 0;
    tmr_setup_period(1, 500);
    int delay = 250;
    while(1) {
        LATBbits.LATB0 = 1;
        tmr_wait_ms(2, delay);
        LATBbits.LATB0 = 0;
        tmr_wait_period(1);
    }
}

void ledPulse() {
    TRISBbits.TRISB0 = 0;
    TRISEbits.TRISE8 = 1;
    int pulses = 1;
    
    int oldStatus = PORTEbits.RE8;
    int newStatus;

    T1CONbits.TCKPS = 1; // 1:8 prescaler -> 31250 clock cycles per second
    T1CONbits.TCS = 0;
    TMR1 = 0;
    PR1 = 3125; // 100 * 31250 / 1000;
    

    T2CONbits.TCKPS = 2; // 1:64 prescaler -> 3.906,25 clock cycles per second
    T2CONbits.TCS = 0;
    TMR2 = 0;
    PR2 = 3 * 3906;
    
    int exit = 0;
    
    while(!exit) {
        int i;
        for(i = 0; i < pulses; i++) {
            LATBbits.LATB0 = 1;
            PR1 = 3125; // 100 * 31250 / 1000;
            TMR1 = 0;
            T1CONbits.TON = 1;
            while(!IFS0bits.T1IF) {
                
                newStatus = PORTEbits.RE8;
                if(!newStatus && oldStatus) {
                    pulses = pulses == 3 ? 1 : pulses + 1;
                    TMR2 = 0;
                    T2CONbits.TON = 1;
                } else if (newStatus) {
                    T2CONbits.TON = 0;
                    TMR2 = 0;
                }
                
                if(IFS0bits.T2IF) {
                    T2CONbits.TON = 0;
                    T1CONbits.TON = 0;
                    exit = 1;
                    IFS0bits.T2IF = 0;
                    break;
                }
                
                oldStatus = newStatus;
            }
            IFS0bits.T1IF = 0;
            if(exit) {
                break;
            }
            
            LATBbits.LATB0 = 0;
            TMR1 = 0;
            while(!IFS0bits.T1IF) {
                
                newStatus = PORTEbits.RE8;
                if(!newStatus && oldStatus) {
                    pulses = pulses == 3 ? 1 : pulses + 1;
                    TMR2 = 0;
                    T2CONbits.TON = 1;
                } else if (newStatus) {
                    T2CONbits.TON = 0;
                    TMR2 = 0;
                }
                
                if(IFS0bits.T2IF) {
                    T2CONbits.TON = 0;
                    T1CONbits.TON = 0;
                    exit = 1;
                    IFS0bits.T2IF = 0;
                    break;
                }
                
                oldStatus = newStatus;
            }
            IFS0bits.T1IF = 0;
            if(exit) {
                break;
            }
         
        }
        
        TMR1 = 0;
        PR1 = 100 * (10 - pulses * 2) * 31250 / 1000;
        while(!IFS0bits.T1IF) {

            newStatus = PORTEbits.RE8;
            if(!newStatus && oldStatus) {
                pulses = pulses == 3 ? 1 : pulses + 1;
                TMR2 = 0;
                T2CONbits.TON = 1;
            } else if (newStatus) {
                T2CONbits.TON = 0;
                TMR2 = 0;
            }

            if(IFS0bits.T2IF) {
                T2CONbits.TON = 0;
                T1CONbits.TON = 0;
                exit = 1;
                IFS0bits.T2IF = 0;
                break;
            }

            oldStatus = newStatus;
        }
        IFS0bits.T1IF = 0;
    }
    
    while(1) {
        LATBbits.LATB0 = 0;
    }
}

int main(void) {
    blinkD31Hz();
    return 0;
}
