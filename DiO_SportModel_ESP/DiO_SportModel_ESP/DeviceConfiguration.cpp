// 
// 
// 

#include "DeviceConfiguration.h"

/* @brief Singelton device configuration instance. */
DeviceConfiguration_t DeviceConfiguration;

/** @brief Load configuration.
 *  @param fileSystem FS, File system of the device.
 *  @return boolean, Successful loading.
 */
bool load_device_configuration(FS* fileSystem) {
#ifndef RELEASE
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");
#endif // !RELEASE

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

#ifndef RELEASE
	String temp;
	json.prettyPrintTo(temp);
	DEBUGLOG("%s\r\n", temp.c_str());
#endif

	DeviceConfiguration.STASSID = json["STASSID"].as<const char *>();
	DeviceConfiguration.STAPassword = json["STAPassword"].as<const char *>();	
	DeviceConfiguration.APPassword = json["APPassword"].as<const char *>();
	DeviceConfiguration.CayenneUsername = json["CayenneUsername"].as<const char *>();
	DeviceConfiguration.CayennePassword = json["CayennePassword"].as<const char *>();
	DeviceConfiguration.CayenneClientID = json["CayenneClientID"].as<const char *>();
	DeviceConfiguration.HTTPUsername = json["HTTPUsername"].as<const char *>();
	DeviceConfiguration.HTTPPassword = json["HTTPPassword"].as<const char *>();

	return true;
}

/** @brief Default configuration.
 *  @param fileSystem FS, File system of the device.
 *  @return Void
 */
void set_default_device_configuration(FS* fileSystem) {
#ifndef RELEASE
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");
#endif // !RELEASE

	DeviceConfiguration.STASSID = DEFAULT_STA_SSID;
	DeviceConfiguration.STAPassword = DEFAULT_STA_PASSWORD;
	DeviceConfiguration.APPassword = DEFAULT_AP_PASSWORD;
	DeviceConfiguration.CayenneUsername = DEFAULT_CAYENNE_USERNAME;
	DeviceConfiguration.CayennePassword = DEFAULT_CAYENNE_PASSWORD;
	DeviceConfiguration.CayenneClientID = DEFAULT_CAYENNE_CLIENT_ID;
	save_device_configuration(fileSystem);
}

/** @brief Save configuration.
 *  @param fileSystem FS, File system of the device.
 *  @return boolean, Successful saving.
 */
bool save_device_configuration(FS* fileSystem) {
#ifndef RELEASE
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");
#endif // !RELEASE

	//flag_config = false;
	DynamicJsonBuffer jsonBuffer(512);

	//StaticJsonBuffer<1024> jsonBuffer;
	JsonObject& json = jsonBuffer.createObject();
	json["STASSID"] = DeviceConfiguration.STASSID;
	json["STAPassword"] = DeviceConfiguration.STAPassword;
	json["APPassword"] = DeviceConfiguration.APPassword;
	json["CayenneUsername"] = DeviceConfiguration.CayenneUsername;
	json["CayennePassword"] = DeviceConfiguration.CayennePassword;
	json["CayenneClientID"] = DeviceConfiguration.CayenneClientID;
	json["HTTPUsername"] = DeviceConfiguration.HTTPUsername;
	json["HTTPPassword"] = DeviceConfiguration.HTTPPassword;
	
	File configFile = fileSystem->open(CONFIG_FILE, "w");
	if (!configFile) {
		DEBUGLOG("Failed to open config file for writing\r\n");
		configFile.close();
		return false;
	}

#ifndef RELEASE
	String temp;
	json.prettyPrintTo(temp);
	DEBUGLOG("%s\r\n", temp.c_str());
#endif

	json.printTo(configFile);
	configFile.flush();
	configFile.close();

	return true;
}
