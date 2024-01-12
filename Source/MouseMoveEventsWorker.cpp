#include "../Headers/MouseMoveEventsWorker.h"

MouseMoveEventsWorker::MouseMoveEventsWorker(clock_t& currentRecTime)
	: currentRecTime{currentRecTime},
	continueListening{},
	mouseMoveEvents(),
	listenThread()
{

}

MouseMoveEventsWorker::~MouseMoveEventsWorker()
{
	if (listenThread.joinable())
	{
		stopListening();
	}
}

void MouseMoveEventsWorker::stopListening()
{
	continueListening = false;
	listenThread.join();
}

void MouseMoveEventsWorker::startListening()
{
	continueListening = true;
	mouseMoveEvents.clear();
	listenThread = std::thread(&MouseMoveEventsWorker::listenLoop, this);
}

std::vector<MouseMoveEvent>::const_iterator MouseMoveEventsWorker::constBeginIterator() const
{
	return mouseMoveEvents.cbegin();
}

std::vector<MouseMoveEvent>::const_iterator MouseMoveEventsWorker::constEndIterator() const
{
	return mouseMoveEvents.cend();
}

void MouseMoveEventsWorker::listenLoop()
{
	POINT previousMousePos;
	GetCursorPos(&previousMousePos);

	POINT tempPoint;
	while (continueListening)
	{
		GetCursorPos(&tempPoint);
		if (tempPoint.x != previousMousePos.x || tempPoint.y != previousMousePos.y)
		{
			previousMousePos = tempPoint;
			mouseMoveEvents.emplace_back(currentRecTime, previousMousePos);
		}

		// Maybe a sleep here
	}
}
