// DebugPort.h

#ifndef _DEBUGPORT_h
#define _DEBUGPORT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#pragma region Headers

/* Application configuration. */
#include "ApplicationConfiguration.h"

#pragma endregion

#pragma region Definitions

#ifdef EANBLE_DEBUG_OUT
#define DEBUGLOG(...) DEBUG_PORT.printf(__VA_ARGS__)
#else
#define DEBUGLOG(...)
#endif

#pragma endregion

#pragma region Functions

/** @brief Configure debug port.
 *  @return Void
 */
void configure_debug_port();

#pragma endregion

#endif

