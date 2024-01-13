#include "../Headers/KeyboardEventsWorker.h"
#include <set>

KeyboardEventsWorker::KeyboardEventsWorker(clock_t& globalClock)
	: EventsWorker(globalClock)
{

}

void KeyboardEventsWorker::listenLoop()
{
	std::set<uint8_t> wPressedKeys;

	while (mContinueListening)
	{
		for (uint8_t observedKey : KeyboardEvent::VK)
		{
			if (GetAsyncKeyState(observedKey))
			{
				if (!wPressedKeys.contains(observedKey))
				{
					mEvents.emplace_back(mGlobalClock, observedKey, KeyboardEvent::KeyState::KEY_DOWN);
					wPressedKeys.insert(observedKey);
				}
			}
			else if (wPressedKeys.contains(observedKey))
			{
				mEvents.emplace_back(mGlobalClock, observedKey, KeyboardEvent::KeyState::KEY_UP);
				wPressedKeys.erase(observedKey);
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	// escape key up
	if (mEvents.back().virtualKey() == VK_ESCAPE)
	{
		mEvents.pop_back(); 
	}
	// escape key down
	if (mEvents.back().virtualKey() == VK_ESCAPE)
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
