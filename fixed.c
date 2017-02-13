// filename ******** fixed.c ************** 
// Lab 1 Spring 2017
// Matt Owens & Rebecca Ho
// 1/26/17

#include "stdint.h"
#include "stdbool.h"
#include "ST7735.h"

char* plot_title;
int32_t plot_minX;
int32_t plot_maxX;
int32_t plot_minY;
int32_t plot_maxY;

void ST7735_sDecOut3(int32_t n) {
	if(n > 9999 || n < -9999) { // number is out of range
			ST7735_OutChar(' ');
			ST7735_OutChar('*');
			ST7735_OutChar('.');
			for(int i = 0; i < 3; i++) {
					ST7735_OutChar('*');
			}
			return;
	}
	bool neg = (n < 0); 
	if(neg) { // make n positive so digits are abs value
			n = -n;
	}
	int32_t rem = n % 1000;
	int32_t thous = n / 1000; // ones place
	int32_t hund = rem / 100; // tenths place
	rem = rem % 100;
	int32_t tens = rem / 10; // hundredths place
	int32_t ones = rem % 10; // thousandths place
	
	if(neg) {
		ST7735_OutChar('-');
	} else {
		ST7735_OutChar(' ');
	}
	
	ST7735_OutChar((char) thous+48);
	ST7735_OutChar('.');
	ST7735_OutChar((char) hund+48);
	ST7735_OutChar((char) tens+48);
	ST7735_OutChar((char) ones+48);
}

void ST7735_uBinOut8(int32_t n){
	if(n < 0 || n >= 256000){ // number n out of range, output '***.**'
			ST7735_OutChar('*');
			ST7735_OutChar('*');
			ST7735_OutChar('*');
			ST7735_OutChar('.');
			ST7735_OutChar('*');
			ST7735_OutChar('*');
			return;
	}

	if(n < 0)
			n = n * -1;
	
	n = (n * 100) / 256; //produces an integer for nice breakdown into digits
	int i;
	int digits[] = {0, 0, 0, 0, 0};
	//calculate digits
	for(i=4; i>=0; i--){
			digits[i] = n % 10;
			n = (n - digits[i]) / 10; //remove digit in one's place, then shift all digits down (100's to 10's place, 10's to 1's, etc.)
	}

	//print digits
	bool validUpperDigit = false;
	for(i = 0; i < 5; i++){
		//if 10's, 100's or 1000's place empty, we print a space, not a zero
		if(i < 2){
			if(digits[i] == 0 && validUpperDigit == false)
				ST7735_OutChar(' ');
			else{
				validUpperDigit = true;
				ST7735_OutChar((char) digits[i]+48);
			}
		}	
		//else print digit normally
		else if (i == 3){
			ST7735_OutChar('.');
			ST7735_OutChar((char) digits[i]+48);
		}
		else
			ST7735_OutChar((char) digits[i]+48);
	}
}

void ST7735_XYplotInit(char *title, int32_t minX, int32_t maxX, int32_t minY, int32_t maxY) {
	plot_title = title;
	plot_minX = minX;
	plot_maxX = maxX;
	plot_minY = minY;
	plot_maxY = maxY;
}

void ST7735_XYplot(uint32_t num, int32_t bufX[], int32_t bufY[]) {
	// x ranges from 0 to 127, y ranges from 32 to 159
	ST7735_OutString(plot_title);
	uint32_t x = 0;
	uint32_t y = 0;
	for(int i = 0; i < num; i++) {
			x = 127*(bufX[i] - plot_minX)/(plot_maxX - plot_minX);
			y = 32 + 127*(plot_maxY - bufY[i])/(plot_maxY - plot_minY);
			ST7735_DrawPixel(x, y, ST7735_WHITE);			
	}
	
	
}

void ST7735_OutputNumber(uint32_t val) {
	if(val > 9999) { // number is out of range
			ST7735_OutChar('*');
			for(int i = 0; i < 3; i++) {
					ST7735_OutChar('*');
			}
			return;
	}
	uint32_t rem = val % 1000;
	uint32_t thous = val / 1000; // thousands place
	int32_t hund = rem / 100; // hundreds place
	rem = rem % 100;
	int32_t tens = rem / 10; // tens place
	int32_t ones = rem % 10; // ones place
		
	ST7735_OutChar((char) thous+48);
	ST7735_OutChar((char) hund+48);
	ST7735_OutChar((char) tens+48);
	ST7735_OutChar((char) ones+48);
}

