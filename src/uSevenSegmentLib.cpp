/**
 * \class uSevenSegmentLib
 * \brief Really tiny library to basic 7 segments displays
 *
 * 7 segment displays directly controlled from microcontroller
 *
 * Common anode and common cathode allowed
 *
 * Uses multiplexed displays for driving as much displays as it can with minimum pins.
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
 * @see <a href="https://github.com/Naguissa/uSevenSegmentLib">https://github.com/Naguissa/uSevenSegmentLib</a>
 * @see <a href="https://github.com/Naguissa/uactTimerLib">https://github.com/Naguissa/uactTimerLib</a> - Useful to use this library with a actTimer
 * @see <a href="https://www.foroelectro.net/librerias-arduino-ide-f29/usevensegmentlib-libreria-arduino-para-controlar-d-t193.html">https://www.foroelectro.net/librerias-arduino-ide-f29/usevensegmentlib-libreria-arduino-para-controlar-d-t193.html</a>
 * @see <a href="mailto:naguissa@foroelectro.net">naguissa@foroelectro.net</a>
 * @version 2.0.0
 */
#include <Arduino.h>
#include "uSevenSegmentLib.h"

/**
 * \brief Static variable assignment to NULL
 */
uSevenSegmentLib * uSevenSegmentLib::instance = NULL;


/**
 * \brief Constructor
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
 *    -- d --  .
 *
 *  0b.gfedcba
 *
 * *Pins definition:*
 *  
 *  Same order as binary representation: {., g, f, e, d, c, b, a}
 *
 * *Muxes definition:*
 *  
 *  Left to right: {digit at left, digit at middle, digit at right....}

 
 
 
 
 * @param displays Number of displays
 * @param pins array of pins {dot, g, f, e, d, c, b, a}
 * @param muxes array of common pin for each display {digit at left, digit at middle, digit at right....}
 * @param freq Optional. Aproximate refresh delay for processing all displays (for all digits, will be multiplied by digits to calculate end result)
 * @param common_anode Optional. Set true to change to common_anode displays
 */
uSevenSegmentLib::uSevenSegmentLib(const uint8_t displays, int pins[8], int muxes[], uint8_t freq, bool common_anode) {
	_displays = displays;
	_delay_ms = (unsigned int) 1000 / freq / displays;
	// Get memory for all multiple displays muxes
	_muxes = (int *) malloc(displays * sizeof(int));
    // Common anode? Invert values
	_common_anode = common_anode;
	uint8_t i;
	// Process pins
	for (i = 0; i < 8; i++) {
		_pins[i] = pins[i];
		pinMode(pins[i], OUTPUT);
		USEVENSEGMENTLIB_YIELD
	}
	// Process muxes
	_muxes = (int *) malloc(displays * sizeof(int));
	for (i = 0; i < displays; i++) {
		_muxes[i] = muxes[i];
		pinMode(_muxes[i], OUTPUT);
		USEVENSEGMENTLIB_YIELD
	}
}

/**
 * \brief Reserve internal memory
 *
 * Frees existing memory if needed and reserves new indicated one.
 *
 * Also checks that minimum reserved memory is the same as displays
 *
 * @param number of bytes to reserve
 */
void uSevenSegmentLib::_reserveMemory(const uint8_t bytes) {
    if (_values != NULL) {
        free(_values);
    }
	_values = (uint8_t *) malloc(((bytes < _displays ? _displays : bytes) + 1)* sizeof(uint8_t));
}


/**
 * \brief Calculates how much bytes you need to store a long int.
 *
 * Takes in mind if negative sign needs to be added.
 *
 * Also checks that size is at least the same as displays.
 *
 * @param number to calculate its needed bytes
 */
uint8_t uSevenSegmentLib::_sizeOfCurrentLong(long int n) {
    uint8_t r = 1;
    if (n < 0) {
        n = -n;
        r++;
    }
    while (n > 9) {
        n /= 10;
        r++;
    }
    return (r < _displays ? _displays : r);
}


/**
 * \brief Sets a number to all displays
 *
 * @param number to be set
 */
