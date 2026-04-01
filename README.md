# Arduino, ESP8266 and STM32 7-segment multiplexed displays library - uSevenSegmentLib #

Master status:  ![Arduino CI](https://github.com/Naguissa/uSevenSegmentLib/workflows/arduino_ci/badge.svg)



## What is this repository for? ##

Really tiny library to control multiplexed basic 7 segments displays.

It can work with only a display but it's intended to use more than one and drive them multiplexed.

If you want to use only one display you can do it without flickering, without using timers and using less code driving it directly.


## How do I get set up? ##

You can get it from Arduino libraries directly, searching by uSevenSegmentLib.

For manual installation:

 * Get the ZIP from releases link: https://github.com/Naguissa/uSevenSegmentLib/releases
 * Rename to uSevenSegmentLib.zip
 * Install library on Arduino


## Documentation and extras ##

You can find all documentation and extras in this repository: https://github.com/Naguissa/uSevenSegmentLib_doc_and_extras

You can read documentation online here: https://naguissa.github.io/uSevenSegmentLib_doc_and_extras/


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

**void uSevenSegmentLibObject.setInteger(long int number);**

Parameters:
 - number: Number to display. It also accept negative numbers.

**void uSevenSegmentLibObject.setText(const char textArray[]);**

Parameters:
 - textArray: Char array with desired text. It will be scrolled if needed.
 

### Interrupt: ###

This library needs a constant call to refresh displays. It can be archieved using intyerrupts but also calling it from your "loop" function, if it's run very often.

To call it in loop you can call:
 * **void uSevenSegmentLibObject.interruptLoop();**


You can use interrupts using:

 * **static void uSevenSegmentLib::interrupt();**

In this scenario you can use uTimerLib, https://github.com/Naguissa/uTimerLib

	```TimerLib.setInterval_us(uSevenSegmentLib::interrupt, 10000);```




### Extra: Leading zero fill ###

void uSevenSegmentLib::zeroFill(bool zf);

Parameters:
 - zf: If true, leading zeros will be displayed instead turning off that displays when numbers are shown.


## Examples ##

Included on example folder, available on Arduino IDE.


## Who do I talk to? ##

 * [Naguissa](https://github.com/Naguissa)
 * https://www.foroelectro.net/electronica-digital-microcontroladores-f8/usevensegmentlib-libreria-arduino-para-controlar-d-t193.html
 * https://www.naguissa.com


## Contribute ##

Any code contribution, report or comment are always welcome. Don't hesitate to use GitHub for that.


 * You can sponsor this project using GitHub's Sponsor button: https://github.com/Naguissa/uSevenSegmentLib
 * You can make a donation via PayPal: https://paypal.me/foroelectro


Thanks for your support.


Contributors hall of fame: https://www.foroelectro.net/hall-of-fame-f32/contributors-contribuyentes-t271.html
