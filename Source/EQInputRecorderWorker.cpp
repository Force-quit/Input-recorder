#include "../Headers/EQInputRecorderWorker.h"
#include <QTimer>
#include <QString>
#include <Windows.h>
#include <QVector>

EQInputRecorderWorker::EQInputRecorderWorker()
	: recordingTime{}, playbackLoop{},
	keyboardEventsHandler(this, recordingTime),
	keyboardEvents(),
	mouseClickEventsWorker(recordingTime),
	mouseMoveEventsWorker(recordingTime)
{
	connect(this, &EQInputRecorderWorker::startListening, &keyboardEventsHandler, &KeyboardEventsHandler::startListening);
}

void EQInputRecorderWorker::startRecording()
{

	setupTimers(true);
}

void EQInputRecorderWorker::startPlayback()
{
	setupTimers(false);
}

void EQInputRecorderWorker::startRealRecording()
{
	clock_t start{ std::clock() };
	recordingTime = start;
	emit textChanged("Recording started");
	emit startListening();

	mouseClickEventsWorker.startListening();
	mouseMoveEventsWorker.startListening();

	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		recordingTime = std::clock() - start;
		QThread::msleep(1);
	}

	emit textChanged("Processing data..");

	mouseMoveEventsWorker.stopListening();
	mouseClickEventsWorker.stopListening();
	keyboardEventsHandler.stopListening();

	keyboardEvents = keyboardEventsHandler.getKeyboardEvents();

	if (keyboardEvents.back().getVkCode() == VK_ESCAPE)
		keyboardEvents.removeLast();

	emit textChanged("Finished recording");
	emit finishedRecording();
}

void EQInputRecorderWorker::startRealPlayBack()
{
	auto mouseMoveEventsIt = mouseMoveEventsWorker.constBeginIterator();
	const auto mouseMoveEventsEndIt = mouseMoveEventsWorker.constEndIterator();

	auto mouseClickEventsIt = mouseClickEventsWorker.constBeginIterator();
	const auto mouseClickEventsEndIt = mouseClickEventsWorker.constEndIterator();

	auto keyboardEventsIt = keyboardEvents.begin();

	long currentPlaybackTime{};
	clock_t playbackStart{ std::clock() };

	EQKeyboardEvent* nextKeyboardEvent{};
	INPUT input{};

	bool userStopped{};
	
	SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_AWAYMODE_REQUIRED);


	emit textChanged("Playback started");

	do
	{
		currentPlaybackTime = std::clock() - playbackStart;

		while (mouseMoveEventsIt != mouseMoveEventsEndIt && mouseMoveEventsIt->eventPlayTime() <= currentPlaybackTime)
		{
			mouseMoveEventsIt->play();
			++mouseMoveEventsIt;
		}

		while (mouseClickEventsIt != mouseClickEventsEndIt && mouseClickEventsIt->eventTime() <= currentPlaybackTime)
		{
			mouseClickEventsIt->play(input);
			++mouseClickEventsIt;
		}
		
		while (keyboardEventsIt != keyboardEvents.end() && keyboardEventsIt->eventPlayTime() <= currentPlaybackTime)
		{
			keyboardEventsIt->play(input);
			++keyboardEventsIt;
		}

		QThread::msleep(1);

		userStopped = checkPlaybackStop();

	} while (currentPlaybackTime < recordingTime && !userStopped);
	
	SetThreadExecutionState(ES_CONTINUOUS);

	if (playbackLoop && !userStopped)
	{
		QTimer::singleShot(1, this, &EQInputRecorderWorker::startRealPlayBack);
		return;
	}

	if (userStopped)
	{
		if (playbackLoop)
			emit canceledPlaybackLoop();
		playbackLoop = false;
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

void EQInputRecorderWorker::setupTimers(const bool recording)
{
	QString textToShow;

	if (recording)
		textToShow = "Recording";
	else
		textToShow = "Playback";

	for (uint8_t i{}; i < COUNTDOWN; ++i)
	{
		QTimer::singleShot(i * 1000, [=]() {
			emit textChanged(textToShow + " in " + QString::number(COUNTDOWN - i) + "...");
		});
	}

	QTimer::singleShot(COUNTDOWN * 1000, [=]() {
		if (recording)
			startRealRecording();
		else
			startRealPlayBack();
	});
}

void EQInputRecorderWorker::setPlaybackLoop(bool playbackLoop)
{
	this->playbackLoop = playbackLoop;
}