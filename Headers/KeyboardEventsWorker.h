#pragma once

#include "KeyboardEvent.h"
#include "EventsWorker.h"

class KeyboardEventsWorker : public EventsWorker<KeyboardEvent>
{
public:
	KeyboardEventsWorker(clock_t& globalClock);

protected:
	void listenLoop(std::stop_token stopToken) override;
};