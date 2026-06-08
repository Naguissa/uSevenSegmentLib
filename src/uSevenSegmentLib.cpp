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
 * @version 2.0.1
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
		pinMode(_pins[i], OUTPUT);
		USEVENSEGMENTLIB_YIELD
	}
	// Process muxes
	_muxes = (int *) malloc(displays * sizeof(int));
	for (i = 0; i < displays; i++) {
		_muxes[i] = muxes[i];
		pinMode(_muxes[i], OUTPUT);
		USEVENSEGMENTLIB_YIELD
	}
	// Internal delay to scroll screens
    _loops_to_wait = 2000 / _delay_ms / _displays;
    if (_loops_to_wait < 10) _loops_to_wait = 10;
    USEVENSEGMENTLIB_YIELD
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
	unsigned int i = 0;
    unsigned int o = 0;
    unsigned int len = 0;
    uint8_t current;

	// strlen skipping commas and dots to save memory
    for(; str[len]; len++) {
        if (str[len] != '.' && str[len] != ',') o++;
    }

    // Reserve needed space for all text.
    // If text is shorter than number of screens use this number as minimum size.
    _valuesLength = o > 0 ? o : 0;
    i = (_valuesLength < _displays ? _displays : _valuesLength);
    _reserveMemory(i);
return;

	// Fill array with translated data
	for (i = 0, o = 0; i < len; o++) {
	    current = uSevenSegmentLib::char2out(str[i]);
	    i++;
		// Check if next is a dot:
		if (i < _valuesLength && (str[i] == '.' || str[i] == ',')) { // Comma or dot
		    current = current | USEVENSEGMENTLIB_DOT;
		    i++;
	    }
	    _values[o] = USEVENSEGMENTLIB_EFFECTIVE(current);
		USEVENSEGMENTLIB_YIELD
	}
	// Clean remaining  visible displays if str is shorter than displays
	for (; o < _displays; o++) {
	    current = USEVENSEGMENTLIB_ALL_OFF;
	    _values[o] = USEVENSEGMENTLIB_EFFECTIVE(current);
		USEVENSEGMENTLIB_YIELD
	}
	// Reset display handling
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

	digitalWrite(_muxes[_currentDisplay], !_common_anode);
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

	digitalWrite(_muxes[_currentDisplay], _common_anode);
	_currentLoop++;
	if (_valuesLength > _displays && _currentLoop > _loops_to_wait) {
	    _currentLoop = 0;
	    _currentValue = (_currentValue + 1) % (_valuesLength - _displays);
    }
	
}

// Resilve linker issue
extern constexpr uint8_t uSevenSegmentLib::charTable[36];

// In code and flash to save RAM
uint8_t uSevenSegmentLib::char2out(uint8_t in) {
    // Numeric values
    if (in <= 9) return uSevenSegmentLib::charTable[in];
    // Numbers in ASCII
    if (in >= '0' && in <= '9') return uSevenSegmentLib::charTable[in - '0'];
    // Common letters in ASCII
    if (in >= 'a' && in <= 'z') return uSevenSegmentLib::charTable[in - 87];  // same as in = in - 'a' + 10
    if (in >= 'A' && in <= 'Z') return uSevenSegmentLib::charTable[in - 55]; // same as in = in - 'A' + 10
    // Add some ASCII symbols
    switch (in) {
        case 32: // space
        case 255: // non-breaking space
            return 0b00000000;  // space

        case 33: // !
        case 173: // ¡
            return 0b00000110;  // same as 1
        
        case 34: return 0b00100010; // "

        case 39: // '
        case 96: // `
        case 239: // '
            return 0b00000010;
    
        case 40: // (
        case 91: // [
            return 0b00111001;
            
        case 41: // )
        case 93: // ]
            return 0b00001111;
            
        case 45: // -
        case 126: // ~
            return 0b01000000;

        case 47: return 0b01010010; // / (slash)

        case 58: // :
        case 59: // ;
            return 0b00001001;
            
        case 61: return 0b01001000; // =
        case 92: return 0b01100100; // \ (backslash)
        case 95: return 0b00001000; // _
        
        case 128: // Ç
        case 135: // ç
             return 0b00111001; // C

        case 164: // ñ
        case 165: // Ñ
             return 0b01010100; // N
        
        case 166: // ª
        case 167: // º
        case 248: // º (degree)
            return 0b01100011;
            
        case 168: return 0b01011010; // ¿
    }

    // Any other? Question mark
    return USEVENSEGMENTLIB_QUESTIONMARK;
}



