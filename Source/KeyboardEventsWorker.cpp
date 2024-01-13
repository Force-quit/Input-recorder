#include "../Headers/KeyboardEventsWorker.h"
#include <set>

KeyboardEventsWorker::KeyboardEventsWorker(clock_t& currentRecTime)
	: mGlobalClock{ currentRecTime },
	mContinueListening{},
	mKeyboardEvents(),
	mListenThread()
{

}

KeyboardEventsWorker::~KeyboardEventsWorker()
{
	if (mListenThread.joinable())
	{
		stopListening();
	}
}

void KeyboardEventsWorker::stopListening()
{
	mContinueListening = false;
	mListenThread.join();
}

void KeyboardEventsWorker::startListening()
{
	mContinueListening = true;
	mKeyboardEvents.clear();
	mListenThread = std::thread(&KeyboardEventsWorker::listenLoop, this);
}

std::vector<KeyboardEvent>::const_iterator KeyboardEventsWorker::constBeginIterator() const
{
	return mKeyboardEvents.cbegin();
}

std::vector<KeyboardEvent>::const_iterator KeyboardEventsWorker::constEndIterator() const
{
	return mKeyboardEvents.cend();
}

void KeyboardEventsWorker::listenLoop()
{
	std::set<uint8_t> pressedKeys;

	resetWindowsPressedKeysBuffer();

	while (mContinueListening)
	{
		for (uint8_t observedKey : KeyboardEvent::VK)
		{
			if (GetAsyncKeyState(observedKey))
			{
				if (!pressedKeys.contains(observedKey))
				{
					mKeyboardEvents.emplace_back(mGlobalClock, observedKey, KeyboardEvent::KeyState::KEY_DOWN);
					pressedKeys.insert(observedKey);
				}
			}
			else if (pressedKeys.contains(observedKey))
			{
				mKeyboardEvents.emplace_back(mGlobalClock, observedKey, KeyboardEvent::KeyState::KEY_UP);
				pressedKeys.erase(observedKey);
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	// escape key up
	if (mKeyboardEvents.back().virtualKey() == VK_ESCAPE)
	{
		mKeyboardEvents.pop_back(); 
	}
	// escape key down
	if (mKeyboardEvents.back().virtualKey() == VK_ESCAPE)
	{
		mKeyboardEvents.pop_back();
	}
}

void KeyboardEventsWorker::resetWindowsPressedKeysBuffer()
{
	for (uint8_t observedKey : KeyboardEvent::VK)
	{
		GetAsyncKeyState(observedKey);
	}
}
