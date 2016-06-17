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


/************************************************
 * function prototypes                          *
 ************************************************/
int LCD_SetAddr(int page, int column);
void LCD_Write(unsigned int cmd, unsigned int DorC);
int PlotPoint(int x, int y, unsigned char state);
void LCD_ClearDisplay(void);

#endif	/* ILI9163_H */
