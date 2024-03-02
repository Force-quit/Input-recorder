#include "../Headers/PressEventWorker.h"

import eutilities;
#include <array>

PressEventWorker::PressEventWorker(clock_t& globalClock)
	: EventsWorker(globalClock)
{

}

void PressEventWorker::listenLoop(std::stop_token stopToken)
{
	std::array<bool, eutilities::keys.size()> wPressedKeys{};

	while (!stopToken.stop_requested())	
	{
		for (size_t i{}; i < eutilities::keys.size(); ++i)
		{
			if (eutilities::isPressed(eutilities::keys[i]))
			{
				if (!wPressedKeys[i])
				{
					mEvents.emplace_back(mGlobalClock, eutilities::keys[i], true);
					wPressedKeys[i] = true;
				}
			}
			else if (wPressedKeys[i])
			{
				mEvents.emplace_back(mGlobalClock, eutilities::keys[i], false);
				wPressedKeys[i] = false;
			}
		}

		eutilities::sleepFor(1);
	}

	// escape key up
	if (mEvents.size() != 0 && mEvents.back().pressedKey == eutilities::Key::ESCAPE)
	{
		mEvents.pop_back(); 
	}
	// escape key down
	if (mEvents.size() != 0 && mEvents.back().pressedKey == eutilities::Key::ESCAPE)
	{
		mEvents.pop_back();
	}
}