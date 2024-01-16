#include "../Headers/KeyboardEvent.h"
#include <Windows.h>

KeyboardEvent::KeyboardEvent(clock_t positionInTime, DWORD vkCode, KeyState keyState)
	: mEventTime{ positionInTime },
	vkCode{ vkCode },
	dwFlags{ convertKeyState(keyState) }
{
}

void KeyboardEvent::play(INPUT& input) const
{
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = vkCode;
	input.ki.dwFlags = dwFlags;
	SendInput(1, &input, sizeof(INPUT));
}

constexpr DWORD KeyboardEvent::convertKeyState(KeyState keyState)
{
	return keyState == KeyState::KEY_DOWN ? 0 : KEYEVENTF_KEYUP;
}