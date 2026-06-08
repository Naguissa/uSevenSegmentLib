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
 * @version 2.0.1
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
			
			static uint8_t char2out(uint8_t);
			
			static constexpr uint8_t charTable[36] = {
                0b00111111, // 0-0
                0b00000110, // 1-1 (also !)
                0b01011011, // 2-2 (also Z)
                0b01001111, // 3-3
                0b01100110, // 4-4
                0b01101101, // 5-5 (also S)
                0b01111101, // 6-6
                0b00000111, // 7-7
                0b01111111, // 8-8
                0b01101111, // 9-9

                0b01110111, // 10-A
                0b01111100, // 11-B
                0b00111001, // 12-C
                0b01011110, // 13-D
                0b01111001, // 14-E
                0b01110001, // 15-F
                0b00111101, // 16-G
                0b01110110, // 17-H (alxo X)
                0b00110000, // 18-I
                0b00011110, // 19-J
                0b01110101, // 20-K
                0b00111000, // 21-L
                0b00110111, // 22-M
                0b01010100, // 23-N
                0b01011100, // 24-O
                0b01110011, // 25-P
                0b01100111, // 26-Q
                0b01010000, // 27-R
                0b01101101, // 28-S (also 5)
                0b01111000, // 29-T
                0b00111110, // 30-U
                0b00011100, // 31-V
                0b01101010, // 32-W
                0b01110110, // 33-X (alxo H)
                0b01101110, // 34-Y
                0b01011011  // 35-Z (also 2)
            };

		private:
			void _interrupt(void);
			void _reserveMemory(uint8_t);
            uint8_t _sizeOfCurrentLong(long int);

			uint8_t _displays;
			unsigned int _valuesLength;
			uint8_t *_values = NULL;
			int *_muxes;
			int _pins[8];
			uint8_t _delay_ms = 50;
			bool _zeroFill = false;
			bool _common_anode = false;

			volatile uint8_t _currentDisplay = 0;
			volatile unsigned int _currentValue = 0;
			volatile uint8_t _currentLoop = 0;
			uint8_t _loops_to_wait;
			unsigned long _lastTime = 0;
	};
#endif

