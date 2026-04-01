/**
 * \mainpage
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
/** \file uSevenSegmentLib.h
 *   \brief uSevenSegmentLib header file
 */
#ifndef _uSevenSegmentLib_
	/**
	 * \brief Prevent multiple inclussion
	 */
	#define _uSevenSegmentLib_

	#include "Arduino.h"
	
	/**
	 * \brief Questionmark, used also for unknown characters
	 */
	#define USEVENSEGMENTLIB_QUESTIONMARK 0b01010011
	
	/**
	 * \brief Dot
	 */
	#define USEVENSEGMENTLIB_DOT 0b10000000
	
	/**
	 * \brief All segments off
	 */
	#define USEVENSEGMENTLIB_ALL_OFF 0b00000000


	/**
	 * \brief Macro for dealing with common_anode
	 */
	#define USEVENSEGMENTLIB_EFFECTIVE(val) (_common_anode ? (val xor 0b11111111) : val)
	
	
	
    #if defined(ARDUINO_ARCH_ESP8266) or defined(ARDUINO_ARCH_ESP32)
		/**
		 * \brief ESP8266 or ESP32, yield to don't block ESP functionality.
		 *
		 * When this library is used in other MCUs this is simply removed by the preprocessor
		 */
		#define USEVENSEGMENTLIB_YIELD yield();
    #else
			#define USEVENSEGMENTLIB_YIELD
    #endif	
	

	class uSevenSegmentLib {
		public:
			// Constructors
			uSevenSegmentLib(const uint8_t, int[8], int [], uint8_t = 25, bool = false);

			void setInteger(long int);
			void setText(const char []);
			
			void zeroFill(bool);

			static void interrupt(void);
			void interruptLoop(void);
			static uSevenSegmentLib *instance;
			
			static uint8_t char2out(char);
			

		private:
			void _interrupt(void);
			void _reserveMemory(uint8_t);
            uint8_t _sizeOfCurrentLong(long int);

			uint8_t _displays;
			uint8_t _valuesLength;
			uint8_t *_values;
			int *_muxes;
			int _pins[8];
			uint8_t _delay_ms = 50;
			bool _zeroFill = false;
			bool _common_anode = false;

			volatile uint8_t _currentDisplay = 0;
			volatile uint8_t _currentValue = 0;
			volatile uint8_t _currentLoop = 0;
			unsigned long _lastTime = 0;
			
	};
#endif

