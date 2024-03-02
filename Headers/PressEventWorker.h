#pragma once

#include "EventsWorker.h"
#include "PressEvent.h"

class PressEventWorker : public EventsWorker<PressEvent>
{
public:
	PressEventWorker(clock_t& globalClock);

protected:
	void listenLoop(std::stop_token stopToken) override;
};