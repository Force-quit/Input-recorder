#include "../Headers/MouseMoveEventsWorker.h"

MouseMoveEventsWorker::MouseMoveEventsWorker(clock_t& currentRecTime)
	: mGlobalClock{currentRecTime},
	mContinueListening{},
	mMouseMoveEvents(),
	mListenThread()
{

}

MouseMoveEventsWorker::~MouseMoveEventsWorker()
{
	if (mListenThread.joinable())
	{
		stopListening();
	}
}

void MouseMoveEventsWorker::stopListening()
{
	mContinueListening = false;
	mListenThread.join();
}

void MouseMoveEventsWorker::startListening()
{
	mContinueListening = true;
	mMouseMoveEvents.clear();
	mListenThread = std::thread(&MouseMoveEventsWorker::listenLoop, this);
}

std::vector<MouseMoveEvent>::const_iterator MouseMoveEventsWorker::constBeginIterator() const
{
	return mMouseMoveEvents.cbegin();
}

std::vector<MouseMoveEvent>::const_iterator MouseMoveEventsWorker::constEndIterator() const
{
	return mMouseMoveEvents.cend();
}

void MouseMoveEventsWorker::listenLoop()
{
	POINT wPreviousMousePos;
	GetCursorPos(&wPreviousMousePos);

	POINT wTempPoint;
	while (mContinueListening)
	{
		GetCursorPos(&wTempPoint);
		if (wTempPoint.x != wPreviousMousePos.x || wTempPoint.y != wPreviousMousePos.y)
		{
			wPreviousMousePos = wTempPoint;
			mMouseMoveEvents.emplace_back(mGlobalClock, wPreviousMousePos);
		}

		// Maybe a sleep here
	}
}
