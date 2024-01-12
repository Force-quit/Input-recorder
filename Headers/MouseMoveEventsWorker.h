#pragma once

#include <ctime>
#include <vector>
#include "MouseMoveEvent.h"
#include <thread>

class MouseMoveEventsWorker
{
public:
	MouseMoveEventsWorker(clock_t& currentRecTime);
	~MouseMoveEventsWorker();

	void startListening();
	void stopListening();

	std::vector<MouseMoveEvent>::const_iterator constBeginIterator() const;
	std::vector<MouseMoveEvent>::const_iterator constEndIterator() const;

private:
	const clock_t& currentRecTime;
	bool continueListening;

	std::vector<MouseMoveEvent> mouseMoveEvents;
	std::thread listenThread;

	void listenLoop();
};