// 
// 
// 

#include "ButtonGesture.h"

ButtonGestureClass ButtonGesture;

void ButtonGestureClass::configure()
{
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");

	pinMode(PIN_BUTTON, INPUT_PULLUP);
}

int ButtonGestureClass::check()
{
	int GestureL = Gestures::None;

	_ButtonState = digitalRead(PIN_BUTTON);
	// Button pressed down
	if (_ButtonState == LOW && _ButtonLastState == HIGH && (millis() - _UpTime) > _DebounceTime)
	{
		_DownTime = millis();
		_IgnoreUp = false;
		_WaitForUp = false;
		_SingleOK = true;
		_HoldEventPast = false;
		_LongHoldEventPast = false;
		if ((millis() - _UpTime) < _DoubleClickGapTime && _DoubleClickOnUp == false && _DoubleClickWaiting == true)  _DoubleClickOnUp = true;
		else  _DoubleClickOnUp = false;
		_DoubleClickWaiting = false;
	}

	// Button released
	else if (_ButtonState == HIGH && _ButtonLastState == LOW && (millis() - _DownTime) > _DebounceTime)
	{
		if (!_IgnoreUp)
		{
			_UpTime = millis();
			if (_DoubleClickOnUp == false) _DoubleClickWaiting = true;
			else
			{
				GestureL = Gestures::Released;
				_DoubleClickOnUp = false;
				_DoubleClickWaiting = false;
				_SingleOK = false;
			}
		}
	}

	// Test for normal click GestureL: _DoubleClickGapTime expired
	if (_ButtonState == HIGH && (millis() - _UpTime) >= _DoubleClickGapTime && _DoubleClickWaiting == true && _DoubleClickOnUp == false && _SingleOK == true && GestureL != 2)
	{
		GestureL = Gestures::Click;
		_DoubleClickWaiting = false;
	}

	// Test for hold
	if (_ButtonState == LOW && (millis() - _DownTime) >= _HoldTime) {
		// Trigger "normal" hold
		if (!_HoldEventPast)
		{
			GestureL = Gestures::Hold;
			_WaitForUp = true;
			_IgnoreUp = true;
			_DoubleClickOnUp = false;
			_DoubleClickWaiting = false;
			//_DownTime = millis();
			_HoldEventPast = true;
		}
		// Trigger "long" hold
		if ((millis() - _DownTime) >= _LongHoldTime)
		{
			if (!_LongHoldEventPast)
			{
				GestureL = Gestures::LongHold;
				_LongHoldEventPast = true;
			}
		}
	}
	_ButtonLastState = _ButtonState;

	if (GestureL == Gestures::Click)
	{
		if (callbackClick != nullptr)
		{
			callbackClick();
		}
	}
	else if (GestureL == Gestures::Released)
	{
		if (callbackReleased != nullptr)
		{
			callbackReleased();
		}
	}
	else if (GestureL == Gestures::Hold)
	{
		if (callbackHold != nullptr)
		{
			callbackHold();
		}
	}
	else if (GestureL == Gestures::LongHold)
	{
		if (callbackLongHold != nullptr)
		{
			callbackLongHold();
		}
	}

	return GestureL;
}

/** @brief Set callback Click geasture.
 *  @param Callback function.
 *  @return Void.
 */
void ButtonGestureClass::setCallbackClick(void(*callback)(void))
{
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");

	callbackClick = callback;
}

/** @brief Set callback Released geasture.
 *  @param Callback function.
 *  @return Void.
 */
void ButtonGestureClass::setCallbackReleased(void(*callback)(void))
{
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");

	callbackReleased = callback;
}

/** @brief Set callback Hold geasture.
 *  @param Callback function.
 *  @return Void.
 */
void ButtonGestureClass::setCallbackHold(void(*callback)(void))
{
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");

	callbackHold = callback;
}

/** @brief Set callback LongHold geasture.
 *  @param Callback function.
 *  @return Void.
 */
void ButtonGestureClass::setCallbackLongHold(void(*callback)(void))
{
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");

	callbackLongHold = callback;
}
