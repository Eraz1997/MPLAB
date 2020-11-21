/*
 * File:   main.c
 * Author: eraz1
 *
 * Created on 21 novembre 2020, 17.08
 */


#include "xc.h"
#include <string.h>

#define MAX_TASKS 3
#define TIMER1 1
#define TIMER2 2
#define TIMER3 3
#define HEARTBEAT 5

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

typedef struct {
    char * data;
    int data_length;
    int index;
    int slider_index;
    int slider_on;
} lcd_data;

lcd_data lcd;

void init_devices() {
    // init led (output pin)
    TRISBbits.TRISB0 = 0;
    LATBbits.LATB0 = 0;
    
    // init button
    TRISEbits.TRISE8 = 1;
    
    // init spi for lcd display :) 
    SPI1CONbits.MSTEN = 1;
    SPI1CONbits.MODE16 = 0;
    SPI1CONbits.PPRE = 3;
    SPI1CONbits.SPRE = 3;
    SPI1STATbits.SPIEN = 1;
    
    lcd.data = "               This is a very long string";
    lcd.data_length = strlen(lcd.data);
    lcd.index = 0;
    lcd.slider_index = 15;
    lcd.slider_on = 1;
    
    // init potentiometer
    ADCON3bits.ADCS = 8;
    ADCON1bits.ASAM = 1;
    ADCON1bits.SSRC = 7;
    ADCON3bits.SAMC = 1;
    ADCON2bits.CHPS = 0; 
    ADCHSbits.CH0SA = 2;
    ADCHSbits.CH0NA = 0;
    ADPCFG = 0xFFFF;
    ADPCFGbits.PCFG2 = 0;
    ADCON1bits.ADON = 1;
}

void task1_write_character() {
    while(SPI1STATbits.SPITBF == 1);
    SPI1BUF = 0x80 + lcd.index;
    
    while(SPI1STATbits.SPITBF == 1);
    SPI1BUF = lcd.data[(lcd.index + lcd.slider_index) % lcd.data_length];
    
    lcd.index = lcd.index == 15 ? 0 : lcd.index + 1;
}

void task2_slide_controller() {
    if(!lcd.slider_on) 
        return;
    lcd.slider_index = lcd.slider_index == lcd.data_length ? 0 : lcd.slider_index + 1; 
}

void task3_blink_led() {
    LATBbits.LATB0 = !LATBbits.LATB0;   // toggle state of RB0
}

typedef struct {
    int n;
    int N;
} heartbeat;

heartbeat schedInfo[MAX_TASKS];

void init_scheduler() {
    schedInfo[0].N = 1;
    schedInfo[1].N = 50;
    schedInfo[2].N = 100;
    int i;
    for(i = 0; i < MAX_TASKS; i++) {
        schedInfo[i].n = 0;
    }
}

void scheduler() {
    int i;
    // int executed = 0;
    for (i = 0; i < MAX_TASKS; i++) {
        schedInfo[i].n++;
        if(schedInfo[i].n >= schedInfo[i].N) {
            switch(i) {
                case 0:
                    task1_write_character();
                    break;
                case 1:
                    task2_slide_controller();
                    break;
                case 2:
                    task3_blink_led();
                    break;
                default:
                    break;
            }
            schedInfo[i].n = 0;
        }
    }
}


int main(void) {
    
    init_devices();
    init_scheduler();
    
    tmr_setup_period(TIMER1, HEARTBEAT);
    
    int button_state = PORTEbits.RE8;
    int new_button_state;
    
    while(1) {
        
        // schedule all tasks
        scheduler();
        
        // toggle slide on/off
        new_button_state = PORTEbits.RE8;
        if(!new_button_state && button_state) {
            lcd.slider_on = !lcd.slider_on;
        }
        
        // change slider speed
        while(ADCON1bits.DONE == 0);
        ADCON1bits.DONE = 0;
        schedInfo[1].N = 50 + (int)((double)ADCBUF0 / 1023.0 * 150.0);  // time is an opinion
        
        // sleep :) zzz...
        tmr_wait_period(TIMER1);
    }
}
