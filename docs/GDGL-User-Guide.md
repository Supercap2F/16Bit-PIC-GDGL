# Function List:
### PlotLine
##### Definition
`int PlotLine(int x0, int y0, int x1, int y1, unsigned char color);`
##### Description
Funcition excepts two endpoints (`x0,y0 x1,y1`) and plots a line between them. The color of the line is defined as an 8-bit value (`color`).
<hr>
### PlotVLine
##### Definition
`void PlotVLine(int x, int y, int length, unsigned char color);`
##### Description 
Function excepts an endpoint (`x,y`) and a length (`length`) and plots a vertical line with those values. Use this for vertical lines instead of the PlotLine function, as this one is specifically optomized for vertical lines. The color of the line is defined as an 8-bit value (`color`).  
<hr>
### PlotHLine
##### Definition
`void PlotHLine(int x, int y, int length, unsigned char color);`
##### Description
Function excepts an endpoint (`x,y`) and a length (`length`) and plots a horizontal line with those values. Use this for horizontal lines instead of the PlotLine function, as this one is specifically optomized for horizontal lines. The color of the Line is defined as an 8-bit value (`color`).
