#pragma once

#include "EventsWorker.h"
#include "MouseClickEvent.h"

class MouseClickEventsWorker : public EventsWorker<MouseClickEvent>
{
public:
	MouseClickEventsWorker(clock_t& globalClock);

protected:
	void listenLoop(std::stop_token stopToken) override;
	void resetWindowsPressedKeysBuffer() override;
};