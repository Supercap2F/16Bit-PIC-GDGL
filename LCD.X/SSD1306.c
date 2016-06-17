/************************************************
 * this file contains the source code for the   *
 * functions: OLED_SetAddr, OLED_WriteChar, and *
 * OLED_WriteString.                            *
 ************************************************/
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
 * OLED_SetAddr function                        *
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

