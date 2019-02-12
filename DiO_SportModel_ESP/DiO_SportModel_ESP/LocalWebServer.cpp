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

#pragma region Headres

#include "LocalWebServer.h"

#pragma endregion

#pragma region Variables

/* @brief Singelton Local WEB server instance. */
LocalWebServerClass LocalWebServer(PORT_HTTP);

#pragma endregion

/** @brief Constructor.
 *  @param port, uint16 WEB server port.
 *  @return LocalWebServerClass
 */
LocalWebServerClass::LocalWebServerClass(uint16 port) : AsyncWebServer(port) {}

/** @brief Begin server.
 *  @param fs, FS file system.
 *  @return Void
 */
void LocalWebServerClass::configure(FS* fs)
{
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");

	_fileSystem = fs;

	// Configure and start Web server
	AsyncWebServer::begin();

	serverInit();

	// Configure DNS.
	MDNS.begin(DeviceConfiguration.DeviceName.c_str());
	MDNS.addService("http", "tcp", PORT_HTTP);

#ifdef ENABLE_OTA_ARDUINO

	configureOTA(DeviceConfiguration.HTTPPassword.c_str());

#endif // ENABLE_OTA_ARDUINO
}

/** @brief Handle OTA process.
 *  @return Void.
 */
void LocalWebServerClass::handle() {

#ifdef ENABLE_OTA_ARDUINO

	ArduinoOTA.handle();

#endif // ENABLE_OTA_ARDUINO

}

/** @brief Decode URL unification. Based on https://code.google.com/p/avr-netino/
 *  @param input String, String to decode.
 *  @return String, Returns the string of unified URL string.
 */
String LocalWebServerClass::urlDecode(String input)
{
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");

	char SymbolL;
	String OutputL = "";

	for (byte index = 0; index < input.length(); index++)
	{
		SymbolL = input[index];

		if (SymbolL == '+')
		{
			SymbolL = ' ';
		}

		if (SymbolL == '%')
		{
			index++;
			SymbolL = input[index];
			index++;
			SymbolL = (hex2dec(SymbolL) << 4) | hex2dec(input[index]);
		}

		OutputL.concat(SymbolL);
	}

	return OutputL;
}

/** @brief Convert file extension to content type.
 *  @param filename, String Name of the file.
 *  @param request, AsyncWebServerRequest Request object.
 *  @return String, Content type.
 */
String LocalWebServerClass::getContentType(String filename, AsyncWebServerRequest *request)
{
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");

	if (request->hasArg("download")) return "application/octet-stream";
	else if (filename.endsWith(".htm")) return "text/html";
	else if (filename.endsWith(".html")) return "text/html";
	else if (filename.endsWith(".css")) return "text/css";
	else if (filename.endsWith(".js")) return "application/javascript";
	else if (filename.endsWith(".json")) return "application/json";
	else if (filename.endsWith(".png")) return "image/png";
	else if (filename.endsWith(".gif")) return "image/gif";
	else if (filename.endsWith(".jpg")) return "image/jpeg";
	else if (filename.endsWith(".ico")) return "image/x-icon";
	else if (filename.endsWith(".xml")) return "text/xml";
	else if (filename.endsWith(".pdf")) return "application/x-pdf";
	else if (filename.endsWith(".zip")) return "application/x-zip";
	else if (filename.endsWith(".gz")) return "application/x-gzip";
	return "text/plain";
}

/** @brief Initialize the server.
 *  @return Void.
 */
