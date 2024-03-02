#include "../Headers/EQInputRecorderWorker.h"
#include <QTimer>
#include <QString>
#include <Windows.h>
#include <QThread>

EQInputRecorderWorker::EQInputRecorderWorker()
	: mGlobalClock{},
	mPlaybackLooping{},
	mPreviousRecordingTime(mGlobalClock),
	mPressEventWorker(mGlobalClock),
	mMouseMoveWorker(mGlobalClock)
{
}

void EQInputRecorderWorker::startRecording()
{
	prepareFor(Sequence::RECORDING);
}

void EQInputRecorderWorker::startPlayback()
{
	prepareFor(Sequence::PLAYBACK);
}

void EQInputRecorderWorker::record()
{
	emit displayText("Recording started");

	clock_t wRecordingStart{ std::clock() };
	mGlobalClock = wRecordingStart;

	mPressEventWorker.startListening();
	mMouseMoveWorker.startListening();

	while (!eutilities::isPressed(eutilities::Key::ESCAPE) && !QThread::currentThread()->isInterruptionRequested())
	{
		mGlobalClock = std::clock() - wRecordingStart;
		QThread::msleep(1);
	}

	mPreviousRecordingTime = mGlobalClock;
	mMouseMoveWorker.stopListening();
	mPressEventWorker.stopListening();

	emit finishedRecording();
	emit displayText("Finished recording");
}

void EQInputRecorderWorker::playback()
{
	emit displayText("Playback started");
	EXECUTION_STATE wPreviousExecutionState{ SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED) };

	bool wUserStoppedPlayback;
	do 
	{
		wUserStoppedPlayback = playbackOnce();
	} while (mPlaybackLooping && !wUserStoppedPlayback);
	
	emit finishedPlayback();
	SetThreadExecutionState(wPreviousExecutionState);
	emit displayText("Playback ended");
}

bool EQInputRecorderWorker::playbackOnce()
{
	auto wMouseMoveEventsIt = mMouseMoveWorker.constBeginIterator();
	auto wMouseClickEventsIt = mPressEventWorker.constBeginIterator();
	bool wUserStoppedPlayback{};
	clock_t wPlaybackStart{ std::clock() };

	do 
	{
		mGlobalClock = std::clock() - wPlaybackStart;

		while (wMouseMoveEventsIt != mMouseMoveWorker.constEndIterator() && wMouseMoveEventsIt->mEventTime <= mGlobalClock)
		{
			wMouseMoveEventsIt->play();
			++wMouseMoveEventsIt;
		}

		while (wMouseClickEventsIt != mPressEventWorker.constEndIterator() && wMouseClickEventsIt->mEventTime <= mGlobalClock)
		{
			wMouseClickEventsIt->play();
			++wMouseClickEventsIt;
		}

		QThread::msleep(1);

		wUserStoppedPlayback = eutilities::isPressed(eutilities::Key::ESCAPE) || QThread::currentThread()->isInterruptionRequested();
	} while (mGlobalClock < mPreviousRecordingTime && !wUserStoppedPlayback);

	return wUserStoppedPlayback;
}

void EQInputRecorderWorker::prepareFor(Sequence sequence)
{
	QString textToShow;

	if (sequence == Sequence::RECORDING)
	{
		textToShow = "Recording";
		QTimer::singleShot(COUNTDOWN * 1000, this, &EQInputRecorderWorker::record);
	}
	else
	{
		textToShow = "Playback";
		QTimer::singleShot(COUNTDOWN * 1000, this, &EQInputRecorderWorker::playback);
	}

	for (uint8_t i{}; i < COUNTDOWN; ++i)
	{
		QTimer::singleShot(i * 1000, [=]()
			{
				emit displayText(textToShow + " in " + QString::number(COUNTDOWN - i) + "...");
			});
	}
}

void EQInputRecorderWorker::setPlaybackLoop(bool playbackLoop)
{
	mPlaybackLooping = playbackLoop;
}