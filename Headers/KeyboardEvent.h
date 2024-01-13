#pragma once

#include <Windows.h>
#include <ctime>
#include <vector>


class KeyboardEvent
{
public:
	static const std::vector<uint8_t> VK;

	enum class KeyState
	{
		KEY_UP,
		KEY_DOWN
	};

	KeyboardEvent(clock_t positionInTime, DWORD vkCode, KeyState keyState);

	[[nodiscard]] constexpr clock_t eventTime() const
	{
		return mEventTime;
	}

	[[nodiscard]] constexpr DWORD virtualKey() const
	{
		return vkCode;
	}

	void play(INPUT& input) const;

private:
	static constexpr DWORD convertKeyState(KeyState keyState);

	const clock_t mEventTime;
	DWORD vkCode;
	DWORD dwFlags;
};
