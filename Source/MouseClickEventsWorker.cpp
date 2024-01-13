#include "../Headers/MouseClickEventsWorker.h"
#include <set>

MouseClickEventsWorker::MouseClickEventsWorker(clock_t& globalClock)
	: EventsWorker(globalClock)
{

}

void MouseClickEventsWorker::listenLoop()
{
	std::set<uint8_t> wPressedKeys;
	POINT wMousePos{};

	while (mContinueListening)
	{
		for (uint8_t wObservedKey : MouseClickEvent::VK)
		{
			if (GetAsyncKeyState(wObservedKey))
			{
				if (!wPressedKeys.contains(wObservedKey))
				{
					GetCursorPos(&wMousePos);
					mEvents.emplace_back(mGlobalClock, wMousePos, wObservedKey, MouseClickEvent::KeyState::KEY_DOWN);
					wPressedKeys.insert(wObservedKey);
				}
			}
			else if (wPressedKeys.contains(wObservedKey))
			{
				mEvents.emplace_back(mGlobalClock, wMousePos, wObservedKey, MouseClickEvent::KeyState::KEY_UP);
				wPressedKeys.erase(wObservedKey);
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void MouseClickEventsWorker::resetWindowsPressedKeysBuffer()
{
	for (uint8_t wObservedKey : MouseClickEvent::VK)
	{
		GetAsyncKeyState(wObservedKey);
	}
}
