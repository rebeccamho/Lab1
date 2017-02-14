// filename ******** fixed.h ************** 
// Lab 1 Spring 2017
// Matt Owens & Rebecca Ho
// 1/26/17

#include "stdint.h"

/****************ST7735_sDecOut3***************
 converts fixed point number to LCD
 format signed 32-bit with resolution 0.001
 range -9.999 to +9.999
 Inputs:  signed 32-bit integer part of fixed-point number
 Outputs: none
 send exactly 6 characters to the LCD 
Parameter LCD display
 12345    " *.***"
  2345    " 2.345"  
 -8100    "-8.100"
  -102    "-0.102" 
    31    " 0.031" 
-12345    " *.***"
 */ 
void ST7735_sDecOut3(int32_t n);


/**************ST7735_uBinOut8***************
 unsigned 32-bit binary fixed-point with a resolution of 1/256. 
 The full-scale range is from 0 to 999.99. 
 If the integer part is larger than 256000, it signifies an error. 
 The ST7735_uBinOut8 function takes an unsigned 32-bit integer part 
 of the binary fixed-point number and outputs the fixed-point value on the LCD
 Inputs:  unsigned 32-bit integer part of binary fixed-point number
 Outputs: none
 send exactly 6 characters to the LCD 
Parameter LCD display
     0	  "  0.00"
     2	  "  0.01"
    64	  "  0.25"
   100	  "  0.39"
   500	  "  1.95"
   512	  "  2.00"
  5000	  " 19.53"
 30000	  "117.19"
255997	  "999.99"
256000	  "***.**"
*/
void ST7735_uBinOut8(uint32_t n); 

/**************ST7735_XYplotInit***************
 Specify the X and Y axes for an x-y scatter plot
 Draw the title and clear the plot area
 Inputs:  title  ASCII string to label the plot, null-termination
          minX   smallest X data value allowed, resolution= 0.001
          maxX   largest X data value allowed, resolution= 0.001
          minY   smallest Y data value allowed, resolution= 0.001
          maxY   largest Y data value allowed, resolution= 0.001
 Outputs: none
 assumes minX < maxX, and miny < maxY
*/
void ST7735_XYplotInit(char *title, int32_t minX, int32_t maxX, int32_t minY, int32_t maxY);

/**************ST7735_XYplot***************
 Plot an array of (x,y) data
 Inputs:  num    number of data points in the two arrays
          bufX   array of 32-bit fixed-point data, resolution= 0.001
          bufY   array of 32-bit fixed-point data, resolution= 0.001
 Outputs: none
 assumes ST7735_XYplotInit has been previously called
 neglect any points outside the minX maxY minY maxY bounds
*/
void ST7735_XYplot(uint32_t num, int32_t bufX[], int32_t bufY[]);

/**************ST7735_OutputNumber***************
 Outputs number in range of [-9999,9999]
 Inputs:		val is the number to be outputted
 Output: 	none
*/
void ST7735_OutputNumber(uint32_t val);

/**************ST7735_Line***************
 Draws one line on the ST7735 color LCD
 Inputs:	(x1, y1) is the start point
					(x1, y2) is the end point
						 x1, x2 are horizontal positions, columns from left edge
								must be less than 128
								0 is on the left, 126 is near the right
						 y1, y2 are vertical positions, rows from top edge
								must be less than 160
								159 is near the wires, 0 is side opposite the wires
					color 16-bit color, which can be produced by ST7735_Color565()
	Output: none
*/
void ST7735_Line2(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

void ST7735_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);


