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

void ST7735_XYplotInit(char *title, int32_t minX, int32_t maxX, int32_t minY, int32_t maxY) {
		*plot_title = *title;
		plot_minX = minX;
		plot_maxX = maxX;
		plot_minY = minY;
		plot_maxY = maxY;
}

void ST7735_XYplot(uint32_t num, int32_t bufX[], int32_t bufY[]) {
		// x ranges from 0 to 127, y ranges from 32 to 159
		int16_t x = 0;
		int16_t y = 0;
		for(int i = 0; i < num; i++) {
				x = 127*(bufX[i] - plot_minX)/(plot_maxX - plot_minX);
				y = 32 + 127*(bufY[i] - plot_minY)/(plot_maxY - plot_minY);
				ST7735_DrawPixel(x, y, ST7735_BLUE);			
		}
}
