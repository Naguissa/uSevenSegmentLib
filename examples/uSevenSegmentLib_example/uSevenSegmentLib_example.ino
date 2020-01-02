/**
 * Really tiny library to basic 7 segments displays
 *
 * Functionality example
 *
 * @see <a href="https://github.com/Naguissa/uSevenSegmentLib">https://github.com/Naguissa/uSevenSegmentLib</a>
 * @see <a href="https://github.com/Naguissa/uTimerLib">https://github.com/Naguissa/uTimerLib</a> - Needed dependecy
 * @see <a href="https://www.foroelectro.net/librerias-arduino-ide-f29/usevensegmentlib-libreria-arduino-para-controlar-d-t193.html">https://www.foroelectro.net/librerias-arduino-ide-f29/usevensegmentlib-libreria-arduino-para-controlar-d-t193.html</a>
 * @see <a href="mailto:naguissa@foroelectro.net">naguissa@foroelectro.net</a>
 */
#include "Arduino.h"
#include "uSevenSegmentLib.h"

#ifdef _VARIANT_ARDUINO_STM32_
	int pins[8] = {D4, D5, D6, D7, D8, D9, D10, D11};
	int muxes[3] = {D12, D13, D14};
#else
	int pins[8] = {1, 2, 3, 4, 5, 6, 7, 8};
	int muxes[3] = {9, 10, 11};
#endif

uSevenSegmentLib sevenSegments(3, pins, muxes);
bool led_status = true;


int i = 0;

void setup() {
	delay (2000);
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, led_status);

	sevenSegments.set(i);
	sevenSegments.attachInterrupt();
}



void loop() {
	delay(1000);
	i = (i + 1) % 256;
	sevenSegments.set(i);
	Serial.println((unsigned int) i);
	led_status = ~led_status;
	digitalWrite(LED_BUILTIN, led_status);
}

