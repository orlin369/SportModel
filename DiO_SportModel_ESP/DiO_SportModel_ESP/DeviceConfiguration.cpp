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

#include "DeviceConfiguration.h"

/* @brief Singelton device configuration instance. */
DeviceConfiguration_t DeviceConfiguration;

/** @brief Load configuration.
 *  @param fileSystem FS, File system of the device.
 *  @return boolean, Successful loading.
 */
bool load_device_configuration(FS* fileSystem) {
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");

	File configFile = fileSystem->open(CONFIG_FILE, "r");
	if (!configFile) {
		DEBUGLOG("Failed to open config file");
		return false;
	}

	size_t size = configFile.size();
	/*if (size > 1024) {
	DEBUGLOG("Config file size is too large");
	configFile.close();
	return false;
	}*/

	// Allocate a buffer to store contents of the file.
	std::unique_ptr<char[]> buf(new char[size]);

	// We don't use String here because ArduinoJson library requires the input
	// buffer to be mutable. If you don't use ArduinoJson, you may as well
	// use configFile.readString instead.
	configFile.readBytes(buf.get(), size);
	configFile.close();
	DEBUGLOG("191 JSON file size: %d bytes\r\n", size);
	DynamicJsonBuffer jsonBuffer(1024);
	//StaticJsonBuffer<1024> jsonBuffer;
	JsonObject& json = jsonBuffer.parseObject(buf.get());

	if (!json.success()) {
		DEBUGLOG("Failed to parse config file\r\n");
		return false;
	}
	
#ifdef EANBLE_DEBUG_OUT

	String temp;
	json.prettyPrintTo(temp);
	DEBUGLOG("%s\r\n", temp.c_str());

#endif

	DeviceConfiguration.STASSID = json["STASSID"].as<const char *>();
	DeviceConfiguration.STAPassword = json["STAPassword"].as<const char *>();	
	DeviceConfiguration.APPassword = json["APPassword"].as<const char *>();
	DeviceConfiguration.HTTPUsername = json["HTTPUsername"].as<const char *>();
	DeviceConfiguration.HTTPPassword = json["HTTPPassword"].as<const char *>();
	DeviceConfiguration.HTTPAuthentication = json["HTTPAuthentication"];
	DeviceConfiguration.DeviceName = json["DeviceName"].as<const char *>();
		
#ifdef ENABLE_CAYENNE_MODE

	DeviceConfiguration.CayenneUsername = json["CayenneUsername"].as<const char *>();
	DeviceConfiguration.CayennePassword = json["CayennePassword"].as<const char *>();
	DeviceConfiguration.CayenneClientID = json["CayenneClientID"].as<const char *>();

#endif // ENABLE_CAYENNE_MODE

	return true;
}

/** @brief Default configuration.
 *  @param fileSystem FS, File system of the device.
 *  @return Void
 */
void set_default_device_configuration(FS* fileSystem) {
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");

	DeviceConfiguration.STASSID = DEFAULT_STA_SSID;
	DeviceConfiguration.STAPassword = DEFAULT_STA_PASSWORD;
	DeviceConfiguration.APPassword = DEFAULT_AP_PASSWORD;
	DeviceConfiguration.HTTPUsername = DEFAULT_HTTP_USERNAME;
	DeviceConfiguration.HTTPPassword = DEFAULT_HTTP_PASSWORD;
	DeviceConfiguration.HTTPAuthentication = false;
	DeviceConfiguration.DeviceName = DEVICE_BRAND;

#ifdef ENABLE_CAYENNE_MODE

	DeviceConfiguration.CayenneUsername = DEFAULT_CAYENNE_USERNAME;
	DeviceConfiguration.CayennePassword = DEFAULT_CAYENNE_PASSWORD;
	DeviceConfiguration.CayenneClientID = DEFAULT_CAYENNE_CLIENT_ID;

#endif // ENABLE_CAYENNE_MODE

	save_device_configuration(fileSystem);
}

/** @brief Save configuration.
 *  @param fileSystem FS, File system of the device.
 *  @return boolean, Successful saving.
 */
bool save_device_configuration(FS* fileSystem) {
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");

	//flag_config = false;
	DynamicJsonBuffer jsonBuffer(512);

	//StaticJsonBuffer<1024> jsonBuffer;
	JsonObject& json = jsonBuffer.createObject();
	json["STASSID"] = DeviceConfiguration.STASSID;
	json["STAPassword"] = DeviceConfiguration.STAPassword;
	json["APPassword"] = DeviceConfiguration.APPassword;
	json["HTTPUsername"] = DeviceConfiguration.HTTPUsername;
	json["HTTPPassword"] = DeviceConfiguration.HTTPPassword;
	json["HTTPAuthentication"] = DeviceConfiguration.HTTPAuthentication;
	json["DeviceName"] = DeviceConfiguration.DeviceName;

#ifdef ENABLE_CAYENNE_MODE

	json["CayenneUsername"] = DeviceConfiguration.CayenneUsername;
	json["CayennePassword"] = DeviceConfiguration.CayennePassword;
	json["CayenneClientID"] = DeviceConfiguration.CayenneClientID;

#endif // ENABLE_CAYENNE_MODE

	File configFile = fileSystem->open(CONFIG_FILE, "w");
	if (!configFile) {
		DEBUGLOG("Failed to open config file for writing\r\n");
		configFile.close();
		return false;
	}

#ifdef EANBLE_DEBUG_OUT

	String temp;
	json.prettyPrintTo(temp);
	DEBUGLOG("%s\r\n", temp.c_str());

#endif // EANBLE_DEBUG_OUT

	json.printTo(configFile);
	configFile.flush();
	configFile.close();

	return true;
}
