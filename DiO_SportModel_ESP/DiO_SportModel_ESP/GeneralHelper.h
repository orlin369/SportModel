// GeneralHelper.h

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

#ifndef _GENERALHELPER_h
#define _GENERALHELPER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#pragma region Definitions

#pragma endregion

#pragma region Headers

#include <ESP8266WiFi.h>

#pragma endregion

#pragma region Prototypes

/** @brief Get MAC address.
 *  @return String, Returns the string of MAC address.
 */
String mac2str(const uint8 * mac);

/** @brief Convert a single hex digit character to its integer value. Based on https://code.google.com/p/avr-netino/
 *  @return String, Returns the string of unified URL string.
 */
unsigned char hex2dec(char c);

/** @brief Converts binary array to heximal string.
 *  @param unsigned char * input, Binary input.
 *  @param unsigned int input_size, Binary input size.
 *  @param char * output, Output result.
 *  @return boolean, Returns the true if value is in the range.
 */
void bin_to_strhex(uint8 *input, unsigned int input_size, uint8 *output);

/** @brief Check the Values is between: [0 - 255].
 *  @param value String, Value of the octet.
 *  @return boolean, Returns the true if value is in the range.
 */
boolean check_octet_range(String value);

/** @brief Check the Values is between: [0 - 255].
 *  @param value int, Value of the octet.
 *  @return boolean, Returns the true if value is in the range.
 */
boolean check_octet_range(int value);

/** @brief Convert size to nice text type.
 *  @param bytes, size_t Size of the file.
 *  @return String, Nice formated size text.
 */
String formatBytes(size_t bytes);

/** @brief Convert many minutes to ending minutes.
 *  @param int minutes, Minutest.
 *  @return int, Ending minutes.
 */
int to_minutes(int minutes);

/** @brief Convert many minutes to hours.
 *  @param int minutes, Minutest.
 *  @return int, Hours.
 */
int to_hours(int minutes);

#pragma endregion

#endif

