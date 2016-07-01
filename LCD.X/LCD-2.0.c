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
#include "LCD-Drivers\ILI9163.h"
#include "GDGL.h"
#include "TrumpImage.h"

/***********************************************
 * Definitions                                 *
 ***********************************************/
// NOTE: the pins that the interface to the LCD are defined in the LCD driver file 
#define SW1 PORTBbits.RB0
#define SW2 PORTBbits.RB1

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
    int x,y;
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
     ***********************************************
     *                   PORTB                     *
     * RB<15:8> -> D<7:0>      | RB7 <- GPIO3      *
     * RB6 <- GPIO1            | RB5 <- GPIO0      *
     * RB4 -> RESET            | RB3 - PGD         *
     * RB2 - PGC               | RB1 <- SW2        *
     * RB0 <- SW1                                  *
     ***********************************************
     *                   PORTA                     *
     * RA4 -> GPIO2            | RA3 -> WR         *
     * RA2 -> RD               | RA1 -> RS         *
     * RA0 -> CS               |                   *
     ***********************************************/
    ANSELB=0b0000000000000000; // turn off all analog features on PORTB<15:8>, RB7, RB1, and RB0
    TRISB =0b0000000011101111; // make PORTB<15:8> and RB4 outputs - RB1 and RB0 inputs and GPIO inputs
    LATB  =0b0000000000000000; // set all PORTB outputs low 
    
    ANSELA=0b0000000000000000; // turn off the analog features on all PORTA pins
    TRISA =0b0000000000000000; // make all PORTA pins outputs 
    LATA  =0b0000000000000000; // set all PORTA outputs low
    
    RD=1;
    CS=0;    // select the chip
    RESET=1; // active low reset
    
    __delay_ms(100);
    
    LCD_Setup();
    LCD_ClearDisplay();
    
    PlotLineAA(10,10,80,14,CYAN,BLACK);
    
    x=40; 
    y=0;
    int del_x=1-2*40; 
    int del_y=1;
    int r_err=0;

    while (x>=y) {
        PlotLineAA(63 + x, 63 + y,63,63,CYAN,BLACK); // 3 plot all quadrants (circle drawn with symmetry)
        PlotLineAA(63 + y, 63 + x,63,63,CYAN,BLACK); // 3
        PlotLineAA(63 - y, 63 + x,63,63,CYAN,BLACK); // 2
        PlotLineAA(63 - x, 63 + y,63,63,CYAN,BLACK); // 2            
        PlotLineAA(63 - x, 63 - y,63,63,CYAN,BLACK); // 1
        PlotLineAA(63 - y, 63 - x,63,63,CYAN,BLACK); // 1
        PlotLineAA(63 + y, 63 - x,63,63,CYAN,BLACK); // 0
        PlotLineAA(63 + x, 63 - y,63,63,CYAN,BLACK); // 0

        y++;
        r_err+=del_y;
        del_y+=2;
        if((2*r_err + del_x)>0) {
            x--;
            r_err+=del_x;
            del_x+=2;
        }
    }
    
    while(1);
}




