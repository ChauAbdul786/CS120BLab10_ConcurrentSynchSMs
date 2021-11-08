/*	Author: Abdullah Chaudhry
 *  Partner(s) Name: 
 *	Lab Section: 021
 *	Assignment: Lab #10  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Link: https://drive.google.com/file/d/1xu_ZnRAzMvSm0Y8e8YVlYaqGiEyUUcLK/view?usp=sharing
 *
 */
#include <avr/io.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum BlinkingLEDStates { BL_Start, BL_LEDOn, BL_LEDOff } BLState;
unsigned char blinkingLED;

enum ThreeLEDsStates { TL_Start, TL_LED1, TL_LED2, TL_LED3 } TLState;
enum LEDStates {none, one, two, three} threeLEDs; 

enum CombineLEDsStates { CL_Start, CL_Write } CLState;

void TickFct_ThreeLEDs();
void TickFct_BlinkingLED();
void TickFct_CombineLEDs();

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRB = 0xFF; PORTB = 0x00; //PORTB is output

    /* Insert your solution below */
    //Initialize Timer
    TimerSet(100);
    TimerOn();
    unsigned long timerPeriod = 100;

    unsigned long TLCount = 0;
    unsigned long BLCount = 0;
    unsigned long CLCount = 0;

    TLState = TL_Start;
    BLState = BL_Start;
    CLState = CL_Start;

    while (1) {
        if(TLCount >= 300){
            TickFct_ThreeLEDs();
            TLCount = 0;
        }    

        if(BLCount >= 1000){
            TickFct_BlinkingLED();
            BLCount = 0;
        }

        if(CLCount >= 300){
            TickFct_CombineLEDs();
            CLCount = 0;
        }

        while(!TimerFlag) {};
        TimerFlag = 0;
        TLCount += timerPeriod;
        BLCount += timerPeriod;
        CLCount += timerPeriod;
    }

    return 1;
}

//TODO: TickFct_BlinkingLED
void TickFct_BlinkingLED(){
    switch(BLState){ //State Transitions
    case BL_Start:
        BLState = BL_LEDOn;
        break;
    case BL_LEDOn:
        BLState = BL_LEDOff;
        break;
    case BL_LEDOff:
        BLState = BL_LEDOn;
        break;
    default:
        BLState = BL_LEDOn;
        break;
    }

    switch(BLState){ //State Actions
    case BL_Start:
        break;
    case BL_LEDOn:
        blinkingLED = 1;
        break;
    case BL_LEDOff:
        blinkingLED = 0;
        break;
    default:
        blinkingLED = 1;
        break;
    }
}

//TODO: TickFct_ThreeLEDs
void TickFct_ThreeLEDs(){
    switch(TLState){ //State Transitions
    case TL_Start:
        TLState = TL_LED1;
        break;
    case TL_LED1:
        TLState = TL_LED2;
        break;
    case TL_LED2:
        TLState = TL_LED3;
        break;
    case TL_LED3:
        TLState = TL_LED1;
        break;
    default:
        TLState = TL_LED1;
        break;
    }

    switch(TLState){ //State Actions
    case TL_Start:
        break;
    case TL_LED1:
        threeLEDs = one;
        break;
    case TL_LED2:
        threeLEDs = two;
        break;
    case TL_LED3:
        threeLEDs = three;
        break;
    default:
        threeLEDs = one;
        break;
    }
}

void TickFct_CombineLEDs(){
    switch(CLState){ //State Transitions
    case CL_Start:
        CLState = CL_Write;
        break;
    case CL_Write:
        CLState = CL_Write;
        break;
    default:
        CLState = CL_Write;
        break;
    }

    switch(CLState){ //State Actions
    case CL_Start:
        break;
    case CL_Write:
        //Blinking LED
        if(blinkingLED){
            PORTB |= 0x08; //BBBB 1BBB
        }else{
            PORTB &= 0xF7; //BBBB 0BBB
        }

        //ThreeLEDs
        if(threeLEDs == none){
            PORTB &= 0xF8; //BBBB B000
        }else if(threeLEDs == one){
            PORTB &= 0xF8; //BBBB B000
            PORTB |= 0x01; //BBBB B001
        }else if(threeLEDs == two){
            PORTB &= 0xF8; //BBBB B000
            PORTB |= 0x02; //BBBB B010
        }else if(threeLEDs == three){
            PORTB &= 0xF8; //BBBB B000
            PORTB |= 0x04; //BBBB B100
        }
        break;

    default:
        //Blinking LED
        if(blinkingLED){
            PORTB |= 0x08; //BBBB 1BBB
        }else{
            PORTB &= 0xF7; //BBBB 0BBB
        }

        //ThreeLEDs
        if(threeLEDs == none){
            PORTB &= 0xF8; //BBBB B000
        }else if(threeLEDs == one){
            PORTB &= 0xF8; //BBBB B000
            PORTB |= 0x01; //BBBB B001
        }else if(threeLEDs == two){
            PORTB &= 0xF8; //BBBB B000
            PORTB |= 0x02; //BBBB B010
        }else if(threeLEDs == three){
            PORTB &= 0xF8; //BBBB B000
            PORTB |= 0x04; //BBBB B100
        }
        break;
    }
}

