/*
 Name:		DiO_SportModel_ESP.ino
 Created:	2/9/2019 12:48:01 PM
 Author:	Orlin
*/

#pragma region Headres

#include <ESP8266WiFi.h>
#include <FS.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

#include "ApplicationConfiguration.h"

#include "DebugPort.h"

#include "GeneralHelper.h"
#include "DeviceConfiguration.h"
#include "Indications.h"
#include "Indications.h"
#include "ApplicationMode.h"
#include "ButtonGesture.h"

//#define CAYENNE_DEBUG
//#define CAYENNE_PRINT DEBUG_PORT
#include <CayenneMQTTESP8266.h>

// the setup function runs once when you press reset or power the board
#include "LocalWebServer.h"

#pragma endregion

#pragma region Variables

ApplicationMode AppMode_g = ApplicationMode::Idle;

/* @brief WiFi mode. */
WiFiMode_t WiFiMode_g = WiFiMode_t::WIFI_OFF;

/* @brief WiFi disconnected since. */
long WiFiDisconnectedSince_g = 0;

/* @brief On station mode connected handler. */
WiFiEventHandler OnStationModeConnectedHandler_g;

/* @brief On station mode disconnected handler. */
WiFiEventHandler OnStationModeDisconnectedHandler_g;

/* @brief On station mode got IP handler. */
WiFiEventHandler OnStationModeGotIPHandler_g;

/* @brief On AP mode station connected. */
WiFiEventHandler OnAPModeStationConnectedHandle_g;

/* @brief On AP mode station disconnected. */
WiFiEventHandler OnAPModeStationDisconnectedHandle_g;

#pragma endregion


#pragma region Prototypes

void device_properties();
void configure_file_system();
void configure_to_sta();

#pragma endregion


void setup()
{
	configure_debug_port();

	device_properties();

	// Turn off the WiFi module.
	WiFi.mode(WiFiMode_g);

	// Start the file system.
	configure_file_system();

	// Try to load configuration from file system.
	if (!load_device_configuration(&SPIFFS)) {
		// Load defaults if any error.
		set_default_device_configuration(&SPIFFS);
	}

	ButtonGesture.configure();
	
	int GestureL = Gestures::None;
	for (;;)
	{
		GestureL = ButtonGesture.check();
		if (GestureL != Gestures::None)
		{
			break;
		}
		ESP.wdtFeed();
	}

	if (GestureL == Gestures::Hold)
	{
		AppMode_g = ApplicationMode::NormalOperation;
	}
	else if(GestureL == Gestures::LongHold)
	{
		AppMode_g = ApplicationMode::Configuriation;
	}

	if (AppMode_g == ApplicationMode::NormalOperation)
	{
		configure_to_sta();

		// TODO: play normal operation song.
		Cayenne.begin(DeviceConfiguration.CayenneUsername.c_str(),
			DeviceConfiguration.CayennePassword.c_str(),
			DeviceConfiguration.CayenneClientID.c_str());
	}
	else if (AppMode_g == ApplicationMode::Configuriation)
	{
		// TODO: Play configuration operation song.
		// TODO: Start the WEB server.
	}
}

// the loop function runs over and over again until power down or reset
void loop()
{
	int GestureL = ButtonGesture.check();

	if (AppMode_g == ApplicationMode::NormalOperation)
	{
		Cayenne.loop();
	}
	else if (AppMode_g == ApplicationMode::Configuriation)
	{
		// TODO: Run the WEB server.
	}

	if (GestureL == Gestures::LongHold)
	{
		ESP.deepSleep(40000);
	}
}

#pragma region Functions

/** @brief Printout in the debug console flash state.
 *  @return Void.
 */
void device_properties() {
#ifndef RELEASE
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");
#endif // RELEASE

	DEBUGLOG("Flash chip size: %u\r\n", ESP.getFlashChipRealSize());
	DEBUGLOG("Sketch size: %u\r\n", ESP.getSketchSize());
	DEBUGLOG("Free flash space: %u\r\n", ESP.getFreeSketchSpace());
	DEBUGLOG("Free heap: %d\r\n", ESP.getFreeHeap());
	DEBUGLOG("Firmware version: %d\r\n", ESP_FW_VERSION);
	DEBUGLOG("SDK version: %s\r\n", ESP.getSdkVersion());
	uint8 c_MACL[WL_MAC_ADDR_LENGTH];
	WiFi.macAddress(c_MACL);
	String MACL = mac2str(c_MACL);
	DEBUGLOG("MAC address: %s\r\n", MACL.c_str());
}

#pragma region File System

/** @brief Configure the file system.
 *  @param fileSystem FS, File system object.
 *  @return Void.
 */
void configure_file_system()
{
	if (!SPIFFS.begin())
	{
		DEBUGLOG("Can not load file system.\r\n");
		for (;;) {}
	}

#ifndef RELEASE
	// List files
	Dir dir = SPIFFS.openDir("/");
	while (dir.next()) {
		String fileName = dir.fileName();
		size_t fileSize = dir.fileSize();
		DEBUGLOG("FS File: %s, size: %s\r\n", fileName.c_str(), formatBytes(fileSize).c_str());
	}
	DEBUGLOG("\r\n");
#endif // RELEASE
}