void LocalWebServerClass::serverInit() {
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");

#pragma region File editor API

#ifdef ENABLE_WEB_EDITOR

	// List directory.
	on("/list", HTTP_GET, [this](AsyncWebServerRequest *request) {
		if (!this->checkAuth(request))
			return request->requestAuthentication();
		this->handleFileList(request);
	});

	// Load editor.
	on("/edit", HTTP_GET, [this](AsyncWebServerRequest *request) {
		if (!this->checkAuth(request))
			return request->requestAuthentication();
		if (!this->handleFileRead("/edit.html", request))
			request->send(404, "text/plain", "FileNotFound");
	});

	// Create file.
	on("/edit", HTTP_PUT, [this](AsyncWebServerRequest *request) {
		if (!this->checkAuth(request))
			return request->requestAuthentication();
		this->handleFileCreate(request);
	});

	// Delete file.
	on("/edit", HTTP_DELETE, [this](AsyncWebServerRequest *request) {
		if (!this->checkAuth(request))
			return request->requestAuthentication();
		this->handleFileDelete(request);
	});

	// First callback is called after the request has ended with all parsed arguments.
	// Second callback handles file uploads at that location.
	on("/edit", HTTP_POST,
		[](AsyncWebServerRequest *request) { request->send(200, "text/plain", ""); },
		[this](AsyncWebServerRequest *request, String filename, size_t index, uint8 *data, size_t len, bool final) {
		this->handleFileUpload(request, filename, index, data, len, final); });

#endif // ENABLE_WEB_EDITOR

#pragma endregion

#pragma region Firmware update API

#ifdef ENABLE_OTA_HTTP

	// Update the firmware.
	on("/update", HTTP_GET, [this](AsyncWebServerRequest *request) {
		if (!this->checkAuth(request))
			return request->requestAuthentication();
		if (!this->handleFileRead("/update.html", request))
			request->send(404, "text/plain", "FileNotFound");
	});

	on("/update", HTTP_POST, [this](AsyncWebServerRequest *request) {
		if (!this->checkAuth(request))
			return request->requestAuthentication();
		AsyncWebServerResponse *response = request->beginResponse(200, "text/html", (Update.hasError()) ? "FAIL" : "<META http-equiv=\"refresh\" content=\"15;URL=/update\">Update correct. Restarting...");
		response->addHeader("Connection", "close");
		response->addHeader("Access-Control-Allow-Origin", "*");
		request->send(response);
		this->_fileSystem->end();
		ESP.restart();
	}, [this](AsyncWebServerRequest *request, String filename, size_t index, uint8 *data, size_t len, bool final) {
		this->updateFirmware(request, filename, index, data, len, final);
	});

	// Update possible.
	on("/api/updatepossible", [this](AsyncWebServerRequest *request) {
		DEBUGLOG("%s\r\n", request->url().c_str());
		if (!this->checkAuth(request))
			return request->requestAuthentication();
		this->sendUpdateFWValues(request);
	});

	// Sets the MD5 sum of the firmware.
	on("/api/setmd5", [this](AsyncWebServerRequest *request) {
		DEBUGLOG("%s\r\n", request->url().c_str());
		if (!this->checkAuth(request))
			return request->requestAuthentication();
		this->setUpdateMD5(request);
	});

#endif // ENABLE_OTA_HTTP

#ifdef ENABLE_OTA_ARDUINO

#endif // ENABLE_OTA_ARDUINO

#pragma endregion

#pragma region Login Form

	// login.html
	on("/login", HTTP_GET, [this](AsyncWebServerRequest *request) {
		DEBUGLOG("%s\r\n", request->url().c_str());
		if (this->checkAuth(request))
		{
			request->redirect("/settings");
			return;
		}

		if (!this->handleFileRead("/login.html", request))
		{
			request->send(404, "text/plain", "FileNotFound");
			return;
		}
	});

	// login.html
	on("/login", HTTP_POST, [this](AsyncWebServerRequest *request) {
		DEBUGLOG("%s\r\n", request->url().c_str());
		if (request->args() == 2)
		{
			// TODO: Check Username and Password.
			if (request->hasArg("HTTPUsername") && 
				request->hasArg("HTTPPassword"))
			{
				request->redirect("/settings");
				return;
			}
			else
			{
				if (!this->handleFileRead("/login.html", request))
				{
					request->send(404, "text/plain", "FileNotFound");
					return;
				}
			}
		}
		else
		{
			if (!this->handleFileRead("/login.html", request))
			{
				request->send(404, "text/plain", "FileNotFound");
				return;
			}
		}

		if (!this->handleFileRead("/login.html", request))
		{
			request->send(404, "text/plain", "FileNotFound");
			return;
		}
	});

#pragma endregion

#pragma region Configuration page

	// configuration.html
	on("/configuration", HTTP_GET, [this](AsyncWebServerRequest *request) {
		DEBUGLOG("%s\r\n", request->url().c_str());
		if (!this->checkAuth(request))
		{
			request->requestAuthentication();
			return;
		}

		if (!this->handleFileRead("/configuration.html", request))
		{
			request->send(404, "text/plain", "FileNotFound");
			return;
		}
	});

	// configuration.html
	on("/configuration", HTTP_POST, [this](AsyncWebServerRequest *request) {
		DEBUGLOG("%s\r\n", request->url().c_str());
		if (!this->checkAuth(request))
		{
			request->requestAuthentication();
			return;
		}

		this->saveConfiguration(request);
	});
	
#pragma endregion

#pragma region API

	// Configuration parameters
	on("/api/v1/configuration", HTTP_GET, [this](AsyncWebServerRequest *request) {
		DEBUGLOG("%s\r\n", request->url().c_str());

		if (!this->checkAuth(request))
		{
			request->requestAuthentication();
			return;
		}

		this->sendConfiguration(request);
	});
	
	// Connection state
	on("/api/v1/connectionState", [this](AsyncWebServerRequest *request) {
		DEBUGLOG("%s\r\n", request->url().c_str());

		if (!this->checkAuth(request))
		{
			request->requestAuthentication();
			return;
		}

		this->sendConnectionState(request);
	});

	// Scans WiFi networks.
	on("/api/v1/scanNetworks", HTTP_GET, [this](AsyncWebServerRequest *request)
	{
		DEBUGLOG("%s\r\n", request->url().c_str());

		if (!this->checkAuth(request))
		{
			request->requestAuthentication();
			return;
		}

		this->sendNetworks(request);
	});

#pragma endregion

#pragma region Page not found API

	// Called when the URL is not defined here.
	// Use it to load content from SPIFFS.
	onNotFound([this](AsyncWebServerRequest *request)
	{
		DEBUGLOG("Not found: %s\r\n", request->url().c_str());
		
		if (!this->checkAuth(request))
		{
			request->requestAuthentication();
			return;
		}

		AsyncWebServerResponse *response = request->beginResponse(200);
		response->addHeader("Connection", "close");
		response->addHeader("Access-Control-Allow-Origin", "*");
		
		if (!this->handleFileRead(request->url(), request))
		{
			request->send(404, "text/plain", "FileNotFound");
		}
		
		delete response; // Free up memory!
	});

#pragma endregion

#pragma region Configuration files API

#ifdef HIDE_SECRET

	on(SECRET_FILE, HTTP_GET, [this](AsyncWebServerRequest *request) {
		if (!this->checkAuth(request))
			return request->requestAuthentication();
		AsyncWebServerResponse *response = request->beginResponse(403, "text/plain", "Forbidden");
		response->addHeader("Connection", "close");
		response->addHeader("Access-Control-Allow-Origin", "*");
		request->send(response);
	});

#endif // HIDE_SECRET

#ifdef HIDE_CONFIG

	on(CONFIG_FILE, HTTP_GET, [this](AsyncWebServerRequest *request) {
		if (!this->checkAuth(request))
			return request->requestAuthentication();
		AsyncWebServerResponse *response = request->beginResponse(403, "text/plain", "Forbidden");
		response->addHeader("Connection", "close");
		response->addHeader("Access-Control-Allow-Origin", "*");
		request->send(response);
	});

	on(USER_CONFIG_FILE, HTTP_GET, [this](AsyncWebServerRequest *request) {
		if (!this->checkAuth(request))
			return request->requestAuthentication();
		AsyncWebServerResponse *response = request->beginResponse(403, "text/plain", "Forbidden");
		response->addHeader("Connection", "close");
		response->addHeader("Access-Control-Allow-Origin", "*");
		request->send(response);
	});

#endif // HIDE_CONFIG

#pragma endregion

}

