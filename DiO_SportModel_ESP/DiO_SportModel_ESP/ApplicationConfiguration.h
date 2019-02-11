// ApplicationConfiguration.h

#ifndef _APPLICATIONCONFIGURATION_h
#define _APPLICATIONCONFIGURATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#pragma region General Configuration

/** @brief Brand name. */
#define DEVICE_BRAND "Sport Model"

// NOTE: Be careful with the Version number in the server and here in the source code.
// TODO: Version Controll - Coment this version notation with the team.
// 1000
// 10 - Major
// 00 - Minor
/** @brief Firmware version string. */
#define ESP_FW_VERSION "0001"

/**
 * @brief Release flag.
 * Comment to enable debug output
 */
 //#define RELEASE

#pragma endregion

#pragma region GPIO Map

#define PIN_BUTTON D1

#pragma endregion

#pragma region Debug Port

#define DEBUG_PORT Serial

#define DEBUG_PORT_BAUDRATE 115200

#pragma endregion

#pragma region AP Configuration

/** @brief Default AP password. */
#define DEFAULT_AP_PASSWORD "12345678"

#pragma endregion

#pragma region STA Configuration

/** @brief Default STA SSID. */
#define DEFAULT_STA_SSID "YOUR_WIFI_PASSWORD"

/** @brief Default STA password. */
#define DEFAULT_STA_PASSWORD "YOUR_WIFI_SSID"

#pragma endregion

#pragma region HTTP Authentication

/** @brief Default HTTP username. */
#define DEFAULT_HTTP_USERNAME "admin"

/** @brief Default HTTP password. */
#define DEFAULT_HTTP_PASSWORD "admin"

#pragma endregion


#pragma region Cayenne

/** @brief Default Cayenne Username. */
#define DEFAULT_CAYENNE_USERNAME "YOUR_CAYENNE_USERNAME"

/** @brief Default Cayenne password. */
#define DEFAULT_CAYENNE_PASSWORD "YOUR_CAYENNE_PASSWORD"

/** @brief Default Cayenne password. */
#define DEFAULT_CAYENNE_CLIENT_ID "YOUR_CAYENNE_CLIENT_ID"

#pragma endregion

#pragma region Config File Path

/* @brief JSON configuration file. */
#define CONFIG_FILE "/config.json"

#pragma endregion

#endif

