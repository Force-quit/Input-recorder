#include "../Headers/MouseMoveEventsWorker.h"

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

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}
