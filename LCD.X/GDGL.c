/************************************************
 * graphical display graphics library           *
 ************************************************/
/************************************************
 * Included files                               *
 ************************************************/
#include <stdlib.h>
#include <xc.h>
#include "GDGL.h"
#include "LCD-Drivers\ILI9163.h"
#include "Fonts\DefaultFont.h"

/************************************************
 * Global variables                             *
 ************************************************/
int tsize=1;  // text size variable
char txwrap=1; // text wrap variable: 1=wrap 0=don't wrap

/************************************************
 * PlotLine function - This bit of code is      *
 * based on Bresenham's Line Algorithm but      *
 * is written by jorticus. Be sure and check    *
 * out his awesome project here:                *
 * https://github.com/jorticus/zeitgeber-firmware                       
 ************************************************/
int PlotLine(int x0, int y0, int x1, int y1, int color) {
    int dx, dy;
	int sx, sy, err;
	int e2;

    dx = abs(x1 - x0);
    dy = abs(y1 - y0);

    sx = (x0 < x1) ? 1 : -1;
    sy = (y0 < y1) ? 1 : -1;
    err = dx - dy;

    while (1) {
        PlotPoint(x0, y0, color);

        if ((x0 == x1) && (y0 == y1)) return(GDGL_OUTOFRANGE);
        e2 = 2 * err;
        if (e2 > -dy) {
            err = err - dy;
            x0 = x0 + sx;
        }
        if (e2 < dx) {
            err = err + dx;
            y0 = y0 + sy;
        }
    }
}

/************************************************
 * PlotVLine Function - this function plots a   *
 * optimized vertical line                      *
 ************************************************/
void PlotVLine(int x, int y, int length, int color) {
    int t;
    if(length<0){        // if length is negative 
        length=0-length; // swap length sign
        y=y-length;      // swap y position
    }
    for(t=0;t<length;t++) 
        PlotPoint(x,y+t,color);
}
/************************************************
 * PlotHLine Function - this function plots a   *
 * optimized horizontal line                    *
 ************************************************/
void PlotHLine(int x, int y, int length, int color) {
    int t;
    if(length<0){        // if length is negative 
        length=0-length; // swap length sign
        x=x-length;      // swap y position
    }
    for(t=0;t<length;t++) 
        PlotPoint(x+t,y,color);
}
/************************************************
 * PlotRectangle Function - this function plots *
 * optimized rectangle                          *
 ************************************************/
void PlotRectangle(int x, int y, int w, int h, int color) {
    PlotHLine(x,y,w,color);   // plot top line
    PlotHLine(x,y+h-1,w,color); // plot bottom line
    PlotVLine(x,y,h,color);   // plot left line
    PlotVLine(x+w-1,y,h,color); // plot right line
}
/************************************************
 * PlotTriangle Function - this function plots  *
 * triangle ABC                                 *
 ************************************************/
void PlotTriangle(int xA, int yA, int xB, int yB, int xC, int yC, int color){
    PlotLine(xA,yA,xB,yB,color);
    PlotLine(xB,yB,xC,yC,color);
    PlotLine(xA,yA,xC,yC,color);
}
/************************************************
 * PlotCirculeQuadrant Function - this function *
 * plots a quadrant of a circle at a given      *
 * location (x,y). Quadrant numbers:            *
 *     |                                        *
 *   1 | 0                                      *
 * ---------                                    *
 *   2 | 3                                      *
 *     |                                        *
 ************************************************/
void PlotCircleQuadrant(int x0, int y0,int r,int quad, int color) {
   int x=r; 
   int y=0;
   int del_x=1-2*r; 
   int del_y=1;
   int r_err=0;
   
   while (x>=y) {
     switch(quad) {
           case 3:
               PlotPoint(x0 + x, y0 + y,color); // 3
               PlotPoint(x0 + y, y0 + x,color); // 3
               break;
           case 2:
               PlotPoint(x0 - y, y0 + x,color); // 2
               PlotPoint(x0 - x, y0 + y,color); // 2
               break;
           case 1:
               PlotPoint(x0 - x, y0 - y,color); // 1
               PlotPoint(x0 - y, y0 - x,color); // 1
               break;
           case 0:
               PlotPoint(x0 + y, y0 - x,color); // 0
               PlotPoint(x0 + x, y0 - y,color); // 0
               break;
       }
      y++;
      r_err+=del_y;
      del_y+=2;
      if((2*r_err + del_x)>0) {
          x--;
          r_err+=del_x;
          del_x+=2;
      }
   }
}
/************************************************
 * PlotCircle Function - based upon Bresenham's *
 * circle drawing algorithm.                     *
 ************************************************/
