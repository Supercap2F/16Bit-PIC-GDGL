/************************************************
 * File:   GDGL.h                               *
 * Author: Supercap2F                           *
 *                                              *
 * Created on June 3, 2016, 12:14 PM            *
 ************************************************/
#ifndef INC_GDGL_H // inclusion guard 
#define	INC_GDGL_H

/************************************************
 * definitions                                  *
 ************************************************/
#define GDGL_SUCCESS    10
#define GDGL_OUTOFRANGE 11 // out of range error 
#define GDGL_PRNTCUTOFF 12 
/************************************************
 * function prototypes                          *
 ************************************************/
int PlotLine(int x0, int y0, int x1, int y1, unsigned char color);
void PlotCircle(int x0, int y0, int r, unsigned char color);
void PlotVLine(int x, int y, int length, unsigned char color);
void PlotHLine(int x, int y, int length, unsigned char color);
void PlotRectangle(int x, int y, int w, int h, unsigned char color);
void PlotTriangle(int xA, int yA, int xB, int yB, int xC, int yC, unsigned char color);
void PlotCircleQuadrant(int CX, int CY,int R,int quad,unsigned char color);
void PlotRoundedRect(int x, int y, int w, int h, int r, unsigned char color);
void PlotFilledRectangle(int x, int y, int w, int h, unsigned char color);
void PlotRoundFilledSide(int x0, int y0, int r, int h, int RorL, unsigned char color);
void PlotFiledRoundedRect(int x0, int y0, int w, int h, int r, unsigned char color);
void PlotFilledCircle(int x0, int y0, int r, unsigned char color);
int WriteChar(int x0, int y0, unsigned char letter, int size, unsigned char color, unsigned char backcolor);
int WriteString(int x0, int y0, char *string, int size, unsigned char color, unsigned char backcolor);

#endif	/* INC_OLED_H */