/** @brief Check the authorization status.
 *  @param request, AsyncWebServer Request request object.
 *  @return boolean, True have to authenticate.
 */
bool LocalWebServerClass::checkAuth(AsyncWebServerRequest *request)
{
	if (!DeviceConfiguration.HTTPAuthentication) {
		return true;
	}
	else {
		return request->authenticate(DeviceConfiguration.HTTPUsername.c_str(),
			DeviceConfiguration.HTTPUsername.c_str());
	}
}

#pragma region IO Oprations

/** @brief Handle file list.
 *  @param request AsyncWebServerRequest, Request object.
 *  @return Void.
 */
void LocalWebServerClass::handleFileList(AsyncWebServerRequest *request) {
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");
	
	if (!request->hasArg("dir"))
	{
		request->send(500, "text/plain", "BAD ARGS");
		return;
	}

	String path = request->arg("dir");
	DEBUGLOG("handleFileList: %s\r\n", path.c_str());
	Dir dir = _fileSystem->openDir(path);
	path = String();

	String output = "[";
	while (dir.next()) {
		File entry = dir.openFile("r");
		if (true)//entry.name()!="secret.json") // Do not show secrets
		{
			if (output != "[")
				output += ',';
			bool isDir = false;
			output += "{\"type\":\"";
			output += (isDir) ? "dir" : "file";
			output += "\",\"name\":\"";
			output += String(entry.name()).substring(1);
			output += "\"}";
		}
		entry.close();
	}

	output += "]";
	DEBUGLOG("%s\r\n", output.c_str());
	request->send(200, "text/json", output);
}

