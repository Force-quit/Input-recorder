#include "../Headers/MouseClickEventsWorker.h"

import eutilities;
#include <array>

MouseClickEventsWorker::MouseClickEventsWorker(clock_t& globalClock)
	: EventsWorker(globalClock)
{

}

void MouseClickEventsWorker::listenLoop(std::stop_token stopToken)
{
	std::array<bool, eutilities::mouseKeys.size()> wPressedKeys{};
	POINT wMousePos;

	while (!stopToken.stop_requested())
	{
		for (size_t i{}; i < eutilities::mouseKeys.size(); ++i)
		{
			if (eutilities::isPressed(eutilities::mouseKeys[i]))
			{
				if (!wPressedKeys[i])
				{
					GetCursorPos(&wMousePos);
					mEvents.emplace_back(mGlobalClock, wMousePos, eutilities::mouseKeys[i], MouseClickEvent::KeyState::KEY_DOWN);
					wPressedKeys[i] = true;
				}
			}
			else if (wPressedKeys[i])
			{
				mEvents.emplace_back(mGlobalClock, wMousePos, eutilities::mouseKeys[i], MouseClickEvent::KeyState::KEY_UP);
				wPressedKeys[i] = false;
			}
		}

		eutilities::sleepFor(1);
	}
}