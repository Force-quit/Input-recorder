#include "../Headers/MouseMoveEventsWorker.h"

import eutilities;

MouseMoveEventsWorker::MouseMoveEventsWorker(clock_t& currentRecTime)
	: EventsWorker(currentRecTime)
{

}

void MouseMoveEventsWorker::listenLoop(std::stop_token stopToken)
{
	POINT wPreviousMousePos;
	GetCursorPos(&wPreviousMousePos);

	POINT wTempPoint;
	while (!stopToken.stop_requested())
	{
		GetCursorPos(&wTempPoint);
		if (wTempPoint.x != wPreviousMousePos.x || wTempPoint.y != wPreviousMousePos.y)
		{
			wPreviousMousePos = wTempPoint;
			mEvents.emplace_back(mGlobalClock, wPreviousMousePos);
		}

		eutilities::sleepFor(1);
	}
}
