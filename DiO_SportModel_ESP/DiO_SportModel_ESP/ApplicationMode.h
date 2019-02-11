// ApplicationMode.h

#ifndef _APPLICATIONMODE_h
#define _APPLICATIONMODE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

/** @brief Application operation mode. */
enum ApplicationMode : int
{
	Idle = 0, ///< Idle.
	NormalOperation, ///< Normal operation mode.
	Configuriation, ///< Configuration mode.
	Sleep,  ///< Sleep mode.
};

#endif