/** @brief Read file.
 *  @param path String, File path.
 *  @param request AsyncWebServerRequest, Request object.
 *  @return Void.
 */
bool LocalWebServerClass::handleFileRead(String path, AsyncWebServerRequest *request) {
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");

	DEBUGLOG("handleFileRead: %s\r\n", path.c_str());

	if (path.endsWith("/"))
	{
		path += "index.htm";
	}

	String contentType = getContentType(path, request);
	String pathWithGz = path + ".gz";
	if (_fileSystem->exists(pathWithGz) || _fileSystem->exists(path)) {
		if (_fileSystem->exists(pathWithGz)) {
			path += ".gz";
		}
		DEBUGLOG("Content type: %s\r\n", contentType.c_str());
		AsyncWebServerResponse *response = request->beginResponse(*_fileSystem, path, contentType);


		// Add header.
		response->addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
		response->addHeader("Pragma", "no-cache");
		response->addHeader("Expires", "0");

		if (path.endsWith(".gz"))
		{
			response->addHeader("Content-Encoding", "gzip");
		}
		//File file = SPIFFS.open(path, "r");
		DEBUGLOG("File %s exist\r\n", path.c_str());
		request->send(response);
		DEBUGLOG("File %s Sent\r\n", path.c_str());

		return true;
	}
	else
	{
		DEBUGLOG("Cannot find %s\r\n", path.c_str());
	}
	return false;
}

/** @brief Create file.
 *  @param request AsyncWebServerRequest, Request object.
 *  @return Void.
 */
void LocalWebServerClass::handleFileCreate(AsyncWebServerRequest *request) {
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");

	if (!checkAuth(request))
		return request->requestAuthentication();
	if (request->args() == 0)
		return request->send(500, "text/plain", "BAD ARGS");
	String path = request->arg(0U);
	DEBUGLOG("handleFileCreate: %s\r\n", path.c_str());
	if (path == "/")
		return request->send(500, "text/plain", "BAD PATH");
	if (_fileSystem->exists(path))
		return request->send(500, "text/plain", "FILE EXISTS");
	File file = _fileSystem->open(path, "w");
	if (file)
		file.close();
	else
		return request->send(500, "text/plain", "CREATE FAILED");
	request->send(200, "text/plain", "");
	path = String(); // Remove? Useless statement?
}

/** @brief Delete file.
 *  @param request AsyncWebServerRequest, Request object.
 *  @return Void.
 */
