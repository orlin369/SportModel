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

#include "UpdateManager.h"

/** @brief Check for self update,
 *         and if there is, just run it.
 *  @return Void.
 */
void  UpdateManagerClass::checkForUpdates()
{
#ifndef RELEASE
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");
#endif // !RELEASE

	uint8 c_MACL[WL_MAC_ADDR_LENGTH];
	WiFi.macAddress(c_MACL);
	String MACL = mac2str(c_MACL);

	String BinariImageUrlL = String(SERVER_DOMAIN) + String(UPDATE_SERVER_PATH);
	String FwVersUrlL = String(SERVER_DOMAIN) + String(VERSION_SERVER_PATH);

	DEBUGLOG("Checking for firmware updates.\r\n");
	DEBUGLOG("MAC address: %s\r\n", MACL.c_str());
	DEBUGLOG("Firmware version URL: %s\r\n", FwVersUrlL.c_str());

	HTTPClient HttpClientL;

	int StatusCodeL;

	HttpClientL.begin(FwVersUrlL);
	StatusCodeL = HttpClientL.GET();

	if (StatusCodeL == 200)
	{
		String NewFwVersionL = HttpClientL.getString();
		int VersionNumberL = NewFwVersionL.toInt();

		DEBUGLOG("Current   firmware version: %d\r\n", FW_VERSION);
		DEBUGLOG("Available firmware version: %d\r\n", VersionNumberL);

		if (VersionNumberL != FW_VERSION)
		{
			DEBUGLOG("Preparing to update\r\n");
			DEBUGLOG("Binary file URL : %s\r\n", BinariImageUrlL.c_str());

			t_httpUpdate_return ResponseL = ESPhttpUpdate.update(BinariImageUrlL, String(FW_VERSION));

			switch (ResponseL)
			{
			case HTTP_UPDATE_FAILED:
				DEBUGLOG("HTTP_UPDATE_FAILD Error (%d): %s\r\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
				break;

			case HTTP_UPDATE_NO_UPDATES:
				DEBUGLOG("HTTP_UPDATE_NO_UPDATES\r\n");
				break;

			case HTTP_UPDATE_OK:
				DEBUGLOG("HTTP_UPDATE_OK\r\n");
				break;
			}
		}
		else
		{
			DEBUGLOG("Already on latest version.\r\n");
		}
	}
	else
	{
		DEBUGLOG("Firmware version check failed, got HTTP response code %d\r\n", StatusCodeL);
	}

	HttpClientL.end();
}


UpdateManagerClass UpdateManager;

