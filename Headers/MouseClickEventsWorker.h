#pragma once

#include "EventsWorker.h"
#include "MouseClickEvent.h"

class MouseClickEventsWorker : public EventsWorker<MouseClickEvent>
{
public:
	MouseClickEventsWorker(clock_t& globalClock);

protected:
	void listenLoop() override;
	void resetWindowsPressedKeysBuffer() override;
};