void LocalWebServerClass::handleFileDelete(AsyncWebServerRequest *request) {
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");

	if (!checkAuth(request))
		return request->requestAuthentication();
	if (request->args() == 0) return request->send(500, "text/plain", "BAD ARGS");
	String path = request->arg(0U);
	DEBUGLOG("handleFileDelete: %s\r\n", path.c_str());
	if (path == "/")
		return request->send(500, "text/plain", "BAD PATH");
	if (!_fileSystem->exists(path))
		return request->send(404, "text/plain", "FileNotFound");
	_fileSystem->remove(path);
	request->send(200, "text/plain", "");
	path = String(); // Remove? Useless statement?
}

/** @brief Upload file.
 *  @param request AsyncWebServerRequest, Request object.
 *  @param filename String, Name of the file.
 *  @param index size_t,
 *  @param data uint8, Content of the file.
 *  @param len size_t, Length of the file.
 *  @param final boolean, Flag for closing multipart file operation.
 *  @return Void.
 */
void LocalWebServerClass::handleFileUpload(
	AsyncWebServerRequest *request,
	String filename,
	size_t index,
	uint8 *data,
	size_t len,
	bool final) {
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");

	static File fsUploadFile;
	static size_t fileSize = 0;

	if (!index) { // Start
		DEBUGLOG("Handle file upload name: %s\r\n", filename.c_str());
		if (!filename.startsWith("/")) filename = "/" + filename;
		fsUploadFile = _fileSystem->open(filename, "w");
		DEBUGLOG("First upload part.\r\n");

	}
	// Continue
	if (fsUploadFile) {
		DEBUGLOG("Continue upload part. Size = %u\r\n", len);
		if (fsUploadFile.write(data, len) != len) {
			DEBUGLOG("Write error during upload.\r\n");
		}
		else
			fileSize += len;
	}
	/*for (size_t index = 0; index < len; index++) {
	if (fsUploadFile)
	fsUploadFile.write(data[index]);
	}*/
	if (final) { // End
		if (fsUploadFile) {
			fsUploadFile.close();
		}
		DEBUGLOG("Handle file upload size: %u\r\n", fileSize);
		fileSize = 0;
	}
}

#pragma endregion

#ifdef ENABLE_OTA_ARDUINO

/** @brief Configure OTA.
 *  @param password String, Password for flashing.
 *  @return Void.
 */
void LocalWebServerClass::configureOTA(String password) {
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");

	// Port defaults to 8266
	// ArduinoOTA.setPort(8266);

	// Host name defaults to esp8266-[ChipID]
	ArduinoOTA.setHostname(DeviceConfiguration.DeviceName.c_str());

	// No authentication by default
	if (password != "") {
		ArduinoOTA.setPassword(password.c_str());
		DEBUGLOG("OTA password set %s\r\n", password.c_str());
	}

	ArduinoOTA.onStart([]() {
		DEBUGLOG("StartOTA\r\n");
	});
	ArduinoOTA.onEnd(std::bind([](FS *fs) {
		fs->end();
		DEBUGLOG("End OTA\r\n");
	}, _fileSystem));
	ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
		DEBUGLOG("OTA Progress: %u%%\r\n", (progress / (total / 100)));
	});
	ArduinoOTA.onError([](ota_error_t error) {
		DEBUGLOG("Error[%u]: ", error);
		if (error == OTA_AUTH_ERROR) DEBUGLOG("Authentication Failed\r\n");
		else if (error == OTA_BEGIN_ERROR) DEBUGLOG("Begin Failed\r\n");
		else if (error == OTA_CONNECT_ERROR) DEBUGLOG("Connect Failed\r\n");
		else if (error == OTA_RECEIVE_ERROR) DEBUGLOG("Receive Failed\r\n");
		else if (error == OTA_END_ERROR) DEBUGLOG("End Failed\r\n");
	});
	DEBUGLOG("OTA Ready\r\n");

	ArduinoOTA.begin();
}

#endif // ENABLE_OTA_ARDUINO

#pragma region ENABLE_OTA_HTTP

#ifdef ENABLE_OTA_HTTP

