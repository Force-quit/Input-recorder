#include "../Headers/EQInputRecorderWorker.h"
#include <QTimer>
#include <QString>
#include <Windows.h>
#include <QThread>

EQInputRecorderWorker::EQInputRecorderWorker()
	: mGlobalClock{},
	mLooping{},
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

	mMouseMoveWorker.stopListening();
	mMouseClickWorker.stopListening();
	mKeyboardWorker.stopListening();

	emit textChanged("Finished recording");
	emit finishedRecording();
}

void EQInputRecorderWorker::playback()
{
	auto mouseMoveEventsIt = mMouseMoveWorker.constBeginIterator();
	auto mouseClickEventsIt = mMouseClickWorker.constBeginIterator();
	auto keyboardEventsIt = mKeyboardWorker.constBeginIterator();

	long currentPlaybackTime{};
	clock_t playbackStart{ std::clock() };
	INPUT input{};
	bool userStopped{};

	SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_AWAYMODE_REQUIRED);

	emit textChanged("Playback started");

	do
	{
		currentPlaybackTime = std::clock() - playbackStart;

		while (mouseMoveEventsIt != mMouseMoveWorker.constEndIterator() && mouseMoveEventsIt->eventPlayTime() <= currentPlaybackTime)
		{
			mouseMoveEventsIt->play();
			++mouseMoveEventsIt;
		}

		while (mouseClickEventsIt != mMouseClickWorker.constEndIterator() && mouseClickEventsIt->eventTime() <= currentPlaybackTime)
		{
			mouseClickEventsIt->play(input);
			++mouseClickEventsIt;
		}

		while (keyboardEventsIt != mKeyboardWorker.constEndIterator() && keyboardEventsIt->eventTime() <= currentPlaybackTime)
		{
			keyboardEventsIt->play(input);
			++keyboardEventsIt;
		}

		QThread::msleep(1);

		userStopped = checkPlaybackStop();

	} while (currentPlaybackTime < mGlobalClock && !userStopped);

	SetThreadExecutionState(ES_CONTINUOUS);

	if (mLooping && !userStopped)
	{
		QTimer::singleShot(1, this, &EQInputRecorderWorker::playback);
		return;
	}

	if (userStopped)
	{
		if (mLooping)
			emit canceledPlaybackLoop();
		mLooping = false;
		emit textChanged("User stopped playback");
	}
	else
		emit textChanged("Playback ended");

	emit finishedPlayback();
}

bool EQInputRecorderWorker::checkPlaybackStop()
{
	return GetAsyncKeyState(VK_ESCAPE);
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