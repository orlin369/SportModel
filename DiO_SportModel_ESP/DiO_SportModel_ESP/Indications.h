// Indications.h

#ifndef _INDICATIONS_h
#define _INDICATIONS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#pragma region Headers

#include "ApplicationConfiguration.h"

#include "DebugPort.h"

#include "Notes.h"

#pragma endregion


class IndicationsClass
{
 protected:
	 void playNote(long frequency, long length);

 public:
	void configure();
	void playNormalOperationMode();
	void playConfigurationMode();
	void playShutdown();
	void playError();
	void playConnectedToInet();
	void playConnectedToService();
	void playDisconnectedFromInet();
	void playDisconnectedFromService();
};

extern IndicationsClass Indications;

#endif