void uSevenSegmentLib::setInteger(long int number) {
    _valuesLength = _sizeOfCurrentLong(number);
    _reserveMemory(_valuesLength);

	int i;

    uint8_t current;
	// Calculate numbers
	if (number == 0) {
		if (_zeroFill) {
	        // Clean extra visible displays if any
	        current = uSevenSegmentLib::char2out(0);
	        current = USEVENSEGMENTLIB_EFFECTIVE(current);
	        for (i = _valuesLength; i >= 0; i--) {
		        _values[i] = current;
	        }
	    } else {
		    current = uSevenSegmentLib::char2out(0);
		    _values[_valuesLength - 1] = USEVENSEGMENTLIB_EFFECTIVE(current);
		    
	        // Clean extra visible displays if any
    	    current = USEVENSEGMENTLIB_EFFECTIVE(USEVENSEGMENTLIB_ALL_OFF);
	        for (i = _valuesLength - 2; i >= 0; i--) {
		        _values[i] = current;
	        }
	    }
	} else {
    	long int _abs = abs(number);
		for (i = _valuesLength - 1; i >= 0 && _abs > 0; i--) {
			current = uSevenSegmentLib::char2out(_abs % 10);
			_values[i] = USEVENSEGMENTLIB_EFFECTIVE(current);
			_abs = _abs / 10;
    		USEVENSEGMENTLIB_YIELD
		}
		if (_zeroFill) {
	        // Clean extra visible displays if any
	        current = uSevenSegmentLib::char2out(0);
	        current = USEVENSEGMENTLIB_EFFECTIVE(current);
	        for (; i >= 0; i--) {
		        _values[i] = current;
        		USEVENSEGMENTLIB_YIELD
	        }
	        // Add negative sign at the begining
		    if (number < 0) {
			    current = uSevenSegmentLib::char2out('-');
			    _values[0] = USEVENSEGMENTLIB_EFFECTIVE(current);
		    }
	    } else {
		    // Negative sign?
		    if (number < 0) {
			    current = uSevenSegmentLib::char2out('-');
			    _values[i] = USEVENSEGMENTLIB_EFFECTIVE(current);
			    i--;
		    }
	        // Clean extra visible displays if any
    	    current = USEVENSEGMENTLIB_EFFECTIVE(USEVENSEGMENTLIB_ALL_OFF);
	        for (; i >= 0; i--) {
		        _values[i] = current;
        		USEVENSEGMENTLIB_YIELD
	        }
        }
	}
	_currentDisplay = 0;
	_currentValue = 0;
	_currentLoop = 0;
}

/**
 * \brief Sets a char array to all displays
 *
 * @param str to be set
 */
void uSevenSegmentLib::setText(const char str[]) {
    // We don't take in mind the dots. It would use few less memory if checked
    _valuesLength = strlen(str);
    uint8_t size = (_valuesLength < _displays ? _displays : _valuesLength);
    _reserveMemory(size);

	int i;
    uint8_t current;

	// Fill array with translated data
	for (i = 0; i < _valuesLength; i++) {
		// Check if next is a dot:
		if (i+1 < _valuesLength && (_values[i+1] == '.'|| _values[i+1] == ',')) { // Comma as dot
		    current = uSevenSegmentLib::char2out(str[i]) | USEVENSEGMENTLIB_DOT;
		    _values[i] = USEVENSEGMENTLIB_EFFECTIVE(current);
		    i++;
	    } else {
		    current = uSevenSegmentLib::char2out(str[i]);
		    _values[i] = USEVENSEGMENTLIB_EFFECTIVE(current);
		}
		USEVENSEGMENTLIB_YIELD
	}
	// Clean remaining  visible displays if str is shorter than displays
	for (; i < size; i++) {
	    current = USEVENSEGMENTLIB_ALL_OFF;
	    _values[i] = USEVENSEGMENTLIB_EFFECTIVE(current);
		USEVENSEGMENTLIB_YIELD
	}
	_currentDisplay = 0;
	_currentValue = 0;
	_currentLoop = 0;
}



