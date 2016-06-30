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

#define Scrn_W 128
#define Scrn_H 128

/************************************************
 * function prototypes                          *
 ************************************************/
/* Standard Aliasing Functions:*/
int PlotLine(int x0, int y0, int x1, int y1, int color);
void PlotCircle(int x0, int y0, int r, int color);
void PlotVLine(int x, int y, int length, int color);
void PlotHLine(int x, int y, int length, int color);
void PlotRectangle(int x, int y, int w, int h, int color);
void PlotTriangle(int xA, int yA, int xB, int yB, int xC, int yC, int color);
void PlotCircleQuadrant(int CX, int CY,int R,int quad, int color);
void PlotRoundedRect(int x, int y, int w, int h, int r, int color);
void PlotFilledRectangle(int x, int y, int w, int h, int color);
void PlotRoundFilledSide(int x0, int y0, int r, int h, int RorL, int color);
void PlotFilledRoundedRect(int x0, int y0, int w, int h, int r, int color);
void PlotFilledCircle(int x0, int y0, int r, int color);

/* Text Functions: */
int WriteChar(int x0, int y0, unsigned char letter, int color, int backcolor);
int WriteString(int x0, int y0, char *string, int color, int backcolor);
int SetTextSize(int Tx_size);
void SetTextWrap(char Tx_wrap);

/* Bitmap Functions: */
void PlotBitmap(int x0, int y0, int w, int h, const unsigned short *image);

/* Anti-Aliasing Functions: */
void PlotLineAA(int x0, int y0, int x1, int y1, unsigned short f_color, unsigned short b_color);
void PlotCircleAA(int x0, int y0, int r, int f_color, int b_color);
int PlotPointBrightness(int x, int y, unsigned short f_color, unsigned short b_color,float brightness);

/* Support Functions: */
int GDGL_abs(int num);
float fpart(float x);
float rfpart(float x);
int rnd(float x);

#endif	/* INC_OLED_H */


