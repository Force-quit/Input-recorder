#pragma once

#pragma once

#include <ctime>
#include <vector>
#include "MouseClickEvent.h"
#include <thread>

class MouseClickEventsWorker
{
public:
	MouseClickEventsWorker(clock_t& globalClock);
	~MouseClickEventsWorker();

	void startListening();
	void stopListening();

	std::vector<MouseClickEvent>::const_iterator constBeginIterator() const;
	std::vector<MouseClickEvent>::const_iterator constEndIterator() const;

private:
	const clock_t& mGlobalClock;
	bool mContinueListening;

	std::vector<MouseClickEvent> mMouseClickEvents;
	std::thread mListenThread;

	void listenLoop();
};