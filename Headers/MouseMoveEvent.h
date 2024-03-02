#pragma once
#include <ctime>
#include <Windows.h>

struct MouseMoveEvent
{
	constexpr MouseMoveEvent(clock_t positionInTime, POINT mousePosition) noexcept
		: mEventTime{ positionInTime },
		eventPosition{ mousePosition }
	{

	};

	inline void play() const
	{
		SetCursorPos(eventPosition.x, eventPosition.y);
	}

	const clock_t mEventTime;
	const POINT eventPosition;
};