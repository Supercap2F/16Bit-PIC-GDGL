/************************************************
 * this file contains the source code for the   *
 * functions: OLED_SetAddr, OLED_WriteChar, and *
 * OLED_WriteString.                            *
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
/************************************************
 * Included files                               *
 ************************************************/
#include "SSD1306.h"
#include <xc.h>

/************************************************
 * Global variables                             *
 ************************************************/
unsigned char ScrnBuff[9][129];
unsigned int current_page=0;
unsigned int current_column=0;

/************************************************
 * OLED_PlotPoint function                      *
 ************************************************/
int PlotPoint(int x, int y, unsigned char state) {
    if ((x < 0) || (x >= 128) || (y < 0) || (y >= 64)) // if sent a pixel not on the screen
        return(OLED_OUTOFRANGE);                       // return error 
    
    switch(state) {
        case ON: 
            ScrnBuff[(y/8)][x]|=(1<<(y&7));
            break;
        case OFF:
            ScrnBuff[(y/8)][x]&=~(1<<(y&7));
            break;
        case IVRT:
            ScrnBuff[(y/8)][x]^=(1<<(y&7));
            break;       
    }
    OLED_SetAddr((y/8),x);
    OLED_Write(ScrnBuff[(y/8)][x],OLED_DAT);
   
    return(OLED_SUCCESS);
}

/************************************************
 * OLED_SetAddr function                        *
 ************************************************/
int OLED_SetAddr(int page, int column)
{
    if(column>127||column<0||page>7||page<0) // if the function is sent an invalid column/page address
        return(OLED_OUTOFRANGE);            // return error
    
    current_page=page;     // update current address variables
    current_column=column;
    
    OLED_Write(0b00100001,OLED_CMD); // write new column address
    OLED_Write(column,OLED_CMD);     //
    OLED_Write(0b01111111,OLED_CMD); // the end column address will still be 127 though 
    
    OLED_Write(0b00100010,OLED_CMD); // write the new page address
    OLED_Write(page,OLED_CMD);       //
    OLED_Write(0b00000111,OLED_CMD); // the end page address will still be 7 though
    
    return(OLED_SUCCESS); // return success code 
}


/***************************************************
 * function OLED_WriteCMD - This function writes   *
 * command/data to the OLED display                *
 ***************************************************/
void OLED_Write(unsigned int cmd, unsigned int DorC)
{
    CS=0;    // select the OLED
    RW=0;    // the data will be written
    D_C=DorC; // set data/command to what the user wants 
    
    E=1;  // set the clock high 
    
 //   if(DorC==OLED_DAT) {
 //      ScrnBuff[current_page][current_column]=cmd;
 //       current_column++; 
 //       if(current_column==128) { // update the current address variables
 //           current_column=0;
 //           current_page++;
//            if(current_page==8)
//                current_page=0;
//        }
//    }
    
    cmd = ((cmd * 0x0802LU & 0x22110LU) | (cmd * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16;  // reverse the command 
    
    PORTB=(cmd<<8) | (PORTB & 0x00FF); // shift cmd 8 places to the left, then 
                                       // clear the upper half of PORTB but keep
                                       // the lower half, and then OR the values 
                                       // together - this whole thing keeps what 
                                       // the lower half of PORTB was, but changes
                                       // the upper half to value in cmd 
    __builtin_nop(); // delay for 64nS
    __builtin_nop(); //
    E=0;             // clock data in 
    __builtin_nop(); // delay for 32nS
    return;       
}

/************************************************
 * OLED_ClearDisplay function                   *
 ************************************************/
void OLED_ClearDisplay(void) {
    int x,y;
    
    OLED_SetAddr(0,0);
    for(y=0;y<8;y++) {
        for(x=0;x<128;x++) {
            OLED_Write(0x00,OLED_DAT);
            ScrnBuff[y][x]=0x00;
        }
    }
    OLED_SetAddr(0,0);
}

