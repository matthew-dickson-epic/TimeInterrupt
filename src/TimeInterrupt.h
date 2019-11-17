/*
Name: TimeInterrupt
Author: Matthew Dickson <matthewdickson.code@gmail.com>
Created: October 8, 2019
Modified: November 17, 2019
Description: This library adds the ability to create timer-based interrupts on common Arduino compatible microcontrollers.
	The library consists of an addInterrupt() function which enables the user to add a void-type function as a periodically
	called interrupt service routine. And a removeInterrupt() function is also available to remove an ISR based on its
	callback function.
*/

#ifndef TimeInterrupt_h
#define TimeInterrupt_h

#include "Arduino.h"
#include "avr/InterruptList.h"

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) || defined(__AVR_ATmega32U4__)
//Do nothing
#elif defined(OVERRIDE_TIME_INTERRUPT)
#warning": overriding TimeInterrup.h's protections may cause issues with the performance of your code"
#else
#error": The microcontroller you are attempting to program is not compatable with this library."
#error": Compatable: ATmega168/328P/2560/1280/32U4, ATtiny25/45/85	add '#define OVERRIDE_TIME_INTERRUPT' to the top of your code to attempt compatability. (probably won't work well, but you can try!)"
#endif

extern interrupt_list TimeInterrupt;
      
#endif