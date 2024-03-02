#pragma once

#pragma warning(disable:5050)
import eutilities;
#include <ctime>


struct PressEvent
{
	constexpr PressEvent(std::clock_t positionInTime, eutilities::Key key, bool pressed) noexcept
		: mEventTime{ positionInTime }, pressedKey{ key }, isPressed{ pressed }
	{

	}

	void play() const
	{
		if (isPressed)
		{
			eutilities::pressKey(pressedKey);
		}
		else
		{
			eutilities::releaseKey(pressedKey);
		}
	}

	const clock_t mEventTime;
	const eutilities::Key pressedKey;
	const bool isPressed;
};