#pragma endregion

#pragma region STA mode

/** @brief Configure WiFi module to station.
 *  @return Void.
 */
void configure_to_sta() {
#ifndef RELEASE
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");
#endif // RELEASE

	// Register WiFi Event to control connection LED.
	OnStationModeConnectedHandler_g = WiFi.onStationModeConnected(
		[](WiFiEventStationModeConnected evt) {
		handler_sta_mode_connected(evt);
	});

	OnStationModeDisconnectedHandler_g = WiFi.onStationModeDisconnected(
		[](WiFiEventStationModeDisconnected evt) {
		handler_sta_mode_disconnected(evt);
	});

	OnStationModeGotIPHandler_g = WiFi.onStationModeGotIP(
		[](WiFiEventStationModeGotIP evt) {
		handler_sta_mode_got_ip(evt);
	});

	// Set the host name.
	WiFi.hostname(DeviceConfiguration.deviceName.c_str());

	//disconnect required here
	//improves reconnect reliability
	WiFi.disconnect();

	// Encourage clean recovery after disconnect species5618, 08-March-2018
	WiFi.setAutoReconnect(true);
	WiFi.mode(WIFI_STA);

	DEBUGLOG("WiFi Connecting: %s\r\n", DeviceConfiguration.ssid.c_str());

	WiFi.begin(DeviceConfiguration.STASSID.c_str(), DeviceConfiguration.STAPassword.c_str());

	// Check the DHCP.
	if (!DeviceConfiguration.dhcp) {
		DEBUGLOG("NO DHCP\r\n");
		WiFi.config(DeviceConfiguration.ip, DeviceConfiguration.gateway, DeviceConfiguration.netmask, DeviceConfiguration.dns);
	}

	WiFi.waitForConnectResult();

#ifndef TEST_EFM32_UPDATE

	uint8 PayloadResponseL[2] = { WiFiMode_t::WIFI_STA, (uint8)WiFi.isConnected() };
	Communicator.sendRawResponse(OpCodes::WiFiMode, StatusCodes::Ok, PayloadResponseL, 2);

#endif // !TEST_EFM32_UPDATE

}

/** @brief Handler that execute when the device is connected.
 *  @param evt WiFiEventStationModeConnected, Data object.
 *  @return Void.
 */
void handler_sta_mode_connected(WiFiEventStationModeConnected evt) {
#ifndef RELEASE
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");
#endif // RELEASE

	DEBUGLOG("WiFi Connected: %s\r\n", DeviceConfiguration.ssid.c_str());
	DEBUGLOG("Waiting for DHCP\r\n");

	WiFiDisconnectedSince_g = 0;
}

/** @brief Handler that execute when the device got IP.
 *  @param evt WiFiEventStationModeGotIP, Callback handler.
 *  @return Void.
 */
void handler_sta_mode_got_ip(WiFiEventStationModeGotIP evt) {
#ifndef RELEASE
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");
#endif // RELEASE

	DEBUGLOG("GotIP Address: %s\r\n", WiFi.localIP().toString().c_str());
	DEBUGLOG("Gateway:       %s\r\n", WiFi.gatewayIP().toString().c_str());
	DEBUGLOG("DNS:           %s\r\n", WiFi.dnsIP().toString().c_str());

	WiFiDisconnectedSince_g = 0;
}

/** @brief Handler that execute when the device is disconnected.
 *  @param evt WiFiEventStationModeDisconnected, Callback handler.
 *  @return Void.
 */
void handler_sta_mode_disconnected(WiFiEventStationModeDisconnected evt) {
#ifndef RELEASE
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");
#endif // RELEASE

	if (WiFiDisconnectedSince_g == 0) {
		WiFiDisconnectedSince_g = millis();
	}

	DEBUGLOG("Disconnected for %d seconds.\r\n", (int)((millis() - WiFiDisconnectedSince_g) / 1000));
}

#pragma endregion

#pragma region Cayenne

// Default function for sending sensor data at intervals to Cayenne.
// You can also use functions for specific channels, e.g CAYENNE_OUT(1) for sending channel 1 data.
CAYENNE_OUT_DEFAULT()
{
	// Write data to Cayenne here. This example just sends the current uptime in milliseconds on virtual channel 0.
	Cayenne.virtualWrite(0, millis());
	// Some examples of other functions you can use to send data.
	//Cayenne.celsiusWrite(1, 22.0);
	//Cayenne.luxWrite(2, 700);
	//Cayenne.virtualWrite(3, 50, TYPE_PROXIMITY, UNIT_CENTIMETER);
}

// Default function for processing actuator commands from the Cayenne Dashboard.
// You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
	CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
	//Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}

#pragma endregion

#pragma endregion