/** @brief Set MD5 sum of the new firmware. Part of the API.
 *  @param request, AsyncWebServer Request request object.
 *  @return Void.
 */
void LocalWebServerClass::setUpdateMD5(AsyncWebServerRequest *request)
{
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");

	_browserMD5 = "";

	DEBUGLOG("Arg number: %d\r\n", request->args());
	if (request->args() > 0)  // Read hash
	{
		for (uint8 index = 0; index < request->args(); index++) {
			DEBUGLOG("Arg %s: %s\r\n", request->argName(index).c_str(), request->arg(index).c_str());
			if (request->argName(index) == "md5") {
				_browserMD5 = urlDecode(request->arg(index));
				Update.setMD5(_browserMD5.c_str());
				continue;
			}if (request->argName(index) == "size") {
				_updateSize = request->arg(index).toInt();
				DEBUGLOG("Update size: %l\r\n", _updateSize);
				continue;
			}
		}
		request->send(200, "text/html", "OK --> MD5: " + _browserMD5);
	}
}

/** @brief Save authentication data.
*  @param request, AsyncWebServerRequest request object.
*  @param filename String Name of the file.
*  @param index size_t Part index.
*  @param data uint8 Data content.
*  @param len size_t Size of the content.
*  @param final boolean Last part to close file.
*  @return boolean, Return true if successful.
*/
void LocalWebServerClass::updateFirmware(
	AsyncWebServerRequest *request,
	String filename,
	size_t index,
	uint8 *data,
	size_t len,
	bool final) 
{
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");

	// handler for the file upload, get's the sketch bytes, and writes
	// them through the Update object
	static long totalSize = 0;
	if (!index) { //UPLOAD_FILE_START
		SPIFFS.end();
		Update.runAsync(true);
		DEBUGLOG("Update start: %s\r\n", filename.c_str());
		uint32_t maxSketchSpace = ESP.getSketchSize();
		DEBUGLOG("Max free sketch space: %u\r\n", maxSketchSpace);
		DEBUGLOG("New sketch size: %u\r\n", _updateSize);
		if (_browserMD5 != NULL && _browserMD5 != "") {
			Update.setMD5(_browserMD5.c_str());
			DEBUGLOG("Hash from client: %s\r\n", _browserMD5.c_str());
		}
		if (!Update.begin(_updateSize)) {//start with max available size
			Update.printError(DEBUG_PORT);
		}

	}

	// Get upload file, continue if not start
	totalSize += len;
	DEBUGLOG(".");
	size_t written = Update.write(data, len);
	if (written != len) {
		DEBUGLOG("len = %d, written = %l, totalSize = %l\r\n", len, written, totalSize);
		Update.printError(DEBUG_PORT);
		//return;
	}
	if (final) {  // UPLOAD_FILE_END
		String updateHash;
		DEBUGLOG("Applying update...\r\n");
		if (Update.end(true)) { //true to set the size to the current progress
			updateHash = Update.md5String();
			DEBUGLOG("Upload finished. Calculated MD5: %s\r\n", updateHash.c_str());
			DEBUGLOG("Update Success: %u\nRebooting...\r\n", request->contentLength());
		}
		else {
			updateHash = Update.md5String();
			DEBUGLOG("Upload failed. Calculated MD5: %s\r\n", updateHash.c_str());
			Update.printError(DEBUG_PORT);
		}
	}
}

/** @brief Send firmware update values. Part of the API.
 *  @param request, AsyncWebServerRequest request object.
 *  @return Void.
 */
void LocalWebServerClass::sendUpdateFWValues(AsyncWebServerRequest *request)
{
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");

	String values = "";
	uint32_t maxSketchSpace = (ESP.getSketchSize() - 0x1000) & 0xFFFFF000;
	//bool updateOK = Update.begin(maxSketchSpace);
	bool updateOK = maxSketchSpace < ESP.getFreeSketchSpace();
	StreamString result;
	Update.printError(result);
	DEBUGLOG("--MaxSketchSpace: %d\n", maxSketchSpace);
	DEBUGLOG("--Update error = %s\n", result.c_str());
	values += "remupd|" + (String)((updateOK) ? "OK" : "ERROR") + "|div\n";

	if (Update.hasError()) {
		result.trim();
		values += "remupdResult|" + result + "|div\n";
	}
	else {
		values += "remupdResult||div\n";
	}

	request->send(200, "text/plain", values);
}

