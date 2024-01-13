#pragma once

#include "KeyboardEvent.h"
#include <thread>

class KeyboardEventsWorker
{
public:
	KeyboardEventsWorker(clock_t& globalClock);
	~KeyboardEventsWorker();

	void startListening();
	void stopListening();

	std::vector<KeyboardEvent>::const_iterator constBeginIterator() const;
	std::vector<KeyboardEvent>::const_iterator constEndIterator() const;

private:
	const clock_t& mGlobalClock;
	bool mContinueListening;

	std::vector<KeyboardEvent> mKeyboardEvents;
	std::thread mListenThread;

	void listenLoop();
	void resetWindowsPressedKeysBuffer();
};