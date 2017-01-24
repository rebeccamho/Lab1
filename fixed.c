#include "stdint.h"
#include "stdbool.h"
#include "ST7735.h"


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
		bool neg = (n < 0); // number is negative
		if(neg) { // make n positive so digits are abs value
				n = -n;
		}
		int32_t rem = n % 1000;
		int32_t thous = n/1000; // first digit
		int32_t hund = rem/100; // second digit, first after decimal
		rem = rem % 100;
		int32_t tens = rem/10; // third digit, sec after decimal
		int32_t ones = rem % 10; // fourth digit, third after decimal
		
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
