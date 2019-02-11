// LocalWebServer.h

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

#ifndef _LOCALWEBSERVER_h
#define _LOCALWEBSERVER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#pragma region Headers

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <StreamString.h>

#ifdef ENABLE_OTA_ARDUINO

#include <ArduinoOTA.h>

#endif // ENABLE_OTA_ARDUINO

#include "ApplicationConfiguration.h"

#include "DebugPort.h"

#include "DeviceConfiguration.h"

#include "GeneralHelper.h"

#pragma endregion


class LocalWebServerClass : public AsyncWebServer
{
 protected:

#pragma region Variables

	 /* @brief File system object. */
	 FS* _fileSystem;

	 /* @brief Browser MD5 sum. */
	 String _browserMD5 = "";

	 /* @brief Size of the firmware. */
	 uint32_t _updateSize = 0;

#pragma endregion

#pragma region Methods

#ifdef ENABLE_WEB_EDITOR

	 /** @brief Handle file list.
	  *  @param request AsyncWebServerRequest, Request object.
	  *  @return Void.
	  */
	 void handleFileList(AsyncWebServerRequest *request);

	 /** @brief Read file.
	  *  @param path String, File path.
	  *  @param request AsyncWebServerRequest, Request object.
	  *  @return Void.
	  */
	 bool handleFileRead(String path, AsyncWebServerRequest *request);

	 /** @brief Create file.
	  *  @param request AsyncWebServerRequest, Request object.
	  *  @return Void.
	  */
	 void handleFileCreate(AsyncWebServerRequest *request);

	 /** @brief Delete file.
	  *  @param request AsyncWebServerRequest, Request object.
	  *  @return Void.
	  */
	 void handleFileDelete(AsyncWebServerRequest *request);

	 /** @brief Upload file.
	  *  @param request AsyncWebServerRequest, Request object.
	  *  @param filename String, Name of the file.
	  *  @param index size_t,
	  *  @param data uint8, Content of the file.
	  *  @param len size_t, Length of the file.
	  *  @param final boolean, Flag for closing multi part file operation.
	  *  @return Void.
	  */
	 void handleFileUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8 *data, size_t len, bool final);

#endif // ENABLE_WEB_EDITOR

#ifdef ENABLE_OTA_HTTP

	 /** @brief Set MD5 sum of the new firmware. Part of the API.
	 *  @param request, AsyncWebServer Request request object.
	 *  @return Void.
	 */
	 void setUpdateMD5(AsyncWebServerRequest *request);

	 /** @brief Save authentication data.
	 *  @param request, AsyncWebServerRequest request object.
	 *  @param filename String Name of the file.
	 *  @param index size_t Part index.
	 *  @param data uint8 Data content.
	 *  @param len size_t Size of the content.
	 *  @param final boolean Last part to close file.
	 *  @return boolean, Return true if successful.
	 */
	 void updateFirmware(AsyncWebServerRequest *request, String filename, size_t index, uint8 *data, size_t len, bool final);

	 /** @brief Send firmware update values. Part of the API.
	 *  @param request, AsyncWebServerRequest request object.
	 *  @return Void.
	 */
	 void sendUpdateFWValues(AsyncWebServerRequest *request);

#endif // ENABLE_OTA_HTTP

#ifdef ENABLE_OTA_ARDUINO

	 /** @brief Configure OTA.
	  *  @param password String, Password for flashing.
	  *  @return Void.
	  */
	 void configureOTA(String password);

#endif // ENABLE_OTA_ARDUINO

#pragma region API Handlers

	 /** @brief Send configuration values. Part of the API.
	  *  @param request, AsyncWebServerRequest request object.
	  *  @return Void.
	  */
	 void saveConfiguration(AsyncWebServerRequest *request);

	 /** @brief Send device configuration. Part of the API.
	  *  @param request, AsyncWebServerRequest request object.
	  *  @return Void.
	  */
	 void sendConfiguration(AsyncWebServerRequest *request);

	 /** @brief Send connection state. Part of the API.
	  *  @param request, AsyncWebServerRequest request object.
	  *  @return Void.
	  */
	 void sendConnectionState(AsyncWebServerRequest *request);

	 /** @brief Send list of networks. Part of the API.
      *  @param request, AsyncWebServerRequest request object.
      *  @return Void.
      */
	 void sendNetworks(AsyncWebServerRequest *request);

#pragma endregion


	 /** @brief Check the authorization status.
	  *  @param request, AsyncWebServer Request request object.
	  *  @return boolean, True have to authenticate.
	  */
	 bool checkAuth(AsyncWebServerRequest *request);

	 /** @brief Initialize the server.
	  *  @return Void.
	  */
	 void serverInit();

	 /** @brief Decode URL unification. Based on https://code.google.com/p/avr-netino/
      *  @param input String, String to decode.
      *  @return String, Returns the string of unified URL string.
      */
	 static String urlDecode(String input);

	 /** @brief Convert file extension to content type.
	  *  @param filename, String Name of the file.
	  *  @param request, AsyncWebServerRequest Request object.
	  *  @return String, Content type.
	  */
	 static String getContentType(String filename, AsyncWebServerRequest *request);

#pragma endregion

 public:

	 /** @brief Constructor.
	  *  @param port, uint16 WEB server port.
	  *  @return LocalWebServerClass
	  */
	 LocalWebServerClass(uint16 port);

	 /** @brief Begin server.
	  *  @param fs, FS file system.
	  *  @return Void
	  */
	 void configure(FS* fs);

	 /** @brief Handle OTA process.
	  *  @return Void.
	  */
	 void handle();

};

extern LocalWebServerClass LocalWebServer;

#endif