void ST7735_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
	//swap point 1 and point 2 if point 2 is left of point 1
	if(x1 > x2){
		int16_t tempX = x1;
		x1 = x2;
		x2 = tempX;
		int16_t tempY = y1;
		y1 = y2;
		y2 = tempY;
	}
	
	uint16_t last_x = 0; // for debug only
	uint16_t last_y = 0; // for debug only
	uint16_t y = y1;
	uint16_t x  = x1;

	int16_t rise = y2 - y1;
	int16_t run = x2 - x1;
	if(run == 0) { // no change in x, avoid divide by 0
		if(y2 > y1) {
			for(y = y1; y <= y2; y++) {
				ST7735_DrawPixel(x, y, color);
				last_x = x; last_y = y;
			}
		} else {
			for(y = y1; y >= y2; y = y-1) {
				ST7735_DrawPixel(x, y, color);
				last_x = x; last_y = y;
			}
		}
	} else if(rise == 0) {
		for(x = x1; x <= x2; x++) {
			ST7735_DrawPixel(x, y, color);
			last_x = x; last_y = y;
		}
	} else {
		bool neg = false;
		if(rise < 0) { // slope will be negative bc rise is negative
			neg = true; 
			rise = -rise;
		}
		int16_t slope = rise/run;
		int16_t y_deficit = rise - slope*run; // amt of y not reached bc of rounding of slope
		uint16_t n = run/y_deficit; // need to add an extra pixel in y dir every n times to make up for y_deficit
		uint16_t m = 0;
		uint16_t x_deficit = 0;
		if(y_deficit > 0) {
			x_deficit = run - n*y_deficit - 1; // amt of x not reached bc of rounding of n
			m = run/x_deficit; // need to add an extra pixel in x dir every m times to make up for x_deficit
		}
		uint16_t y_turn_count = 1; // way to keep track of when to add extra y pixel
		uint16_t x_turn_count = 1; // way to keep track of when to add extra x pixel
		
		for(x = x1; x <= x2; x++) {
			for(int i = 0; i < slope; i++) { // draw line along x1-x2
				ST7735_DrawPixel(x, y, color);
				last_x = x; last_y = y;
				if(neg) { y = y - 1; }
				else { y++; }
			}
			if(x_turn_count == m && x_deficit > 0) {
				ST7735_DrawPixel(x, y, color);
				last_x = x; last_y = y;
				x++;
				x_deficit = x_deficit - 1;
				x_turn_count = 0; // reset x_turn_count
			}
			if(y_turn_count == n) { // %% y_deficit > 0
				ST7735_DrawPixel(x, y, color);
				last_x = x; last_y = y;
				if(neg) { y = y - 1; }
				else { y++; }
				y_deficit = y_deficit - 1;
				y_turn_count = 0; // reset y_turn_count
			}

			y_turn_count++;
			x_turn_count++;
		}
		while(y_deficit > 0) {			
			for(int i = 0; i < slope; i++) { // draw line along x1-x2
				ST7735_DrawPixel(x, y, color);
				last_x = x; last_y = y;
				if(neg) { y = y - 1; }
				else { y++; }
				y_deficit = y_deficit - 1;
			}
			x++;
		}	
		/*
		while(x_deficit > 0) {
			ST7735_DrawPixel(x, y2, color);
			last_x = x; last_y = y;
			x++;
			x_deficit = x_deficit - 1;
		}			
			*/
	}
	
	/*
	ST7735_SetCursor(0,0);
	ST7735_OutString("Final x,y: ");
	ST7735_OutputNumber(last_x);
	ST7735_OutChar(',');
	ST7735_OutputNumber(last_y);
	*/
	
}

void ST7735_Line2(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color){
	//deltaX > deltaY, deltaX and deltaY positive
	//draw first point
	int16_t xPixel = x1;
	int16_t yPixel = y1;
	ST7735_DrawPixel(xPixel, yPixel, color);
	//set up pieces of equation
	int16_t deltaX = x2 - x1;
	int16_t deltaY = y2 - y1;
	int16_t j = y1;
	int16_t err = deltaY - deltaX;
	for(int i = x1; i < x2 - 1; i++){
		ST7735_DrawPixel(i, j, color);
		if(err >= 0){
			j++;
			err = err - deltaX;
		}
		err += deltaY;
	}
	
}

