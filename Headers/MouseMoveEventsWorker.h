#pragma once

#include "EventsWorker.h"
#include "MouseMoveEvent.h"

class MouseMoveEventsWorker : public EventsWorker<MouseMoveEvent>
{
public:
	MouseMoveEventsWorker(clock_t& globalClock);

protected:
	void listenLoop(std::stop_token stopToken) override;
};