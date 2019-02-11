// 
// 
// 

#include "Indications.h"

void IndicationsClass::configure()
{
	pinMode(PIN_BUZZER, OUTPUT);
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
