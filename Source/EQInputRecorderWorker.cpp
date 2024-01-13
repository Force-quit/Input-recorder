#include "../Headers/EQInputRecorderWorker.h"
#include <QTimer>
#include <QString>
#include <Windows.h>
#include <QThread>

EQInputRecorderWorker::EQInputRecorderWorker()
	: mGlobalClock{},
	mLooping{},
	mPreviousRecordingTime{},
	mMouseClickWorker(mGlobalClock),
	mMouseMoveWorker(mGlobalClock),
	mKeyboardWorker(mGlobalClock)
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
	clock_t start{ std::clock() };
	mGlobalClock = start;
	emit textChanged("Recording started");

	mKeyboardWorker.startListening();
	mMouseClickWorker.startListening();
	mMouseMoveWorker.startListening();

	GetAsyncKeyState(VK_ESCAPE);
	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		mGlobalClock = std::clock() - start;
		QThread::msleep(1);
	}

	emit textChanged("Processing data..");

	mPreviousRecordingTime = mGlobalClock;
	mMouseMoveWorker.stopListening();
	mMouseClickWorker.stopListening();
	mKeyboardWorker.stopListening();

	emit textChanged("Finished recording");
	emit finishedRecording();
}

void EQInputRecorderWorker::playback()
{
	bool wUserStopped{};
	EXECUTION_STATE wPreviousExecutionState{ SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED) };
	INPUT wTempInputStruct{};
	emit textChanged("Playback started");

	do
	{
		auto wMouseMoveEventsIt = mMouseMoveWorker.constBeginIterator();
		auto wMouseClickEventsIt = mMouseClickWorker.constBeginIterator();
		auto wKeyboardEventsIt = mKeyboardWorker.constBeginIterator();

		clock_t wPlaybackStart{ std::clock() };

		do
		{
			mGlobalClock = std::clock() - wPlaybackStart;

			while (wMouseMoveEventsIt != mMouseMoveWorker.constEndIterator() && wMouseMoveEventsIt->eventPlayTime() <= mGlobalClock)
			{
				wMouseMoveEventsIt->play();
				++wMouseMoveEventsIt;
			}

			while (wMouseClickEventsIt != mMouseClickWorker.constEndIterator() && wMouseClickEventsIt->eventTime() <= mGlobalClock)
			{
				wMouseClickEventsIt->play(wTempInputStruct);
				++wMouseClickEventsIt;
			}

			while (wKeyboardEventsIt != mKeyboardWorker.constEndIterator() && wKeyboardEventsIt->eventTime() <= mGlobalClock)
			{
				wKeyboardEventsIt->play(wTempInputStruct);
				++wKeyboardEventsIt;
			}

			QThread::msleep(1);

			wUserStopped = GetAsyncKeyState(VK_ESCAPE);

		} while (mGlobalClock < mPreviousRecordingTime && !wUserStopped);
	} while (mLooping && !wUserStopped);
	
	emit finishedPlayback();
	emit textChanged("Playback ended");
	SetThreadExecutionState(wPreviousExecutionState);
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
				emit textChanged(textToShow + " in " + QString::number(COUNTDOWN - i) + "...");
			});
	}
}

void EQInputRecorderWorker::setPlaybackLoop(bool playbackLoop)
{
	mLooping = playbackLoop;
}