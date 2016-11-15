/***********************************************************************************/
/* File:   SSD1351.c                                                               */
/* Author: Supercap2F                                                              */
/*                                                                                 */
/* Created on August 04, 2016, 6:00 PM                                             */
/* Copyright (c) 2016, Supercap2F                                                  */
/* All rights reserved.                                                            */
/*                                                                                 */
/* Redistribution and use in source and binary forms, with or without              */    
/* modification, are permitted provided that the following conditions are met:     */
/*                                                                                 */
/*    * Redistributions of source code must retain the above copyright notice,     */
/*      this list of conditions and the following disclaimer.                      */
/*                                                                                 */
/*    * Redistributions in binary form must reproduce the above copyright notice,  */
/*      this list of conditions and the following disclaimer in the documentation  */
/*      and/or other materials provided with the distribution.                     */
/*                                                                                 */
/*    * Neither the name of 16Bit-PIC-GDGL nor the names of its                    */
/*      contributors may be used to endorse or promote products derived from       */
/*      this software without specific prior written permission.                   */         
/*                                                                                 */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"     */
/* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE       */
/* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE    */
/* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL      */
/* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR      */
/* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER      */
/* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE   */
/* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.            */
/*                                                                                 */
/* Project repo here:                                                              */
/*      https://github.com/Supercap2F/16Bit-PIC-GDGL                               */
/***********************************************************************************/
/******************************************************************************/
/* Section: Includes                                                          */
/******************************************************************************/
#include <xc.h>
#define FCY 31323000 // must define FCY before including libpic30.h
#include <libpic30.h>
#include "SSD1351.h"

/******************************************************************************/
/* Section: LCD_Write Function - This function contains code for both the     */
/* 6800 and 8800 interface, uncomment the one that you want to use.           */
/******************************************************************************/
void LCD_Write(unsigned int cmd, unsigned int DorC) {
    /* This Code is for the 6800 interface of the display */
    dis_E_RD=1;
    dis_D_C=DorC;
    dis_CS=0;
    dis_R_W=0;
    PORTB=(cmd<<8) | (PORTB & 0x00FF); // shift cmd 8 places to the left, then 
                                       // clear the upper half of PORTB but keep
                                       // the lower half, and then OR the values 
                                       // together - this whole thing keeps what 
                                       // the lower half of PORTB was, but changes
                                       // the upper half to value in cmd 
    dis_E_RD=0; //clock the data in
    dis_CS=1;
    dis_D_C=~DorC;
    /********************************************************/
    
    /* This Code is for the 8800 interface of the display * /
    dis_E_RD=1;
    dis_D_C=DorC;
    dis_CS=0;
    dis_R_W=0;
    PORTB=(cmd<<8) | (PORTB & 0x00FF); // shift cmd 8 places to the left, then 
                                       // clear the upper half of PORTB but keep
                                       // the lower half, and then OR the values 
                                       // together - this whole thing keeps what 
                                       // the lower half of PORTB was, but changes
                                       // the upper half to value in cmd 
    dis_R_W=1;
    dis_CS=1;
    dis_D_C=~DorC;
    /********************************************************/
}


