#include "../Headers/KeyboardEventsWorker.h"

import eutilities;
#include <array>

KeyboardEventsWorker::KeyboardEventsWorker(clock_t& globalClock)
	: EventsWorker(globalClock)
{

}

void KeyboardEventsWorker::listenLoop(std::stop_token stopToken)
{
	std::array<bool, eutilities::keyboardKeys.size()> wPressedKeys{};

	while (!stopToken.stop_requested())	
	{
		for (size_t i{}; i < eutilities::keyboardKeys.size(); ++i)
		{
			if (eutilities::isPressed(eutilities::keyboardKeys[i]))
			{
				if (!wPressedKeys[i])
				{
					mEvents.emplace_back(mGlobalClock, eutilities::keyboardKeys[i], KeyboardEvent::KeyState::KEY_DOWN);
					wPressedKeys[i] = true;
				}
			}
			else if (wPressedKeys[i])
			{
				mEvents.emplace_back(mGlobalClock, eutilities::keyboardKeys[i], KeyboardEvent::KeyState::KEY_UP);
				wPressedKeys[i] = false;
			}
		}

		eutilities::sleepFor(1);
	}

	// escape key up
	if (mEvents.size() != 0 && mEvents.back().virtualKey() == eutilities::Key::ESCAPE)
	{
		mEvents.pop_back(); 
	}
	// escape key down
	if (mEvents.size() != 0 && mEvents.back().virtualKey() == eutilities::Key::ESCAPE)
	{
		mEvents.pop_back();
	}
}