void PlotCircle(int x0, int y0, int r, int color)  {
   int x=r; 
   int y=0;
   int del_x=1-2*r; 
   int del_y=1;
   int r_err=0;
   
   while (x>=y) {
      PlotPoint(x0 + x, y0 + y,color); // 3 plot all quadrants (circle drawn with symmetry)
      PlotPoint(x0 + y, y0 + x,color); // 3
      PlotPoint(x0 - y, y0 + x,color); // 2
      PlotPoint(x0 - x, y0 + y,color); // 2            
      PlotPoint(x0 - x, y0 - y,color); // 1
      PlotPoint(x0 - y, y0 - x,color); // 1
      PlotPoint(x0 + y, y0 - x,color); // 0
      PlotPoint(x0 + x, y0 - y,color); // 0
                
      y++;
      r_err+=del_y;
      del_y+=2;
      if((2*r_err + del_x)>0) {
          x--;
          r_err+=del_x;
          del_x+=2;
      }
   }
}
/************************************************
 * PlotRoundRect Function - Plots a rectangle   *
 * with rounded corners                         *
 ************************************************/
void PlotRoundedRect(int x, int y, int w, int h, int r, int color){
    PlotHLine(x+r,y,w-2*r,color);     // plot top line
    PlotHLine(x+r,y+h-1,w-2*r,color); // plot bottom line
    PlotVLine(x,y+r,h-2*r,color);     // plot left line
    PlotVLine(x+w-1,y+r,h-2*r,color); // plot right line
    
    PlotCircleQuadrant(x+r,y+r,r,1,color);         // plot top left (quad 1) 
    PlotCircleQuadrant(x+w-r-1,y+r,r,0,color);     // plot top right (quad 0)
    PlotCircleQuadrant(x+w-r-1,y+h-r-1,r,3,color); // plot bottom right (quad 3)      
    PlotCircleQuadrant(x+r,y+h-r-1,r,2,color);     // plot bottom left (quad 2)
}
/************************************************
 * PlotFilledRectangle Function - Plots a       *
 * filled rectangle                             *
 ************************************************/
void PlotFilledRectangle(int x, int y, int w, int h, int color) {
    int z;
    for(z=x;z<w+x;z++)
        PlotVLine(z,y,h,color);
}
/************************************************
 * PlotRoundFilledSide Function - function      *
 * plots a rectangle side with rounded corners. *
 * 0 = right, 1 = left                          *  
 ************************************************/
void PlotRoundFilledSide(int x0, int y0, int r, int h, int RorL, int color){
    int x=r; 
    int y=0;
    int del_x=1-2*r; 
    int del_y=1;
    int r_err=0;
   
    while (x>=y) {
        if (RorL==0) {
            PlotVLine(x0+x, y0-y, 2*y+1+h, color);
            PlotVLine(x0+y, y0-x, 2*x+1+h, color);
        }
        if (RorL==1) {
            PlotVLine(x0-x, y0-y, 2*y+1+h, color);
            PlotVLine(x0-y, y0-x, 2*x+1+h, color);
        } 
                
      y++;
      r_err+=del_y;
      del_y+=2;
      if((2*r_err + del_x)>0) {
          x--;
          r_err+=del_x;
          del_x+=2;
      }
   }
    
}

/************************************************
 * PlotFilledRoundedRect Function - function    *
 * plots a rectangle with rounded corners.      *
 ************************************************/
void PlotFilledRoundedRect(int x0, int y0, int w, int h, int r, int color) {
    int z;
    for(z=y0;z<h+y0;z++)                  // plot center of the rounded rectangle 
        PlotHLine(x0+r,z,w-2*r,color);    //
    
    PlotRoundFilledSide(x0+w-r-1, y0+r, r, h-2*r-1, 0, color); // plot right side 
    PlotRoundFilledSide(x0+r,     y0+r, r, h-2*r-1, 1, color); // plot left side 
}

/************************************************
 * PlotFilledRoundedRect Function - function    *
 * plots a rectangle with rounded corners.      *
 ************************************************/
void PlotFilledCircle(int x0, int y0, int r, int color){
    PlotVLine(x0, y0-r, 2*r+1, color);
    PlotRoundFilledSide(x0, y0, r, 0, 0, color); // plot right side
    PlotRoundFilledSide(x0, y0, r, 0, 1, color); // plot left side 
}
/*******************************************************************************
 * Text drawing functions past this point                                      *
 *******************************************************************************/
