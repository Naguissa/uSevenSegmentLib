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
 * Library depends on uTimerLib library, https://github.com/Naguissa/uTimerLib
 *
 * @see <a href="https://github.com/Naguissa/uSevenSegmentLib">https://github.com/Naguissa/uSevenSegmentLib</a>
 * @see <a href="https://github.com/Naguissa/uTimerLib">https://github.com/Naguissa/uTimerLib</a> - Needed dependecy
 * @see <a href="https://www.foroelectro.net/librerias-arduino-ide-f29/usevensegmentlib-libreria-arduino-para-controlar-d-t193.html">https://www.foroelectro.net/librerias-arduino-ide-f29/usevensegmentlib-libreria-arduino-para-controlar-d-t193.html</a>
 * @see <a href="mailto:naguissa@foroelectro.net">naguissa@foroelectro.net</a>
 * @version 1.1.0
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
	#include "uTimerLib.h"

	class uSevenSegmentLib {
		public:
			// Constructors
			uSevenSegmentLib(const unsigned char, int[8], int *, unsigned int = 40, bool = false);

			void set(long int);
			long int get();
			void attachInterrupt();

			void zeroFill(bool);

			static void interrupt(void);

			static uSevenSegmentLib *instance;

		private:
			void _interrupt(void);

			unsigned char _displays;
			// Value 200 means digit disabled.
			// Value 201 means minus sign
			// Value 1X is X with dot
			unsigned char *_values;
			int *_muxes;
			int _pins[8];
			int _freq = 40;
			bool _zeroFill = false;

			// Originally defined as common_cathode. Will XOR if common anode

			/**
			 * Segments used for every symbol:
			 *
			 * 0 11111100
			 * 1 01100000
			 * 2 11011010
			 * 3 11110010
			 * 4 01100110
			 * 5 10110110
			 * 6 00111110
			 * 7 11100000
			 * 8 11111110
			 * 9 11100110
			 * . 00000001;
			 * - 00000010;
			 *   00000000;
			 */
			unsigned char _mask[10] = {B11111100, B01100000, B11011010, B11110010, B01100110, B10110110, B00111110, B11100000, B11111110, B11100110};
			unsigned char _dot = B00000001;
			unsigned char _minus = B00000010;
			bool _off = B00000000;
			bool _offB = B00000000;
			bool _onB = B11111111;

			unsigned char _current = 0;
			unsigned char _last = 0;
	};
#endif

