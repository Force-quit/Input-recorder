#pragma once

#pragma warning(disable:5050)
#include <thread>
#include <vector>
#include <functional>

template <class EventClass>
class EventsWorker
{
public:
	EventsWorker(clock_t& globalClock)
		: mGlobalClock{ globalClock },
		mEvents(),
		mListenThread()
	{

	}

	void startListening()
	{
		mEvents.clear();
		mListenThread = std::jthread(std::bind_front(&EventsWorker::listenLoop, this));
	}

	void stopListening()
	{
		mListenThread.request_stop();
		mListenThread.join();
	}

	constexpr std::vector<EventClass>::const_iterator constBeginIterator() const
	{
		return mEvents.cbegin();
	}

	constexpr std::vector<EventClass>::const_iterator constEndIterator() const
	{
		return mEvents.cend();
	}

protected:
	const clock_t& mGlobalClock;
	std::vector<EventClass> mEvents;

	virtual void listenLoop(std::stop_token stopToken) = 0;

private:
	std::jthread mListenThread;
};