#pragma once

#include <Windows.h>
#include <time.h>
#include <vector>
#include <unordered_map>

class MouseClickEvent
{
public:
	static const std::vector<int8_t> VK;

	enum class KeyState
	{
		KEY_UP,
		KEY_DOWN
	};

	MouseClickEvent(clock_t eventTime, POINT clickPosition, int8_t pressedKey, KeyState keyState) noexcept;

	[[nodiscard]] constexpr clock_t eventTime() const
	{
		return mEventTime;
	}

	void play(INPUT& tempInput) const;

private:
	static DWORD convertEventFlags(int8_t pressedKey, KeyState keyState);
	static DWORD convertMouseData(int8_t pressedKey);

	static const std::unordered_map<int8_t, DWORD> KEY_UP_FLAGS;
	static const std::unordered_map<int8_t, DWORD> KEY_DOWN_FLAGS;
	static const std::unordered_map<int8_t, DWORD> MOUSE_DATA;

	const clock_t mEventTime;
	const POINT mMousePosition;
	const DWORD mMouseData;
	const DWORD mEventFlags;
};