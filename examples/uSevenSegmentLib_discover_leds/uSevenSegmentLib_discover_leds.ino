#include "Arduino.h"

// Reverse this to test common annode ones
#define ONE 1
#define ZERO 0

#ifdef _VARIANT_ARDUINO_STM32_
	int pins[8] = {D4, D5, D6, D7, D8, D9, D10, D11};
	int mux = D12;
#else
	int pins[8] = {1, 2, 3, 4, 5, 6, 7, 8};
	int mux = 9;
#endif



unsigned char i;




void setup() {
	pinMode(mux, OUTPUT);
	digitalWrite(mux, ZERO);

	for(i = 0; i < 8; i++) {
		pinMode(pins[i], OUTPUT);
	}
}

void loop() {
	for (i = 0; i < 8; i++) {
		digitalWrite(pins[i], ONE);
		delay(1500);
	}
	delay(3500);

	for (i = 0; i < 8; i++) {
		digitalWrite(pins[i], ZERO);
		delay(1500);
	}
	delay(5000);
}


