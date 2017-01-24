#include "stdint.h"
#include "ST7735.h"


void ST7735_sDecOut3(int32_t n) {
		if(n > 9999 || n < -9999) { // print *.***
				ST7735_DrawCharS(0, 0, '*', ST7735_Color565(255, 0, 0), 0, 1);
				ST7735_DrawCharS(0, 6, '.', ST7735_Color565(255, 0, 0), 0, 1);
				int16_t col = 12;
				for(int i = 0; i < 3; i++) {
						ST7735_DrawCharS(0, col, '*', ST7735_Color565(255, 0, 0), 0, 1);
						col = col + 6;
				}				
		}
		
		//ST7735_DrawCharS(0, 0, 'c', ST7735_Color565(255, 0, 0), 0, 1);
}
