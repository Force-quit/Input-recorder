#include "../Headers/MouseClickEventsWorker.h"
#include <set>

MouseClickEventsWorker::MouseClickEventsWorker(clock_t& currentRecTime)
	: mGlobalClock{ currentRecTime },
	mContinueListening{},
	mMouseClickEvents(),
	mListenThread()
{

}

MouseClickEventsWorker::~MouseClickEventsWorker()
{
	if (mListenThread.joinable())
	{
		stopListening();
	}
}

void MouseClickEventsWorker::stopListening()
{
	mContinueListening = false;
	mListenThread.join();
}

void MouseClickEventsWorker::startListening()
{
	mContinueListening = true;
	mMouseClickEvents.clear();
	mListenThread = std::thread(&MouseClickEventsWorker::listenLoop, this);
}

std::vector<MouseClickEvent>::const_iterator MouseClickEventsWorker::constBeginIterator() const
{
	return mMouseClickEvents.cbegin();
}

std::vector<MouseClickEvent>::const_iterator MouseClickEventsWorker::constEndIterator() const
{
	return mMouseClickEvents.cend();
}

void MouseClickEventsWorker::listenLoop()
{
	std::set<uint8_t> mousePressedKeys;
	POINT mousePos{};

	while (mContinueListening)
	{
		for (uint8_t observedKey : MouseClickEvent::VK)
		{
			if (GetAsyncKeyState(observedKey))
			{
				if (!mousePressedKeys.contains(observedKey))
				{
					GetCursorPos(&mousePos);
					mMouseClickEvents.emplace_back(mGlobalClock, mousePos, observedKey, MouseClickEvent::KeyState::KEY_DOWN);
					mousePressedKeys.insert(observedKey);
				}
			}
			else if (mousePressedKeys.contains(observedKey))
			{
				mMouseClickEvents.emplace_back(mGlobalClock, mousePos, observedKey, MouseClickEvent::KeyState::KEY_UP);
				mousePressedKeys.erase(observedKey);
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Otherwise this function takes too much CPU power
		}
	}
}
