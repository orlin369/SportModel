// DeviceConfiguration.h

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

#ifndef _DEVICECONFIGURATION_h
#define _DEVICECONFIGURATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#pragma region Headers

#include <FS.h>

#include <ArduinoJson.h>

#include "ApplicationConfiguration.h"

#include "DebugPort.h"

#pragma endregion

#pragma region Structures

/** @brief Device configuration structure
 *  @author Orlin Dimitrov <or.dimitrov@polygonteam.com>
 *  @version 1.0
 *
 *  Device configuration structure, it hold configuration parameters values.
 */
typedef struct {
	String STASSID; ///< SSID of the near AP.
	String STAPassword; ///< Password of the near AP.
	String APPassword;
	String HTTPUsername;
	String HTTPPassword;
	bool HTTPAuthentication;
	String DeviceName;

#ifdef ENABLE_CAYENNE_MODE

	String CayenneUsername;
	String CayennePassword;
	String CayenneClientID;

#endif // ENABLE_CAYENNE_MODE

	
} DeviceConfiguration_t;

#pragma endregion

#pragma region Functions

/** @brief Load configuration.
 *  @param fileSystem FS, File system of the device.
 *  @return boolean, Successful loading.
 */
bool load_device_configuration(FS* fileSystem);

/** @brief Default configuration.
 *  @param fileSystem FS, File system of the device.
 *  @return Void
 */
void set_default_device_configuration(FS* fileSystem);

/** @brief Save configuration.
 *  @param fileSystem FS, File system of the device.
 *  @return boolean, Successful saving.
 */
bool save_device_configuration(FS* fileSystem);

#pragma endregion

/* @brief Singelton device configuration instance. */
extern DeviceConfiguration_t DeviceConfiguration;


#endif

