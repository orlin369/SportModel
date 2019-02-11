// LocalWebServer.h

#ifndef _LOCALWEBSERVER_h
#define _LOCALWEBSERVER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#pragma region Headers

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <ArduinoJson.h>

#pragma endregion


class LocalWebServerClass
{
 protected:


 public:
	void init();
};

extern LocalWebServerClass LocalWebServer;

#endif