#endif // ENABLE_OTA_HTT

#pragma endregion

#pragma region API Handlers

/** @brief Send list of networks. Part of the API.
 *  @param request, AsyncWebServerRequest request object.
 *  @return Void.
 */
void LocalWebServerClass::sendNetworks(AsyncWebServerRequest *request)
{
	String json = "[";
	int n = WiFi.scanComplete();

	if (n == WIFI_SCAN_FAILED)
	{
		WiFi.scanNetworks(true);
	}
	else if (n)
	{
		for (int index = 0; index < n; ++index)
		{
			if (index) json += ",";
			json += "{";
			json += "\"rssi\":" + String(WiFi.RSSI(index));
			json += ",\"ssid\":\"" + WiFi.SSID(index) + "\"";
			json += ",\"bssid\":\"" + WiFi.BSSIDstr(index) + "\"";
			json += ",\"channel\":" + String(WiFi.channel(index));
			json += ",\"secure\":" + String(WiFi.encryptionType(index));
			json += ",\"hidden\":" + String(WiFi.isHidden(index) ? "true" : "false");
			json += "}";
		}
		WiFi.scanDelete();
		if (WiFi.scanComplete() == WIFI_SCAN_FAILED)
		{
			WiFi.scanNetworks(true);
		}
	}
	json += "]";
	request->send(200, "text/json", json);
}

/** @brief Send connection state. Part of the API.
 *  @param request, AsyncWebServerRequest request object.
 *  @return Void.
 */
void LocalWebServerClass::sendConnectionState(AsyncWebServerRequest *request) {
#ifndef RELEASE
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");
#endif // !RELEASE

	String state = "N/A";
	String Networks = "";
	if (WiFi.status() == 0) state = "Idle";
	else if (WiFi.status() == 1) state = "NO SSID AVAILBLE";
	else if (WiFi.status() == 2) state = "SCAN COMPLETED";
	else if (WiFi.status() == 3) state = "CONNECTED";
	else if (WiFi.status() == 4) state = "CONNECT FAILED";
	else if (WiFi.status() == 5) state = "CONNECTION LOST";
	else if (WiFi.status() == 6) state = "DISCONNECTED";

	WiFi.scanNetworks(true);

	String values = "";
	values += "connectionstate|" + state + "|div\n";
	//values += "networks|Scanning networks ...|div\n";
	request->send(200, "text/plain", values);
	state = "";
	values = "";
	Networks = "";
}

/** @brief Send device configuration. Part of the API.
 *  @param request, AsyncWebServerRequest request object.
 *  @return Void.
 */
void LocalWebServerClass::sendConfiguration(AsyncWebServerRequest *request)
{
#ifndef RELEASE
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");
#endif // !RELEASE

	String values = "";
	values += "FWVersion|" + String(ESP_FW_VERSION) + "|div\n";
	values += "DeviceName|" + (String)DeviceConfiguration.DeviceName + "|input\n";
	values += "STASSID|" + (String)DeviceConfiguration.STASSID + "|input\n";
	values += "HTTPUsername|" + (String)DeviceConfiguration.HTTPUsername + "|input\n";
	values += "HTTPAuthentication|" + (String)DeviceConfiguration.HTTPAuthentication + "|input\n";

#ifdef ENABLE_CAYENNE_MODE

	values += "CayenneUsername|" + (String)DeviceConfiguration.CayenneUsername + "|input\n";

#endif // ENABLE_CAYENNE_MODE

	request->send(200, "text/plain", values);
}

/** @brief Settings arguments parser. Part of the API.
 *  @param request, AsyncWebServerRequest request object.
 *  @return Void.
 */
