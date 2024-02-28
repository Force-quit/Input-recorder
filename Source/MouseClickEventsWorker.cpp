#include "../Headers/MouseClickEventsWorker.h"
#include <array>

MouseClickEventsWorker::MouseClickEventsWorker(clock_t& globalClock)
	: EventsWorker(globalClock)
{

}

void MouseClickEventsWorker::listenLoop(std::stop_token stopToken)
{
	std::array<bool, MouseClickEvent::VK.size()> wPressedKeys{};
	POINT wMousePos;

	while (!stopToken.stop_requested())
	{
		for (size_t i{}; i < MouseClickEvent::VK.size(); ++i)
		{
			auto wObservedKey = MouseClickEvent::VK[i];

			if (GetAsyncKeyState(wObservedKey))
			{
				if (!wPressedKeys[i])
				{
					GetCursorPos(&wMousePos);
					mEvents.emplace_back(mGlobalClock, wMousePos, wObservedKey, MouseClickEvent::KeyState::KEY_DOWN);
					wPressedKeys[i] = true;
				}
			}
			else if (wPressedKeys[i])
			{
				mEvents.emplace_back(mGlobalClock, wMousePos, wObservedKey, MouseClickEvent::KeyState::KEY_UP);
				wPressedKeys[i] = false;
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
