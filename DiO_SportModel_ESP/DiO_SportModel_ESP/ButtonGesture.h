// ButtonGeasture.h

#ifndef _BUTTONGEASTURE_h
#define _BUTTONGEASTURE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#pragma region Headres

#include "ApplicationConfiguration.h"

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
	void configure();

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
};

extern ButtonGestureClass ButtonGesture;

#endif

