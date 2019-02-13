// ApplicationConfiguration.h

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

/** @brief Firmware version string. */
#define ESP_FW_VERSION 1

#define EANBLE_DEBUG_OUT

#define ENABLE_WEB_EDITOR

//#define ENABLE_OTA_ARDUINO

#define ENABLE_OTA_HTTP

#define ENABLE_CAYENNE_MODE

#pragma endregion

#pragma region GPIO Map

#define PIN_BUTTON D5

#define PIN_LED D2

#define PIN_BUZZER D1

#pragma endregion

#pragma region Debug Port

#ifdef EANBLE_DEBUG_OUT

#define DEBUG_PORT Serial

#define DEBUG_PORT_BAUDRATE 115200

#endif // EANBLE_DEBUG_OUT

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

#pragma region HTTP WEB / Authentication

#define PORT_HTTP 80

/** @brief Default HTTP username. */
#define DEFAULT_HTTP_USERNAME "admin"

/** @brief Default HTTP password. */
#define DEFAULT_HTTP_PASSWORD "admin"

#pragma endregion

#pragma region Config File Path

/* @brief JSON configuration file. */
#define CONFIG_FILE "/config.json"

#pragma endregion

#pragma region Cayenne

#ifdef ENABLE_CAYENNE_MODE

/** @brief Default Cayenne Username. */
#define DEFAULT_CAYENNE_USERNAME "YOUR_CAYENNE_USERNAME"

/** @brief Default Cayenne password. */
#define DEFAULT_CAYENNE_PASSWORD "YOUR_CAYENNE_PASSWORD"

/** @brief Default Cayenne password. */
#define DEFAULT_CAYENNE_CLIENT_ID "YOUR_CAYENNE_CLIENT_ID"

#endif // ENABLE_CAYENNE_MODE

#pragma endregion

#pragma region Deep sleep settings

#define NO_ACTION_TIME_OUT 5000

#define DISCONNECTED_SECONDS 60

#pragma endregion


#endif

