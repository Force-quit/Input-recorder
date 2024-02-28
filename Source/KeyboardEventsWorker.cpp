#include "../Headers/KeyboardEventsWorker.h"
#include <set>
#include <array>

KeyboardEventsWorker::KeyboardEventsWorker(clock_t& globalClock)
	: EventsWorker(globalClock)
{

}

void KeyboardEventsWorker::listenLoop(std::stop_token stopToken)
{
	std::array<bool, KeyboardEvent::VK.size()> wPressedKeys{};

	while (!stopToken.stop_requested())	
	{
		for (size_t i{}; i < KeyboardEvent::VK.size(); ++i)
		{
			auto wObservedKey = KeyboardEvent::VK[i];
			if (GetAsyncKeyState(wObservedKey))
			{
				if (!wPressedKeys[i])
				{
					mEvents.emplace_back(mGlobalClock, wObservedKey, KeyboardEvent::KeyState::KEY_DOWN);
					wPressedKeys[i] = true;
				}
			}
			else if (wPressedKeys[i])
			{
				mEvents.emplace_back(mGlobalClock, wObservedKey, KeyboardEvent::KeyState::KEY_UP);
				wPressedKeys[i] = false;
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	// escape key up
	if (mEvents.size() != 0 && mEvents.back().virtualKey() == VK_ESCAPE)
	{
		mEvents.pop_back(); 
	}
	// escape key down
	if (mEvents.size() != 0 && mEvents.back().virtualKey() == VK_ESCAPE)
	{
		mEvents.pop_back();
	}
}

void KeyboardEventsWorker::resetWindowsPressedKeysBuffer()
{
	for (uint8_t observedKey : KeyboardEvent::VK)
	{
		GetAsyncKeyState(observedKey);
	}
}
