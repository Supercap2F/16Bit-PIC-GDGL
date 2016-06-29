/************************************************
 * File:   ILI9163.h                            *
 * Author: Supercap2F                           *
 *                                              *
 * Created on June 17, 2016, 1:32 PM            *
 ************************************************/
#ifndef ILI9163_H // inclusion guard
#define	ILI9163_H

/************************************************
 * definitions                                  *
 ************************************************/
/* screen info */
#define RESET LATBbits.LATB4 // i/o pins 
#define WR    LATAbits.LATA3
#define RD    LATAbits.LATA2
#define RS    LATAbits.LATA1
#define CS    LATAbits.LATA0

#define Scrn_W 128 // screen width
#define Scrn_H 128 // screen heigth

/* returnable codes */
#define SUCCESS 1
#define OUT_OF_RANGE 2

/* passable codes   */
#define CMD 0
#define DAT 1

#define NORM    0b000 // display rotation values
#define BOT_LEF 0b101
#define BOT_RGT 0b110
#define TOP_RGT 0b011

/* colors */
#define RED     0xF800
#define GREEN   0x07E0
#define BLUE    0x001F
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0 
#define BLACK   0x0000
#define WHITE   0xFFFF

/************************************************
 * function prototypes                          *
 ************************************************/
void LCD_Setup(void);
int LCD_SetAddr(unsigned char column, unsigned char page, unsigned char endcolumn, unsigned char endpage);
void LCD_Write(unsigned int cmd, unsigned int DorC);
int PlotPoint(int x, int y, int state);
void LCD_ClearDisplay(void);
void LCD_SetRotation(unsigned char rot);

#endif	/* ILI9163_H */
