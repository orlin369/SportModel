// ButtonGesture.h

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

#ifndef _BUTTONGESTURE_h
#define _BUTTONGESTURE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#pragma region Headres

#include "ApplicationConfiguration.h"

#include "DebugPort.h"

#pragma endregion

#pragma region Enums

/** @brief Button geasture types */
enum Gestures : int
{
	None = 0, ///< No gesture.
	Click, ///< Click.
	Released, ///< Released.
	Hold,  ///< Double click geasture.
	LongHold,  ///< Click and hold.
};

#pragma endregion

class ButtonGestureClass
{

private:

#pragma region Variables

	/** @brief Debounce period to prevent flickering when pressing or releasing the button. [ms] */
	int _DebounceTime = 20;

	/** @brief Maximum time between clicks for a double click event. [ms] */
	int _DoubleClickGapTime = 250;

	/** @brief Hold period: how long to wait for press+hold event. [ms] */
	int _HoldTime = 1000;

	/** @brief Long hold period: how long to wait for press+hold event. [ms] */
	int _LongHoldTime = 3000;

	/** @brief value read from button */
	boolean _ButtonState = HIGH;

	/** @brief Buffered value of the button's previous state. */
	boolean _ButtonLastState = HIGH;

	/** @brief Whether we're waiting for a double click (down). */
	boolean _DoubleClickWaiting = false;

	/** @brief Whether to register a double click on next release, or whether to wait and click. */
	boolean _DoubleClickOnUp = false;

	/** @brief Whether it's OK to do a single click. */
	boolean _SingleOK = true;

	/** @brief Time the button was pressed down. */
	long _DownTime = -1;

	/** @brief Time the button was released. */
	long _UpTime = -1;

	/** @brief Whether to ignore the button release because the click+hold was triggered. */
	boolean _IgnoreUp = false;

	/** @brief When held, whether to wait for the up event. */
	boolean _WaitForUp = false;

	/** @brief Whether or not the hold event happened already. */
	boolean _HoldEventPast = false;

	/** @brief Whether or not the long hold event happened already. */
	boolean _LongHoldEventPast = false;

#pragma endregion

#pragma region Callbacks

	/** @brief Callback Click event. */
	void(*callbackClick)(void);

	/** @brief Callback Released event. */
	void(*callbackReleased)(void);

	/** @brief Callback Hold event. */
	void(*callbackHold)(void);

	/** @brief Callback Long Hold event. */
	void(*callbackLongHold)(void);

#pragma endregion

protected:


public:

#pragma region Methods

	/** @brief Configure the module.
     *  @return Void.
     */
	void configure();

	/** @brief Check for the gesture.
     *  @return Button state.
     */
	int check();

	/** @brief Set callback Click geasture.
	 *  @param Callback function.
	 *  @return Void.
	 */
	void setCallbackClick(void(*callback)(void));

	/** @brief Set callback Released geasture.
	 *  @param Callback function.
	 *  @return Void.
	 */
	void setCallbackReleased(void(*callback)(void));

	/** @brief Set callback Hold geasture.
	 *  @param Callback function.
	 *  @return Void.
	 */
	void setCallbackHold(void(*callback)(void));

	/** @brief Set callback LongHold geasture.
	 *  @param Callback function.
	 *  @return Void.
	 */
	void setCallbackLongHold(void(*callback)(void));

#pragma endregion

};

/** @brief Single instance of the button gestures. (Singelton) */
extern ButtonGestureClass ButtonGesture;

#endif

