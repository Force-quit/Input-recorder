#pragma once

#include <ctime>
#include <vector>
#include "MouseMoveEvent.h"
#include <thread>

class MouseMoveEventsWorker
{
public:
	MouseMoveEventsWorker(clock_t& globalClock);
	~MouseMoveEventsWorker();

	void startListening();
	void stopListening();

	std::vector<MouseMoveEvent>::const_iterator constBeginIterator() const;
	std::vector<MouseMoveEvent>::const_iterator constEndIterator() const;

private:
	const clock_t& mGlobalClock;
	bool mContinueListening;

	std::vector<MouseMoveEvent> mMouseMoveEvents;
	std::thread mListenThread;

	void listenLoop();
};