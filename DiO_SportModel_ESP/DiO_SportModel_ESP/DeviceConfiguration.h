// DeviceConfiguration.h

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
	String CayenneUsername;
	String CayennePassword;
	String CayenneClientID;
	String HTTPUsername;
	String HTTPPassword;
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

