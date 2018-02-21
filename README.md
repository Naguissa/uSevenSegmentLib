# Arduino, ESP8266 and STM32 7-segment multiplexed displays library - uSevenSegmentLib #

## What is this repository for? ##

Really tiny library to control multiplexed basic 7 segments displays.

It can work with only a display but it's intended to use more than one and drive them multiplexed.

If you want to use only one display you can do it without flickering, without using timers and using less code driving it directly.

## Important: Dependencies ##

This library requires uTimerLib libary; it's available on IDE Library Manager and also here:

https://github.com/Naguissa/uTimerLib
https://github.com/Naguissa/uTimerLib/releases



## How do I get set up? ##

You can get it from Arduino libraries directly, searching by uSevenSegmentLib.

For manual installation:

 * Get the ZIP from releases link: https://github.com/Naguissa/uSevenSegmentLib/releases
 * Rename to uSevenSegmentLib.zip
 * Install library on Arduino

## Usage ##

Important: Remember to add resistors as needed, depending on your MCU voltage.

### Constructor: ###

uSevenSegmentLib::uSevenSegmentLib(unsigned char displays, int pins[8], int *muxes, unsigned int freq, bool common_anode);

Parameters:
 - displays: number of displays (7-segment modules)
 - pins: Array of all pins of the displays in {a, b, c, d, e, f, g, h} form. See extras folder for details.
 - muxes: Array of common anode/cathode, one by each display. Used to multiplex the displays.
 - freq: Optional. Desired refresh frequency. Default is 40 (Hz). This frequency will be multiplied by number of displays, so it's a "all displays" refresh rate.
 - common_anode: Optional. Set to true if you are using common_anode displays.

### Set a value: ###

void uSevenSegmentLib::set(long int number);

Parameters:
 - number: Number to display. It also accept negative numbers.

### Start displaying: ###

void uSevenSegmentLib::attachInterrupt();

Attaches internal timer interrupts to run multiplexed displays.


### Extra: Leading zero fill ###

void uSevenSegmentLib::zeroFill(bool zf);

Parameters:
 - zf: If true, leading zeros will be displayed instead turning off that displays.



## Typical usage: ##

 - You instantiate an object (usually a global scope object): uSevenSegmentLib sevenSegments(3, pins, muxes);
 - Then set any initial value on it on setup(): sevenSegments.set(0);
 - And attach interrupts to start working (still on setup()): sevenSegments.attachInterrupt();
 - Then, wherever you want, you can change displayed values at your will: sevenSegments.set(77);



## Examples ##

Included on example folder, available on Arduino IDE.

 - Three 7-segments counter example, that counts from 0 to 255.
 - One example to test connections to a 7-segment module; lights sequently from "a" to "h" segments. This example doesn't use library functions.

## Extra ##

Look in extras folder for a typical pinout and

## Who do I talk to? ##

 * [Naguissa](https://github.com/Naguissa)
 * https://www.foroelectro.net/electronica-digital-microcontroladores-f8/usevensegmentlib-libreria-arduino-para-controlar-d-t193.html
 * https://www.naguissa.com

