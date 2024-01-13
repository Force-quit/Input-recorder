#pragma once
#include <ctime>
#include <Windows.h>

class MouseMoveEvent
{
public:
	constexpr MouseMoveEvent(clock_t positionInTime, POINT mousePosition) noexcept
		: eventPositionInTime{ positionInTime },
		eventPosition{ mousePosition }
	{

	};

	[[nodiscard]] constexpr clock_t eventPlayTime() const
	{
		return eventPositionInTime;
	}

	inline void play() const
	{
		SetCursorPos(eventPosition.x, eventPosition.y);
	}

private:
	const clock_t eventPositionInTime;
	const POINT eventPosition;
};