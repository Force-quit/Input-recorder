#pragma once

#include <Windows.h>
#include <time.h>
#include <array>
#include <unordered_map>

class MouseClickEvent
{
public:
	static constexpr std::array<int8_t, 5> VK{
		VK_LBUTTON,
		VK_RBUTTON,
		VK_MBUTTON,
		VK_XBUTTON1,
		VK_XBUTTON2
	};

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