/************************************************
 * WriteChar Function - function plots a single *
 * character.                                   *
 ************************************************/
int WriteChar(int x0, int y0, unsigned char letter, int color, int backcolor){
    int x,y,ySize,xSize;
    unsigned char mask;
    
    if(letter<0x20||letter>0x7E) // if function is sent a ASCII character it can't print
        return(GDGL_OUTOFRANGE); // return Out of Range error 
    letter-=0x20;                // subtract unused ASCII characters so variable  
                                 // 'letter' can be used for an arrays value
    
    for(x=x0;x<(5*tsize)+x0;x+=tsize){           // Please don't try to understand this - just accept it. 
        mask=1;                                  // reset mask variable 
        for(y=y0;y<(8*tsize)+y0;y=y+tsize){      //
            for(xSize=0;xSize<tsize;xSize++)     // 
                for(ySize=0;ySize<tsize;ySize++) // 
                    PlotPoint(x+xSize,y+ySize,((Dfont[letter][((x-x0)/tsize)] & mask) ? color:backcolor));
            mask<<=1;                            // shift the mask left 1 bit
        }
    }
    return(GDGL_SUCCESS);
}
/************************************************
 * WriteChar Function - function plots a string *
 ************************************************/
int WriteString(int x0, int y0, char *string, int color, int backcolor){
    int error_code=0;
    
    while(*string) // will loop until NULL is reached (0x00)
    {
        error_code=WriteChar(x0, y0, *string, color, backcolor); // write the current character to the screen
        
        if(error_code!=GDGL_SUCCESS) // if WriteChar returns an error 
            return(error_code);      // stop and return it to the user 
        
        if(txwrap) {                         // if the user wants the text to wrap:
            if(Scrn_W-(x0+5*tsize)<5*tsize){ // if there is not enough room to print the character on the same line
                y0+=8*tsize;                 // drop the current line down one 
                if(Scrn_H-y0<8*tsize)        // if there's not enough room to print the character on the next line
                    return(GDGL_PRNTCUTOFF); // return error
                x0=0;                        // reset x axis
                string++;                    // increment the current character
            }
            else {                                  // else 
                x0+=5*tsize;                        // do the regular text stuff
                PlotVLine(x0,y0,7*tsize,backcolor); //
                string++;                           //
                x0++;                               //
            }
        }
        else {                                 // else if the user does not want the text to wrap:
            x0+=5*tsize;                       // increment the x axis to make a spot for the new character
            PlotVLine(x0,y0,7*tsize,backcolor);// plot a blank line between characters
            string++;                          // increment the current character
            x0++;                              // increment x axis to make room for line just plotted
        }
    }
    return(GDGL_SUCCESS); // return success code 
}
/************************************************
 * SetTextSize Function - sets the current      *
 * text size.                                   *
 ************************************************/
int SetTextSize(int Tx_size){
    if(Tx_size==0)               // if size is zero
        return(GDGL_OUTOFRANGE); // return error 
    tsize=Tx_size;               // else set global size variable to Tx_size
    return(GDGL_SUCCESS);        // return success 
}
/************************************************
 * SetTextWrap Function - sets the text to wrap *  
 * or not.                                      *
 ************************************************/
void SetTextWrap(char Tx_wrap){
    if(Tx_wrap)   // if Tx_wrap is 1
        txwrap=1; // set the text to wrap
    else          // else
        txwrap=0; // set it to not wrap
}
/*******************************************************************************
 * Bitmap functions past this point                                            *
 *******************************************************************************/
/************************************************
 * PlotBitmap Function - plots a R5 G6 B5       *  
 * bitmap.                                      *
 ************************************************/
void PlotBitmap(int x0, int y0, int w, int h,const unsigned short *image){
    int x,y;
    for(y=0;y<h;y++)
        for(x=0;x<w;x++)
            PlotPoint(x+x0,y+y0,*(image+(x+y*w)));    
}

/*********************************************************************************
 * REASOURCES:                                                                   *
 * http://web.engr.oregonstate.edu/~sllu/bcircle.pdf - circle drawing algorithm  *
 * https://en.wikipedia.org/wiki/Xiaolin_Wu%27s_line_algorithm
 * https://github.com/jorticus/zeitgeber-firmware
 *********************************************************************************/

