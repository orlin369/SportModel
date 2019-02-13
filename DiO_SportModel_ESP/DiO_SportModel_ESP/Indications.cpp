/*

Copyright (c) [2019] [Orlin Dimitrov]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include "Indications.h"

void IndicationsClass::configure()
{
	pinMode(PIN_BUZZER, OUTPUT);
	pinMode(PIN_LED, OUTPUT);
}

void IndicationsClass::playNote(long frequency, long length)
{
	long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
	// 1 second's worth of microseconds, divided by the frequency, then split in half since
	// there are two phases to each cycle
	long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
	// multiply frequency, which is really cycles per second, by the number of seconds to
	// get the total number of cycles to produce
	for (long index = 0; index < numCycles; index++)
	{ // for the calculated length of time...
		digitalWrite(PIN_BUZZER, HIGH); // write the buzzer pin high to push out the diaphram
		delayMicroseconds(delayValue); // wait for the calculated delay value
		digitalWrite(PIN_BUZZER, LOW); // write the buzzer pin low to pull back the diaphram
		delayMicroseconds(delayValue); // wait again or the calculated delay value

		ESP.wdtFeed();
	}
}

void IndicationsClass::playNormalOperationMode()
{
	// TODO: Create song.
}

void IndicationsClass::playConfigurationMode()
{
	// TODO: Create song.
}

void IndicationsClass::playShutdown()
{
	// TODO: Create song.
}

void IndicationsClass::playError()
{
	// TODO: Create song.
}

void IndicationsClass::playConnectedToInet()
{
	// TODO: Create song.
}

void IndicationsClass::playConnectedToService()
{
	// TODO: Create song.
}

void IndicationsClass::playDisconnectedFromInet()
{
	// TODO: Create song.
}

void IndicationsClass::playDisconnectedFromService()
{
	// TODO: Create song.
}

IndicationsClass Indications;