/******************************************************************************/
/* Section: LCD_Setup Function                                                */
/******************************************************************************/
void LCD_Setup(void) {
    int x, y;
    dis_CS=0; 
    __delay_ms(100);
    
    dis_RES=0;
    __delay_ms(100);
    dis_RES=1;
    __delay_ms(100);
    
     dis_EN=1;  // turn on VCC
    __delay_ms(100);
    
    LCD_Write(SET_SLEEP_ON,CMD); // Display off
    LCD_Write(SET_DIS_OFF,CMD); // Display off
    
   
    
    LCD_Write(SET_COMMAND_LCK,CMD); // Set Command Lock
	LCD_Write(0xb1,DAT);
	
    
	LCD_Write(SET_COL_ADDR,CMD); //set column
	LCD_Write(0x00,DAT);
	LCD_Write(0x7f,DAT);
	LCD_Write(SET_ROW_ADDR,CMD); //set row
	LCD_Write(0x00,DAT);
	LCD_Write(0x7f,DAT);
    
    LCD_Write(SET_COLOR_DEPTH,CMD); // Set Re-map / Color Depth  MAY NEED CHANGING
	LCD_Write(0x74,DAT);	        //   Color sequence is not swapped 
	LCD_Write(SET_DIS_OFFSET,CMD);  // Set display offset
	LCD_Write(0x00,DAT);
	LCD_Write(SET_DIS_NORM,CMD);    // Normal display
	LCD_Write(FUNCTION_SELECT,CMD); // Set Function selection
	LCD_Write(0x01,DAT);
	LCD_Write(SET_SLEEP_OFF,CMD);   // Set Sleep mode
	LCD_Write(SET_RESET,CMD);       // Set pre & dis_charge MAY NEED TO BE CHANGED
	LCD_Write(0x32,DAT);
	LCD_Write(FREQUENCY_CLOCK,CMD); // clock & frequency MAY NEED TO BE CHAGNED
	LCD_Write(0xf1,DAT);
	LCD_Write(SET_SEG_LOW_VOLT,CMD); // Set Segment LOW Voltage VSL SETTING MAY NEED CHANGING
	LCD_Write(0xa1,DAT);
	LCD_Write(0xb5,DAT);
	LCD_Write(0x55,DAT);
	LCD_Write(SET_GPIO,CMD);         // Set GPIO sets them low
	LCD_Write(0x0E,DAT);
	LCD_Write(SET_SEC_PRECHARGE,CMD);// Set Second Pre-charge Period MAY NEED TO BE CHANGED
	LCD_Write(0x01,DAT);
    
    // do gray scale stuff here
    
    
    LCD_Write(SET_PRECHARGE_VOLT,CMD); // Set pre-charge voltage of color A B C
	LCD_Write(0x17,DAT);
	LCD_Write(SET_VCOMH_VOLT,CMD);     // Set VcomH
	LCD_Write(0x05,DAT);
	LCD_Write(SET_CONTRAST,CMD);       // Set contrast current for A B C
	LCD_Write(0x88,DAT);
	LCD_Write(0x70,DAT);
	LCD_Write(0x88,DAT);
	LCD_Write(MSTR_CTRST_CURENT,CMD);  // Set master contrast
	LCD_Write(0x0F,DAT);
	LCD_Write(SET_MUX_RATIO ,CMD);     // Duty
	LCD_Write(0x7f,DAT);
   
    
	LCD_Write(SET_SLEEP_OFF,CMD);      // Display on
    //LCD_Write(SET_DIS_ON,CMD);
    

    __delay_ms(100);
}

   
/************************************************
 * PlotPoint function                           *
 ************************************************/
int PlotPoint(int x, int y, int state) {
    if ((x < 0) || (x >= Scrn_W) || (y < 0) || (y >= Scrn_H)) // if sent a pixel not on the screen
        return(OUT_OF_RANGE);                       // return error 
    
    LCD_SetAddr(x,y,x+2,y+2); 
    
    LCD_Write(WRITE_RAM_CMD,CMD);       // write memory write start command 
    LCD_Write(((state>>8)&0x00FF),DAT); // write the first part of the color
    LCD_Write((state&0x00FF),DAT);      // write the last part of the color
    
    return(SUCCESS);
}

/************************************************
 * LCD_SetAddr function                         *
 ************************************************/
int LCD_SetAddr(unsigned char column, unsigned char row, unsigned char endcolumn, unsigned char endrow) {
    LCD_Write(SET_COL_ADDR,CMD); //set column
	LCD_Write(column,DAT);
	LCD_Write(endcolumn,DAT);
    
    LCD_Write(SET_ROW_ADDR,CMD); //set row
	LCD_Write(row,DAT);
	LCD_Write(endrow,DAT);
    
    return(SUCCESS); 
}
