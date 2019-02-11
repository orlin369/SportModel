// Indications.h

#ifndef _INDICATIONS_h
#define _INDICATIONS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class IndicationsClass
{
 protected:


 public:
	void configure();
};

extern IndicationsClass Indications;

#endif

