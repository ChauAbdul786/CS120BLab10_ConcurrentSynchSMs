/*	Author: Abdullah Chaudhry
 *  Partner(s) Name: 
 *	Lab Section: 021
 *	Assignment: Lab #10  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

typedef struct task{ //task struct
    int state;
    unsigned long period;
    unsigned long elapsedTime;
    int (*TickFct)(int);
} task;

task tasks[3]; //Two tasks in this system
const unsigned char tasksNum = 3;
const unsigned long tasksPeriodGCD = 1000;
const unsigned long periodBlinkingLED = 1000;
const unsigned long periodThreeLEDs = 1000;
const unsigned long periodCombineLEDs = 1000;

enum BlinkingLEDStates { BL_Start, BL_LEDOn, BL_LEDOff };
unsigned char blinkingLED;
int TickFct_BlinkingLED(int state);

enum ThreeLEDsStates { TL_Start, TL_LED1, TL_LED2, TL_LED3 };
enum LEDStates {none, one, two, three} threeLEDs; 
int TickFct_ThreeLEDs(int state);

enum CombineLEDsStates { CL_Start, CL_Write };
int TickFct_CombineLEDs(int state);

void TimerISR(){
    unsigned char i;
    for(i = 0; i < tasksNum; i++){
        if(tasks[i].elapsedTime >= tasks[i].period){ //Task is ready
            tasks[i].state = tasks[i].TickFct(tasks[i].state); //Update state of task[i] with TickFct
            tasks[i].elapsedTime = 0; //Restart count
        }
    }
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRB = 0xFF; PORTB = 0x00; //PORTB is output

    /* Insert your solution below */
    //Initialize tasks array
    unsigned char i = 0;
    tasks[i].state = BL_Start;
    tasks[i].period = periodBlinkingLED;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &TickFct_BlinkingLED;
    i++;

    tasks[i].state = TL_Start;
    tasks[i].period = periodThreeLEDs;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &TickFct_ThreeLEDs;
    i++;

    tasks[i].state = CL_Start;
    tasks[i].period = periodCombineLEDs;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &TickFct_CombineLEDs;


    //Set timer
    TimerSet(tasksPeriodGCD);
    TimerOn();

    while (1) {
        Sleep(); //Low power until TimerISR()
    }
    return 1;
}

//TODO: TickFct_BlinkingLED
int TickFct_BlinkingLED(int state){
    switch(state){ //State Transitions
    case BL_Start:
        state = BL_LEDOn;
        break;
    case BL_LEDOn:
        state = BL_LEDOff;
        break;
    case BL_LEDOff:
        state = BL_LEDOn;
        break;
    default:
        state = BL_LEDOn;
        break;
    }

    switch(state){ //State Actions
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

    return state;
}

//TODO: TickFct_ThreeLEDs
int TickFct_ThreeLEDs(int state){
    switch(state){ //State Transitions
    case TL_Start:
        state = TL_LED1;
        break;
    case TL_LED1:
        state = TL_LED2;
        break;
    case TL_LED2:
        state = TL_LED3;
        break;
    case TL_LED3:
        state = TL_LED1;
        break;
    default:
        state = TL_LED1;
        break;
    }

    switch(state){ //State Actions
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

    return state;
}

int TickFct_CombineLEDs(int state){
    switch(state){ //State Transitions
    case CL_Start:
        state = CL_Write;
        break;
    case CL_Write:
        state = CL_Write;
        break;
    default:
        state = CL_Write;
        break;
    }

    switch(state){ //State Actions
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

    return state;
}

