/***********************************************
 * File:   LCD.c                               *
 * Author: OPTIPLEX  960                       *
 *                                             *
 * Created on May 27, 2015, 12:06 PM           *
 ***********************************************/
/***********************************************
 * Included files                              *
 ***********************************************/
#include <xc.h>

#define FCY 31323000 // must define FCY before including libpic30.h
#include <libpic30.h>
#include "OLED.h"
#include "GDGL.h"

/***********************************************
 * Definitions                                 *
 ***********************************************/
// NOTE: the pins that the interface to the OLED are on, are defined in the OLED.h file 
#define START   PORTBbits.RB1
#define DIS_OFF PORTBbits.RB0

/***********************************************
 * Device configuration                        *
 ***********************************************/
#pragma config ICS=PGD1     // communicate with device on PGEC1 and PGED1
#pragma config JTAGEN=OFF   // turn off JTAG
#pragma config ALTI2C1=OFF  // don't remap I2C module 1 pins 
#pragma config ALTI2C2=ON   // I2C module two on pins ASDA2 and ASCL2
#pragma config PLLKEN=ON    // clock switch will wait for PLL LOCK to be set
#pragma config FWDTEN=OFF   // turn off watch dog timer
#pragma config POSCMD=NONE  // turn off the primary oscillator at startup
#pragma config OSCIOFNC=ON  // OSC2 doesn't output the system clock 
#pragma config IOL1WAY=OFF  // allow multiple PPS reconfigurations
#pragma config FCKSM=CSECMD // enable clock switching, and turn off the fail safe clock monitor 
#pragma config FNOSC=FRC    // turn on the Fast RC oscillator 
#pragma config IESO=OFF     // turn off two speed start up 

/***********************************************
 * main function                               *
 ***********************************************/
int main() 
{        
    /***********************************************
     * variable definitions                        *
     ***********************************************/
    int x;
    

    /***********************************************
     * oscillator configuration                    *
     ***********************************************/
    PLLFBD=32; // set the PLL divisor to 34 (32+2) FOSC will be 62.645MHz FCY will be 31.323MHz
    CLKDIVbits.PLLPOST=1; // set the PLL post divisor to 2 (0+2)
    CLKDIVbits.PLLPRE=0;  // set the PLL pre divisor to 2 (0+2)
    
    __builtin_write_OSCCONH(0x01);          // intilize clock switch to FRCPLL
    __builtin_write_OSCCONL(OSCCON | 0x01); // set the OSWEN bit (switch clock)
    
    while(OSCCONbits.COSC!= 0b001); // wait for clock switch to occur 
    while(OSCCONbits.LOCK!= 1);     // wait for PLL to lock 
    
    
    /***********************************************
     * I/O pin configuration                       *
     ***********************************************/
    /***********************************************
     * I/O connections:                            *
     * RB<15:8> -> D<7:0>      | RB7 -> TRAN       *
     * RB6 -> GPIO1            | RB5 -> GPIO2      *
     * RB1 -> START            | RB0 -> DIS_ON     * 
     * RA4 -> #E               | RA3 -> #R/W       *
     * RA2 -> #D/C             | RA1 -> #RES       *
     * RA0 -> #CS                                  *
     ***********************************************/
    ANSELB=0x000C; // turn off all analog features on PORTB<15:8>, RB7, RB1, and RB0
    TRISB=0x007F;  // make PORTB<15:8> and RB7 outputs - RB1 and RB0 inputs 
    LATB=0x00;     // set all PORTB outputs low 
    ANSELA=0x00;   // turn off the analog features on all PORTA pins
    TRISA=0x00;    // make all PORTA pins outputs 
    LATA=0x00;     // set all PORTA outputs low
    RES=1;         // except for #RESET (RA3) which needs to be high
    
    /***********************************************
     * main program                                *
     ***********************************************/
    while(1) {
    while(1)               // wait for switch START to be pressed 
        if(START!=1)       // if it's pressed
        {                  //
            __delay_ms(8); // delay 8mS to debounce 
            if(START!=1)   // if it's still pressed
                break;     // break from endless while loop 
        }                  // but if it isn't, do loop again 
    
    RES=0;           // set #RESET low (which resets the display)
    __delay_us(4);   // wait for it to register 
    RES=1;           // set #RESET back to it's original state
    __delay_us(4);   // wait for it to register
    TRAN=1;          // turn on VCC for the display
    __delay_ms(101); // wait for it to become stable and then turn on the display
    
    OLED_Write(0b10101111,OLED_CMD); // turn the display on 
    OLED_Write(0b00100000,OLED_CMD); // set display to horizontal addressing mode
    OLED_Write(0b00000000,OLED_CMD); //
    
  
    
    for(x=0;x<8192;x++)
        OLED_Write(0x00,OLED_DAT);
    OLED_SetAddr(0,0);
    
//    for(x=0;x<127;x=x+2)
//        OLED_PlotPoint(x,0,ON);
//    for(x=0;x<63;x=x+2)
//        OLED_PlotPoint(0,x,ON);
   
    SetTextSize(4);
    WriteString(2,2,"Yaba Yaba Yaba Yaba Yaba Yaba. Yaba Yaba Yaba Yaba Yaba Yaba. Yaba Yaba Yaba Yaba Yaba Yaba.",ON,OFF);
    
    
    while(1) { 
               // wait for switch DIS_OFF to be pressed 
        if(DIS_OFF!=1)     // if it's pressed
        {                  //
            __delay_ms(8); // delay 8mS to debounce 
            if(DIS_OFF!=1) // if it's still pressed
                break;     // break from endless while loop 
        }                  // but if it isn't, do loop again 
    }
    OLED_Write(0b10101110,OLED_CMD); // turn off the display 
    TRAN=0;                          // turn off VCC
    __delay_ms(100);                 // wait for Toff
    //while(1);                        // wait for user to remove power     
    }
}




