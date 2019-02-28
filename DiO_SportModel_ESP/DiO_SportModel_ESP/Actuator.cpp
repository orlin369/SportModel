// 
// 
// 

#include "Actuator.h"

void ActuatorClass::configure()
{
	// Config PWM Range.
	analogWriteRange(PWM_RANGE);

	// Config PWM frequency.
	analogWriteFreq(PWM_FREQ);

	// Shutdown the actuator.
	analogWrite(PIN_ACTUATOR, 0);
}

void ActuatorClass::setValue(uint8 value)
{
	// Shutdown the actuator.
	analogWrite(PIN_ACTUATOR, value);
}

ActuatorClass Actuator;