void LocalWebServerClass::saveConfiguration(AsyncWebServerRequest *request)
{
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");

	if (!checkAuth(request))
		return request->requestAuthentication();

	if (request->args() > 0)  // Save Settings
	{
		for (uint8 index = 0; index < request->args(); index++)
		{
			DEBUGLOG("Arg %s: %s\r\n", request->argName(index).c_str(), urlDecode(request->arg(index)).c_str());

#pragma region Device Name

			if (request->argName(index) == "DeviceName") {
				DeviceConfiguration.DeviceName = urlDecode(request->arg(index));
				DEBUGLOG("STASSID: %s\r\n", DeviceConfiguration.DeviceName.c_str());

				continue;
			}

#pragma endregion

#pragma region STA

			if (request->argName(index) == "STASSID")
			{
				DeviceConfiguration.STASSID = urlDecode(request->arg(index));
				DEBUGLOG("STASSID: %s\r\n", DeviceConfiguration.STASSID.c_str());
				continue;
			}

			if (request->argName(index) == "STAPassword")
			{
				String pswd = urlDecode(request->arg(index));
				if (pswd != "")
				{
					DeviceConfiguration.STAPassword = pswd;
					DEBUGLOG("STAPassword: %s\r\n", DeviceConfiguration.STAPassword.c_str());
				}
				continue;
			}

#pragma endregion

#pragma region AP

			if (request->argName(index) == "APPassword") {
				String pswd = urlDecode(request->arg(index));
				if (pswd != "")
				{
					DeviceConfiguration.APPassword = pswd;
					DEBUGLOG("APPassword: %s\r\n", DeviceConfiguration.APPassword.c_str());
				}
				continue;
			}

#pragma endregion

#pragma region HTTP Authentication

			if (request->argName(index) == "HTTPUsername") {
				DeviceConfiguration.HTTPUsername = urlDecode(request->arg(index));
				DEBUGLOG("HTTPUsername: %s\r\n", DeviceConfiguration.HTTPUsername.c_str());
				continue;
			}

			if (request->argName(index) == "HTTPPassword") {
				String pswd = urlDecode(request->arg(index));
				if (pswd != "")
				{
					DeviceConfiguration.HTTPPassword = pswd;
					DEBUGLOG("HTTPPassword: %s\r\n", DeviceConfiguration.HTTPPassword.c_str());
				}
				continue;
			}

			if (request->argName(index) == "HTTPAuthentication") {
				DeviceConfiguration.HTTPAuthentication = true;
				DEBUGLOG("HTTPAuthentication: %d\r\n", DeviceConfiguration.HTTPAuthentication);
				continue;
			}

#pragma endregion

#pragma region Cayenne

#ifdef ENABLE_CAYENNE_MODE

			if (request->argName(index) == "CayenneUsername") {
				String uname = urlDecode(request->arg(index));
				if (uname != "")
				{
					DeviceConfiguration.CayenneUsername = uname;
					DEBUGLOG("CayenneUsername: %s\r\n", DeviceConfiguration.CayenneUsername.c_str());
				}
				continue;
			}

			if (request->argName(index) == "CayennePassword") {
				String pswd = urlDecode(request->arg(index));
				if (pswd != "")
				{
					DeviceConfiguration.CayennePassword = pswd;
					DEBUGLOG("CayennePassword: %s\r\n", DeviceConfiguration.CayennePassword.c_str());
				}
				continue;
			}

			if (request->argName(index) == "CayenneClientID") {
				String cid = urlDecode(request->arg(index));
				if (cid != "")
				{
					DeviceConfiguration.CayenneClientID = cid;
					DEBUGLOG("CayenneClientID: %s\r\n", DeviceConfiguration.CayenneClientID.c_str());
				}
				continue;
			}

#endif // ENABLE_CAYENNE_MODE

#pragma endregion

		}

		// Save device configuration.
		save_device_configuration(_fileSystem);

	}

	if (!this->handleFileRead("/configuration.html", request))
	{
		request->send(404, "text/plain", "FileNotFound");
		return;
	}
}

#pragma endregion
