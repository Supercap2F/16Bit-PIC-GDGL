/***********************************************************************************
 * File:   ILI9163.c                                                               *
 * Author: Supercap2F                                                              *
 *                                                                                 *
 * Copyright (c) 2016, Supercap2F                                                  *
 * All rights reserved.                                                            *
 *                                                                                 *
 * Redistribution and use in source and binary forms, with or without              *     
 * modification, are permitted provided that the following conditions are met:     *
 *                                                                                 *
 *    * Redistributions of source code must retain the above copyright notice,     *
 *      this list of conditions and the following disclaimer.                      *
 *                                                                                 *
 *    * Redistributions in binary form must reproduce the above copyright notice,  *
 *      this list of conditions and the following disclaimer in the documentation  *
 *      and/or other materials provided with the distribution.                     *
 *                                                                                 *
 *    * Neither the name of 16Bit-PIC-GDGL nor the names of its                    *
 *      contributors may be used to endorse or promote products derived from       *
 *      this software without specific prior written permission.                   *          
 *                                                                                 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"     *
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE       *
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  *
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE    *
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL      *
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR      *
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER      *
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   *
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE   *
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.            *
 *                                                                                 *
 * Project repo here:                                                              *
 *      https://github.com/Supercap2F/16Bit-PIC-GDGL                               *
 ***********************************************************************************/
#include <xc.h>
#include "ILI9163.h" 
#define FCY 31323000 // must define FCY before including libpic30.h
#include <libpic30.h>

/************************************************
 * LCD_Write function                           *
 ************************************************/
void LCD_Write(unsigned int cmd, unsigned int DorC){
    RS=DorC; // 1=data 0=command
    RD=1;    // not reading from the display
    WR=1;    // set the clock high 
    
    cmd = ((cmd * 0x0802LU & 0x22110LU) | (cmd * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16;  // reverse the command 
    
    PORTB=(cmd<<8) | (PORTB & 0x00FF); // shift cmd 8 places to the left, then 
                                       // clear the upper half of PORTB but keep
                                       // the lower half, and then OR the values 
                                       // together - this whole thing keeps what 
                                       // the lower half of PORTB was, but changes
                                       // the upper half to value in cmd 
    WR=0;
    WR=1; // write the data to the display 
    
}

/************************************************
 * PlotPoint function                           *
 ************************************************/
int PlotPoint(int x, int y, int state) {
    if ((x < 0) || (x >= Scrn_W) || (y < 0) || (y >= Scrn_H)) // if sent a pixel not on the screen
        return(OUT_OF_RANGE);                       // return error 
    
    LCD_SetAddr(x,y,x+1,y+1); 
    
    LCD_Write(0b00101100,CMD);   // write memory write start command 
    LCD_Write(((state>>8)&0x00FF),DAT);   // write the first part of the color
    LCD_Write((state&0x00FF),DAT); // write the last part of the color
    
    return(SUCCESS);
}

/************************************************
 * LCD_SetAddr function                         *
 ************************************************/
int LCD_SetAddr(unsigned char column, unsigned char page, unsigned char endcolumn, unsigned char endpage) {
    LCD_Write(0b00101010,CMD); // write the column address 
    LCD_Write(0b00000000,DAT); // write 0x00 for X start (XS) 15:8
    LCD_Write(column,DAT);     // write the column address for X start (XS) 7:0
    LCD_Write(0b00000000,DAT); // write 0x00 for X end (XE) 15:8
    LCD_Write(endcolumn,DAT);      // write the column address for X end (XE) 7:0
    
    
    LCD_Write(0b00101011,CMD); // write the row address 
    LCD_Write(0b00000000,DAT); // write 0x00 for Y start (YS) 15:8
    LCD_Write(page,DAT);       // write the row address for Y start (YS) 7:0
    LCD_Write(0b00000000,DAT); // write 0x00 for Y end (YE) 15:8
    LCD_Write(endpage,DAT);     // write the row address for Y end (YE) 7:0
    
    return(SUCCESS); 
}

/************************************************
 * LCD_Setup function                           *
 ************************************************/
void LCD_Setup(void) {
    LCD_Write(0x01,CMD);       // software reset 
    
    LCD_Write(0b00010001,CMD); // sleep out
    __delay_ms(6);
    
    LCD_Write(0b00111010,CMD); // interface pixel format 
    LCD_Write(0x05,1); // 16 bits per pixel
    
    LCD_Write(0b00010011,CMD); // normal mode on 
    
    LCD_Write(0b00110110,CMD); // this sets the color to BRG but on my display it makes it RGB
    LCD_Write(0b00001000,DAT); 
    
    LCD_Write(0b00100110,CMD); // gamma curve
    LCD_Write(0x08,DAT);
    
    LCD_Write(0b00101001,CMD); // turn on the display
    __delay_ms(10);
    
    //LCD_Write(0b00101100,0); // start command 
}


/************************************************
 * LCD_ClearDisplay function                    *
 ************************************************/
void LCD_ClearDisplay(void) {
    int x;
    
    LCD_SetAddr(0,0,Scrn_W,Scrn_H); 
    
    LCD_Write(0b00101100,CMD); // write memory write start command 
    for(x=0;x<16384;x++){      // write black to the whole display 
        LCD_Write(0x00,DAT);   // write the first part of the color
        LCD_Write(0x00,DAT);   // write the last part of the color
    }
}

/************************************************
 * LCD_SetRotation function                     *
 ************************************************/
void LCD_SetRotation(unsigned char rot) {
    LCD_Write(0b00110110,CMD);          // set rotation command
    LCD_Write((rot<<5)|0b00001000,DAT);
}