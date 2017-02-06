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
			ST7735_DrawPixel(x, y, ST7735_BLUE);			
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
	uint16_t y;
	uint16_t x;

	int16_t rise = y2 - y1;
	int16_t run = x2 - x1;
	if(run == 0) { // no change in x, avoid divide by 0
		for(y = y1; y <= y2; y++) {
			ST7735_DrawPixel(x1, y, color);
		}
	} else {
		bool neg = false;
		int16_t slope = rise/run;
		if(slope < 0) { 
			neg = true; 
			slope = -slope;
		}
		int16_t deficit = rise - slope*run; // amt of y not reached bc of rounding of slope
		if(deficit < 0) { deficit = -deficit; }
		uint16_t n = run/deficit; // need to add an extra pixel every n times to make up for deficit
		uint16_t turn_count = 1; // way to keep track of when to add extra pixel
		y = y1;
		
		for(x = x1; x <= x2; x++) {
			for(int i = 0; i <= slope; i++) { // draw line along x1-x2
				ST7735_DrawPixel(x, y, color);
				if(neg) { y = y - 1; }
				else { y++; }
			}
			if(turn_count == n && deficit > 0) {
				ST7735_DrawPixel(x, y, color);
				if(neg) { y = y - 1; }
				else { y++; }
				deficit = deficit - 1;
				turn_count = 0; // reset turn_count
			}
			turn_count++;
		}
		while(deficit > 0) {
			ST7735_DrawPixel(x2, y, color);
			if(neg) { y = y - 1; }
			else { y++; }
			deficit = deficit - 1;
		}
		ST7735_SetCursor(0,0);
		ST7735_OutString("Final x,y: ");
		ST7735_OutputNumber(x);
		ST7735_OutChar(',');
		ST7735_OutputNumber(y);		
	}
}

