#include "../Headers/MouseClickEvent.h"

const std::unordered_map<int8_t, DWORD> MouseClickEvent::KEY_UP_FLAGS
{
	{VK_LBUTTON, MOUSEEVENTF_LEFTUP },
	{VK_RBUTTON, MOUSEEVENTF_RIGHTUP },
	{VK_MBUTTON, MOUSEEVENTF_MIDDLEUP },
	{VK_XBUTTON1, MOUSEEVENTF_XUP },
	{VK_XBUTTON2, MOUSEEVENTF_XUP }
};

const std::unordered_map<int8_t, DWORD> MouseClickEvent::KEY_DOWN_FLAGS
{
	{VK_LBUTTON, MOUSEEVENTF_LEFTDOWN},
	{VK_RBUTTON, MOUSEEVENTF_RIGHTDOWN},
	{VK_MBUTTON, MOUSEEVENTF_MIDDLEDOWN},
	{VK_XBUTTON1, MOUSEEVENTF_XDOWN},
	{VK_XBUTTON2, MOUSEEVENTF_XDOWN}
};

const std::unordered_map<int8_t, DWORD> MouseClickEvent::MOUSE_DATA
{
	{VK_XBUTTON1, XBUTTON1},
	{VK_XBUTTON2, XBUTTON2}
};

MouseClickEvent::MouseClickEvent(clock_t eventTime, POINT clickPosition, int8_t pressedKey, KeyState keyState) noexcept
	: mEventTime{ eventTime },
	mMousePosition{ clickPosition },
	mMouseData{ convertMouseData(pressedKey) },
	mEventFlags{ convertEventFlags(pressedKey, keyState) }
{

}

void MouseClickEvent::play(INPUT& tempInput) const
{
	tempInput.type = INPUT_MOUSE;
	tempInput.mi.dwFlags = mEventFlags;
	tempInput.mi.dx = mMousePosition.x;
	tempInput.mi.dy = mMousePosition.y;
	tempInput.mi.mouseData = mMouseData;
	SendInput(1, &tempInput, sizeof(INPUT));
}

DWORD MouseClickEvent::convertEventFlags(int8_t pressedKey, KeyState keyState)
{
	return keyState == KeyState::KEY_DOWN ? KEY_DOWN_FLAGS.at(pressedKey) : KEY_UP_FLAGS.at(pressedKey);
}

DWORD MouseClickEvent::convertMouseData(int8_t pressedKey)
{
	return MOUSE_DATA.contains(pressedKey) ? MOUSE_DATA.at(pressedKey) : 0;
}