/**
 * \brief Sets Zerofill on/off
 *
 * When enabled all leading digits are light as zero.
 *
 * @param bool zf If true, leading zeros will be displayed
 */
void uSevenSegmentLib::zeroFill(bool zf) {
	_zeroFill = zf;
}


/**
 * \brief Main public interrupt when using a actTimer
 *
 * Calls private interrupt
 */

void uSevenSegmentLib::interrupt() {
	instance->_interrupt();
}


/**
 * \brief Main public interrupt when using in a loop (manual calls)
 *
 * Checks millis and calls private interrupt if needed
 */
void uSevenSegmentLib::interruptLoop(void) {
	unsigned long actTime = millis();
	if (actTime < _delay_ms || _lastTime < actTime - _delay_ms) {
	    _lastTime = actTime;
	    _interrupt();
    }
}


/**
 * \brief Main private interrupt loop
 *
 * Refreshes all 8-segment digits
 */
void uSevenSegmentLib::_interrupt(void) {
    // Skip if not initialized
    if (_values == NULL) {
        return;
    }

	digitalWrite(_muxes[_currentDisplay], _common_anode);
	_currentDisplay = (_currentDisplay + 1) % _displays;

	uint8_t current = _values[_currentValue + _currentDisplay];
	digitalWrite(_pins[0], current & 0b10000000);
	digitalWrite(_pins[1], current & 0b01000000);
	digitalWrite(_pins[2], current & 0b00100000);
	digitalWrite(_pins[3], current & 0b00010000);
	digitalWrite(_pins[4], current & 0b00001000);
	digitalWrite(_pins[5], current & 0b00000100);
	digitalWrite(_pins[6], current & 0b00000010);
	digitalWrite(_pins[7], current & 0b00000001);

	digitalWrite(_muxes[_currentDisplay], !_common_anode);
	
	_currentLoop++;
	if (_valuesLength > _displays && _currentLoop > 2000 / _delay_ms / _displays) {
	    _currentLoop = 0;
	    _currentValue = (_currentValue + 1) % (_valuesLength - _displays);
    }
	
}

// In code to save RAM
uint8_t uSevenSegmentLib::char2out(char in) {
    // All to caps or 0-9 value
    if (in >= 'a' && in <= 'z') in = in - 'a' + 'A';
    else if (in >= '0' && in <= '9') in = in - '0';
    
    switch (in) {
        case 0: return 0b00111111;
        case 1:
        case '!': return 0b00000110;
        case 2:
        case 'Z': return 0b01011011;
        case 3: return 0b01001111;
        case 4: return 0b01100110;
        case 5: 
        case 'S': return 0b01101101;
        case 6: return 0b01111101;
        case 7: return 0b00000111;
        case 8: return 0b01111111;
        case 9: return 0b01101111;
        case 'A': return 0b01110111;
        case 'B': return 0b01111100;
        case 'C':
        case 135: // ç
        case 128: return 0b00111001; // Ç
        case 'D': return 0b01011110;
        case 'E': return 0b01111001;
        case 'F': return 0b01110001;
        case 'G': return 0b00111101;
        case 'H':
        case 'X': return 0b01110110;
        case 'I': return 0b00110000;
        case 'J': return 0b00011110;
        case 'K': return 0b01110101;
        case 'L': return 0b00111000;
        case 'M': return 0b00110111;
        case 'N': return 0b01010100;
        case 164: // ñ
        case 165: return 0b01010101; // Ñ
        case 'O': return 0b01011100;
        case 'P': return 0b01110011;
        case 'Q': return 0b01100111;
        case 'R': return 0b01010000;
        case 'T': return 0b01111000;
        case 'U': return 0b00111110;
        case 'V': return 0b00011100;
        case 'W': return 0b01101010;
        case 'Y': return 0b01101110;
        case '-': return 0b01000000;
        case '_': return 0b00001000;
        case 167: return 0b01100011; // º
        case '/': return 0b01010010;
        case ':': return 0b00001001;
        case '=': return 0b01001000;
        case ' ': return 0b00000000;
//        case '?': return 0b01010011; // Default, commented
    }
    return USEVENSEGMENTLIB_QUESTIONMARK;
}



