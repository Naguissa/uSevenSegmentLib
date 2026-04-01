/**
 * Really tiny library to basic 7 segments displays
 *
 * Dicover segments connections utility.
 *
 * This example doesn't use any library functionality. It's useful to discover the connections to all segments.
 * It will light all segments in order to be able to check pins and muxes defintions.
 * After that, pins and muxes definitions can be copied to your sketch.
 *
 *
 * *Segments in the display:*
 *
 *    -- a --
 *   |       |
 *   f       b
 *   |       |
 *    -- g --
 *   |       |
 *   e       c
 *   |       |
 *    -- d --  dot
 *
 * *Binary correspondence:*
 *  
 *  0b.gfedcba
 *
 * *Pins definition:*
 *  
 *  Same order as binary representation: {dot, g, f, e, d, c, b, a}
 *
 * *Muxes definition:*
 *  
 *  Left to right: {digit at left, digit at middle, digit at right....}
 *
 *
 *
 * @see <a href="https://github.com/Naguissa/uSevenSegmentLib">https://github.com/Naguissa/uSevenSegmentLib</a>
 * @see <a href="https://github.com/Naguissa/uTimerLib">https://github.com/Naguissa/uTimerLib</a> - Needed dependecy
 * @see <a href="https://www.foroelectro.net/librerias-arduino-ide-f29/usevensegmentlib-libreria-arduino-para-controlar-d-t193.html">https://www.foroelectro.net/librerias-arduino-ide-f29/usevensegmentlib-libreria-arduino-para-controlar-d-t193.html</a>
 * @see <a href="mailto:naguissa@foroelectro.net">naguissa@foroelectro.net</a>
 */
#include "Arduino.h"

// Reverse this to test common annode ones
#define ONE 1
#define ZERO 0

#ifdef _VARIANT_ARDUINO_STM32_
	int pins[8] = {D4, D5, D6, D7, D8, D9, D10, D11};
	int mux = D12;
#else
	int pins[8] = {2, 3, 4, 5, 6, 7, 8, 9};
	int mux = 11;
#endif

char names[8][4] = {"dot", "g", "f", "e", "d", "c", "b", "a"};

unsigned char i;

void setup() {
	delay (2000);
	Serial.begin(9600);
	Serial.println("Serial OK");

	pinMode(mux, OUTPUT);
	digitalWrite(mux, ZERO);

	for(i = 0; i < 8; i++) {
		pinMode(pins[i], OUTPUT);
	}
}

void loop() {
	for (i = 0; i < 8; i++) {
		digitalWrite(pins[i], ONE);
    	Serial.print(names[i]);
    	Serial.println(" ON");
		delay(1500);
	}
	delay(3500);

	for (i = 0; i < 8; i++) {
		digitalWrite(pins[i], ZERO);
    	Serial.print(names[i]);
    	Serial.println(" OFF");
		delay(1500);
	}
	delay(5000);
}

