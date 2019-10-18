/**
 * Really tiny library to basic 7 segments displays
 *
 * 7 segment displays directly controlled from microcontroller
 *
 * Common anode and common cathode allowed.
 *
 * Uses multiplexed displays for driving as much displays as it can with minimum pins.
 *
 *
 * @copyright Naguissa
 * @author Naguissa
 * @email naguissa.com@gmail.com
 * @version 1.0.1
 * @created 2018-01-27
 */
#include <Arduino.h>
#include "uSevenSegmentLib.h"

// Static variable assignment
uSevenSegmentLib * uSevenSegmentLib::_instance = NULL;


/**
 * Constructor
 *
 * @param unsigned char displays Number of displays
 * @param int[8] array of pins {a, b, c, d, e, f, g, dot}
 * @param int* array of pins for each display
 */
uSevenSegmentLib::uSevenSegmentLib(unsigned char displays, int pins[8], int *muxes) {
	_displays = displays;
	unsigned char i;
	for (i = 0; i < 8; i++) {
		_pins[i] = pins[i];
		pinMode(pins[i], OUTPUT);
	}
	// Get memory for all digits
	_values = (unsigned char *) malloc(_displays);
	// Get memory for all multiple displays muxes
	_muxes = (int *) malloc(_displays);
	for (i = 0; i < _displays; i++) {
		_muxes[i] = muxes[i];
		pinMode(muxes[i], OUTPUT);
	}
}

/**
 * Constructor
 *
 * @param unsigned char displays Number of displays
 * @param int[8] array of pins {a, b, c, d, e, f, g, dot}
 * @param int* array of pins for each display
 * @param bool common_anode Set true to change to common_anode displays
 */
uSevenSegmentLib::uSevenSegmentLib(unsigned char displays, int pins[8], int *muxes, bool common_anode) {
	if (common_anode) {
		for (unsigned char i = 0; i < 10; i++) {
			_mask[i] = _mask[i] xor B11111111;
		}
		_dot = ~_dot;
		_minus = ~_minus;
		_onB = _offB;
		_offB = ~_offB;
		_off = ~_off;
	}
	uSevenSegmentLib(displays, pins, muxes);
}


/**
 * Constructor
 *
 * @param unsigned char displays Number of displays
 * @param int[8] array of pins {a, b, c, d, e, f, g, dot}
 * @param int* array of pins for each display
 * @param unsigned int Refresh frequency (for all digits, will be multiplied by digits to calculate end result)
 */
uSevenSegmentLib::uSevenSegmentLib(unsigned char displays, int pins[8], int *muxes, unsigned int freq) {
	_freq = freq;
	uSevenSegmentLib(displays, pins, muxes);
}

/**
 * Constructor
 *
 * @param unsigned char displays Number of displays
 * @param int[8] array of pins {a, b, c, d, e, f, g, dot}
 * @param int* array of pins for each display
 * @param unsigned int Refresh frequency (for all digits, will be multiplied by digits to calculate end result)
 * @param bool common_anode Set true to change to common_anode displays
 */
uSevenSegmentLib::uSevenSegmentLib(unsigned char displays, int pins[8], int *muxes, unsigned int freq, bool common_anode) {
	_freq = freq;
	uSevenSegmentLib(displays, pins, muxes, common_anode);
}




/**
 * Sets a number
 *
 * @param int number to be setted
 */
void uSevenSegmentLib::set(long int number) {
	long int _abs = abs(number);
	unsigned char i;

	// Clean -- turn off
	for (i = 0; i < _displays; i++) {
		_values[i] = _zeroFill ? _mask[0] : _off;
	}

	// Calculate numbers
	if (number == 0) {
		_values[0] = _mask[0];
	} else {
		for (i = 0; i < _displays && _abs > 0; i++) {
			_values[i] = _mask[_abs % 10];
			_abs = _abs / 10;
		}
		// Negative sign?
		if (number < 0) {
			if ((10 ^ (_displays - 1)) < (-number)) {
				_values[i] = _minus;
			}
		}
	}
}

/**
 * Gets stored number
 *
 * @return int number in memory
 */
long int uSevenSegmentLib::get() {
	unsigned char i;
	long int value = 0;
	unsigned char dotMask;
	unsigned char currentValue;
	if (_dot == B00000001) {
		dotMask = ~_dot;
	} else {
		dotMask = _dot;
	}
	int last = 1;

	// Calculate numbers
	for (i = 0; i < _displays; i++) {
		currentValue = _values[_displays - i] & dotMask;
		if (currentValue != _off && currentValue != _dot) {
			if (currentValue == _minus) {
				last = -1;
			} else {
				value = value * 10 + currentValue;
				break;
			}
		}
	}
	return value * last;
}

/**
 * Gets stored number
 *
 * @param bool zf If true, leading zeros will be displayed
 */
void uSevenSegmentLib::zeroFill(bool zf) {
	_zeroFill = zf;
}


/**
 * Attach Timer2 interrupt
 *
 * Needed for usual operation, but you can call loop manually instead
 */
void uSevenSegmentLib::attachInterrupt() {
	extern uTimerLib TimerLib;
	if (_instance == NULL) {
		uSevenSegmentLib::_instance = this;
		unsigned long int period_us = 1000000 / _freq / _displays;
		TimerLib.setInterval_us(uSevenSegmentLib::interrupt, period_us);
	}
}


/**
 * Main loop
 *
 * Refreshes all 8-segment digits
 */
void uSevenSegmentLib::interrupt() {
	_instance->_interrupt();
}


void uSevenSegmentLib::_interrupt(void) {
	digitalWrite(_muxes[_last], _onB); // In case of muxes, off works inverse

	if (_values[_current] != _off) {
		// _dot  and _minus included
		unsigned char current = _values[_current];

	/*
		digitalWrite(_pins[0], current & B10000000 xor _on);
		digitalWrite(_pins[1], current & B01000000 xor _on);
		digitalWrite(_pins[2], current & B00100000 xor _on);
		digitalWrite(_pins[3], current & B00010000 xor _on);
		digitalWrite(_pins[4], current & B00001000 xor _on);
		digitalWrite(_pins[5], current & B00000100 xor _on);
		digitalWrite(_pins[6], current & B00000010 xor _on);
		digitalWrite(_pins[7], current & B00000001 xor _on);
	*/
		digitalWrite(_pins[0], current & B10000000);
		digitalWrite(_pins[1], current & B01000000);
		digitalWrite(_pins[2], current & B00100000);
		digitalWrite(_pins[3], current & B00010000);
		digitalWrite(_pins[4], current & B00001000);
		digitalWrite(_pins[5], current & B00000100);
		digitalWrite(_pins[6], current & B00000010);
		digitalWrite(_pins[7], current & B00000001);

		digitalWrite(_muxes[_current], _offB);
	}
	_last = _current;
	_current = (_current + 1) % _displays; // In case of muxes, off works inverse
}
