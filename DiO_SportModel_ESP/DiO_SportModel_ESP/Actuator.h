// Actuator.h

#ifndef _ACTUATOR_h
#define _ACTUATOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#pragma region Headers

#include "ApplicationConfiguration.h"

#pragma endregion


class ActuatorClass
{
 protected:


 public:
	void configure();

	void setValue(uint8 value);
};

extern ActuatorClass Actuator;

#endif

