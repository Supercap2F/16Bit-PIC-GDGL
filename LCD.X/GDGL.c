/************************************************
 * graphical display graphics library           *
 ************************************************/
/************************************************
 * Included files                               *
 ************************************************/
#include "GDGL.h"
#include "OLED.h"
#include <stdlib.h>
#include <xc.h>
#include "DefaultFont.h"

/************************************************
 * Global variables                             *
 ************************************************/


/************************************************
 * PlotLine function - This bit of code is      *
 * based on Bresenham's Line Algorithm but      *
 * written by jorticus. Be sure and check out   *
 * his awesome project here:                    *
 * https://github.com/jorticus/zeitgeber-firmware                       
 ************************************************/
int PlotLine(int x0, int y0, int x1, int y1,unsigned char color) {
    int dx, dy;
	int sx, sy, err;
	int e2;

    dx = abs(x1 - x0);
    dy = abs(y1 - y0);

    sx = (x0 < x1) ? 1 : -1;
    sy = (y0 < y1) ? 1 : -1;
    err = dx - dy;

    while (1) {
        OLED_PlotPoint(x0, y0, color);

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
void PlotVLine(int x, int y, int length, unsigned char color) {
    int t;
    if(length<0){        // if length is negative 
        length=0-length; // swap length sign
        y=y-length;      // swap y position
    }
    for(t=0;t<length;t++) 
        OLED_PlotPoint(x,y+t,color);
}
/************************************************
 * PlotHLine Function - this function plots a   *
 * optimized horizontal line                    *
 ************************************************/
void PlotHLine(int x, int y, int length, unsigned char color) {
    int t;
    if(length<0){        // if length is negative 
        length=0-length; // swap length sign
        x=x-length;      // swap y position
    }
    for(t=0;t<length;t++) 
        OLED_PlotPoint(x+t,y,color);
}
/************************************************
 * PlotRectangle Function - this function plots *
 * optimized rectangle                          *
 ************************************************/
void PlotRectangle(int x, int y, int w, int h, unsigned char color) {
    PlotHLine(x,y,w,color);   // plot top line
    PlotHLine(x,y+h-1,w,color); // plot bottom line
    PlotVLine(x,y,h,color);   // plot left line
    PlotVLine(x+w-1,y,h,color); // plot right line
}
/************************************************
 * PlotTriangle Function - this function plots  *
 * triangle ABC                                 *
 ************************************************/
void PlotTriangle(int xA, int yA, int xB, int yB, int xC, int yC, unsigned char color){
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
void PlotCircleQuadrant(int x0, int y0,int r,int quad,unsigned char color) {
   int x=r; 
   int y=0;
   int del_x=1-2*r; 
   int del_y=1;
   int r_err=0;
   
   while (x>=y) {
     switch(quad) {
           case 3:
               OLED_PlotPoint(x0 + x, y0 + y,color); // 3
               OLED_PlotPoint(x0 + y, y0 + x,color); // 3
               break;
           case 2:
               OLED_PlotPoint(x0 - y, y0 + x,color); // 2
               OLED_PlotPoint(x0 - x, y0 + y,color); // 2
               break;
           case 1:
               OLED_PlotPoint(x0 - x, y0 - y,color); // 1
               OLED_PlotPoint(x0 - y, y0 - x,color); // 1
               break;
           case 0:
               OLED_PlotPoint(x0 + y, y0 - x,color); // 0
               OLED_PlotPoint(x0 + x, y0 - y,color); // 0
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
void PlotCircle(int x0, int y0, int r, unsigned char color)  {
   int x=r; 
   int y=0;
   int del_x=1-2*r; 
   int del_y=1;
   int r_err=0;
   
   while (x>=y) {
      OLED_PlotPoint(x0 + x, y0 + y,color); // 3 plot all quadrants (circle drawn with symmetry)
      OLED_PlotPoint(x0 + y, y0 + x,color); // 3
      OLED_PlotPoint(x0 - y, y0 + x,color); // 2
      OLED_PlotPoint(x0 - x, y0 + y,color); // 2            
      OLED_PlotPoint(x0 - x, y0 - y,color); // 1
      OLED_PlotPoint(x0 - y, y0 - x,color); // 1
      OLED_PlotPoint(x0 + y, y0 - x,color); // 0
      OLED_PlotPoint(x0 + x, y0 - y,color); // 0
                
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
void PlotRoundedRect(int x, int y, int w, int h, int r, unsigned char color){
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
void PlotFilledRectangle(int x, int y, int w, int h, unsigned char color) {
    int z;
    for(z=x;z<w+x;z++)
        PlotVLine(z,y,h,color);
}
/************************************************
 * PlotRoundFilledSide Function - function      *
 * plots a rectangle side with rounded corners. *
 * 0 = right, 1 = left                          *  
 ************************************************/
void PlotRoundFilledSide(int x0, int y0, int r, int h, int RorL, unsigned char color){
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
void PlotFiledRoundedRect(int x0, int y0, int w, int h, int r, unsigned char color) {
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
void PlotFilledCircle(int x0, int y0, int r, unsigned char color){
    PlotVLine(x0, y0-r, 2*r+1, color);
    PlotRoundFilledSide(x0, y0, r, 0, 0, color); // plot right side
    PlotRoundFilledSide(x0, y0, r, 0, 1, color); // plot left side 
}
/*******************************************************************************
 * Only text drawing functions past this point                                 *
 *******************************************************************************/
/************************************************
 * WriteChar Function - function plots a single *
 * character.                                   *
 ************************************************/
int WriteChar(int x0, int y0, unsigned char letter, int size, unsigned char color){
    int x,y;
    unsigned char mask;
    if(letter<0x20||letter>0x7E) // if function is sent a ASCII character it can't print
        return(GDGL_OUTOFRANGE); // return Out of Range error 
    letter-=0x20;                // subtract unused ASCII characters so variable  
                                 // 'letter' can be used for an arrays value
    
    for(x=x0;x<x0+5;x++) {
        for(mask=1,y=y0;y<y0+8;y++,mask=mask<<1){
            OLED_PlotPoint(x,y,((Dfont[letter][(x-x0)] & mask) ? color:0));
        }
    }
    return(GDGL_SUCCESS);
}


 
/*********************************************************************************
 * REASOURCES:                                                                   *
 * http://web.engr.oregonstate.edu/~sllu/bcircle.pdf - circle drawing algorithm  *
 * 
 *********************************************************************************/

