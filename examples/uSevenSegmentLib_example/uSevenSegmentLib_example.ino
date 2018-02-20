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
	Serial.begin(57600);
	Serial.println("Serial OK");

	#ifdef _VARIANT_ARDUINO_STM32_
		Serial.println("Board: STM32");
	#else
		Serial.println("Board: Other");
	#endif
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

