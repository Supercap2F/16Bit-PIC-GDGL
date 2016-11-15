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
#include "LCD-Drivers\SSD1351.h"
#include "GDGL.h"
#include "Images\Bird_128x128_C.h"

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
     * RB<15:8> -> D<7:0>      | RB7 -> E          *
     * RB6 -> RW               | RB5 -> GPIO0/disEn*
     * RB4 -> RESET            | RB3 - PGD         *
     * RB2 - PGC               | RB1 <- SW2        *
     * RB0 <- SW1                                  *
     ***********************************************
     *                   PORTA                     *
     * RA4 <- GPIO1            | RA3 -> DC         *
     * RA2 -> CS               | RA1 <- GPIO2      *
     * RA0 <- GPIO3            |                   *
     ***********************************************/
    ANSELB=0b0000000000000000; // turn off all analog features on PORTB<15:8>, RB7, RB1, and RB0
    TRISB =0b0000000000001111; // 
    LATB  =0b0000000000000000; // set all PORTB outputs low 
    
    ANSELA=0b0000000000000000; // turn off the analog features on all PORTA pins
    TRISA =0b0000000000010011; //  
    LATA  =0b0000000000000000; // set all PORTA outputs low
    
    //RD=1;
    //CS=0;    // select the chip
    //RESET=1; // active low reset
    
    //__delay_ms(100);
    
    LCD_Setup();
    for(x=0;x<128;x++)
        for(y=0;y<128;y++){
            PlotPoint(x,y,BLACK);
        }
    
    PlotBitmap(0,0,128,64,bird_color);
    
    while(1);
}




