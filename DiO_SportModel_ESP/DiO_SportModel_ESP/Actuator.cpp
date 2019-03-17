// 
// 
// 

#include "Actuator.h"

void ActuatorClass::configure()
{
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");

	// Config PWM Range.
	analogWriteRange(PWM_RANGE);

	// Config PWM frequency.
	analogWriteFreq(PWM_FREQ);

	// Shutdown the actuator.
	analogWrite(PIN_ACTUATOR, 0);
}

void ActuatorClass::setValue(uint8 value)
{
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");

	// Shutdown the actuator.
	analogWrite(PIN_ACTUATOR, value);
}

ActuatorClass Actuator;

