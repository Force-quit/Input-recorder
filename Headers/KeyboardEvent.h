#pragma once

#include <Windows.h>
#include <ctime>
#include <array>


class KeyboardEvent
{
public:
	static constexpr std::array<uint8_t, 104> VK{

		VK_BACK,
		VK_TAB,
		VK_CLEAR,
		VK_RETURN,
		VK_SHIFT,
		VK_CONTROL,
		VK_LWIN,
		VK_MENU,
		VK_PAUSE,
		VK_CAPITAL,
		VK_ESCAPE,
		VK_SPACE,
		VK_PRIOR,
		VK_NEXT,
		VK_END,
		VK_HOME,
		VK_SELECT,
		VK_PRINT,
		VK_EXECUTE,
		VK_SNAPSHOT,
		VK_INSERT,
		VK_DELETE,
		VK_HELP,
		VK_LEFT,
		VK_UP,
		VK_RIGHT,
		VK_DOWN,
		0x31,
		0x32,
		0x33,
		0x34,
		0x35,
		0x36,
		0x37,
		0x38,
		0x39,
		0x30,
		VK_OEM_MINUS,
		VK_OEM_PLUS,
		0x41,
		0x42,
		0x43,
		0x44,
		0x45,
		0x46,
		0x47,
		0x48,
		0x49,
		0x4A,
		0x4B,
		0x4C,
		0x4D,
		0x4E,
		0x4F,
		0x50,
		0x51,
		0x52,
		0x53,
		0x54,
		0x55,
		0x56,
		0x57,
		0x58,
		0x59,
		0x5A,
		VK_OEM_COMMA,
		VK_OEM_PERIOD,
		VK_OEM_1,
		VK_OEM_2,
		VK_OEM_3,
		VK_OEM_4,
		VK_OEM_5,
		VK_OEM_6,
		VK_OEM_7,
		VK_OEM_8,
		VK_OEM_102,
		VK_NUMPAD0,
		VK_NUMPAD1,
		VK_NUMPAD2,
		VK_NUMPAD3,
		VK_NUMPAD4,
		VK_NUMPAD5,
		VK_NUMPAD6,
		VK_NUMPAD7,
		VK_NUMPAD8,
		VK_NUMPAD9,
		VK_MULTIPLY,
		VK_ADD,
		VK_SEPARATOR,
		VK_SUBTRACT,
		VK_DECIMAL,
		VK_DIVIDE,
		VK_F1,
		VK_F2,
		VK_F3,
		VK_F4,
		VK_F5,
		VK_F6,
		VK_F7,
		VK_F8,
		VK_F9,
		VK_F10,
		VK_F11,
		VK_F12
	};

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
