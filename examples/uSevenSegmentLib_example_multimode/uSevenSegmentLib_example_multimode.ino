/**
 * Really tiny library to basic 7 segments displays
 *
 *
 * Functionality example
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
 * @see <a href="https://www.foroelectro.net/librerias-arduino-ide-f29/usevensegmentlib-libreria-arduino-para-controlar-d-t193.html">https://www.foroelectro.net/librerias-arduino-ide-f29/usevensegmentlib-libreria-arduino-para-controlar-d-t193.html</a>
 * @see <a href="mailto:naguissa@foroelectro.net">naguissa@foroelectro.net</a>
 */
#include "Arduino.h"
#include "uSevenSegmentLib.h"

#ifdef _VARIANT_ARDUINO_STM32_
	int pins[8] = {D4, D5, D6, D7, D8, D9, D10, D11};
	int muxes[3] = {D12, D13};
#else
	int pins[8] = {2, 3, 4, 5, 6, 7, 8, 9};
	int muxes[3] = {10, 11};
#endif

uSevenSegmentLib sevenSegments(2, pins, muxes);

bool led_status = true;
char mode = 0;

long int i = -110;
unsigned long lastTime = 0;
unsigned long lastLedTime = 0;
unsigned long actTime;

void setup() {
	delay (2000);
	Serial.begin(9600);
	Serial.println("Serial OK");
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, led_status);
	mode = 0;
	Serial.print("Loading text...");
	showTextMessage();
	Serial.println(" ok");
	lastTime = millis();
}

void showTextMessage() {
    char message[] = "uSevenSegmentLib by Naguissa. https://github.com/Naguissa/uSevenSegmentLib";
    sevenSegments.setText(message);
}


void newNumber() {
	i++;
	sevenSegments.setInteger(i);
	Serial.println(i);
}

void loop() {
	sevenSegments.interruptLoop();
	actTime = millis();
	
    if (lastLedTime + 1000 < actTime) {
	    led_status = !led_status;
	    digitalWrite(LED_BUILTIN, led_status);
	    lastLedTime = actTime;
    }
	
    if (mode == 0) { // show text
	    if (lastTime + 30000 < actTime) {
	        mode = 1;
	        i = -110;
			Serial.print("Loading number WITHOUT zerofill...");
	        sevenSegments.zeroFill(true);
	        newNumber();
			Serial.println(" ok");
	        lastTime = actTime;
        }
    }
    if (mode == 1) { // show number no zerofill
	    if (lastTime + 750 < actTime) {
	        newNumber();
	        lastTime = actTime;
        }
	    if (i > 110) {
	        i = -110;
	        mode = 2;
			Serial.print("Loading number WITH zerofill...");
	        sevenSegments.zeroFill(true);
	        newNumber();
			Serial.println(" ok");
	        lastTime = actTime;
        }
    
    }
    if (mode == 2) { // show number zerofill
	    if (lastTime + 750 < actTime) {
	        newNumber();
	        lastTime = actTime;
        }
	    if (i > 110) {
	        mode = 0;
			Serial.print("Loading text...");
			showTextMessage();
			Serial.println(" ok");
	        lastTime = millis();
        }
    }
}

