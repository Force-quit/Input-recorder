#pragma once

#include <thread>
#include <vector>

template <class EventClass>
class EventsWorker
{
public:
	EventsWorker(clock_t& globalClock)
		: mGlobalClock{ globalClock },
		mContinueListening{},
		mEvents(),
		mListenThread()
	{

	}

	~EventsWorker()
	{
		if (mListenThread.joinable())
		{
			stopListening();
		}
	}

	void startListening()
	{
		mContinueListening = true;
		mEvents.clear();
		resetWindowsPressedKeysBuffer();
		mListenThread = std::thread(&EventsWorker::listenLoop, this);
	}

	void stopListening()
	{
		mContinueListening = false;
		mListenThread.join();
	}

	std::vector<EventClass>::const_iterator constBeginIterator() const
	{
		return mEvents.cbegin();
	}

	std::vector<EventClass>::const_iterator constEndIterator() const
	{
		return mEvents.cend();
	}

protected:
	const clock_t& mGlobalClock;
	bool mContinueListening;

	std::vector<EventClass> mEvents;

	virtual void listenLoop() = 0;
	virtual void resetWindowsPressedKeysBuffer() {};

private:
	std::thread mListenThread;
};