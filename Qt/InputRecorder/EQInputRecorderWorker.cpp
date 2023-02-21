#include "EQInputRecorderWorker.h"
#include <QTimer>
#include <QString>
#include <Windows.h>
#include "MouseEventsWorker.h"
#include <QVector>

EQInputRecorderWorker::EQInputRecorderWorker()
	: recordingTime{},
	mouseEventsWorker{ new MouseEventsWorker(recordingTime)}, mouseEventsThread(),
	keyboardEventsHandler(this, recordingTime),
	mouseMoveEvents(), mouseClickEvents(), keyboardEvents()
{
	mouseEventsWorker->moveToThread(&mouseEventsThread);
	connect(&mouseEventsThread, &QThread::finished, mouseEventsWorker, &QObject::deleteLater);
	mouseEventsThread.start();

	connect(this, &EQInputRecorderWorker::startListening, mouseEventsWorker, &MouseEventsWorker::startListening);
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

	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		recordingTime = std::clock() - start;
		QThread::msleep(1);
	}

	mouseEventsWorker->stopListening();
	keyboardEventsHandler.stopListening();

	while (!mouseEventsWorker->isReadyToShare())
		QThread::msleep(5);

    mouseMoveEvents = mouseEventsWorker->getMouseMoveEvents();
	mouseClickEvents = mouseEventsWorker->getMouseClickEvents();

	keyboardEvents = keyboardEventsHandler.getKeyboardEvents();

	if (keyboardEvents.back().getVkCode() == VK_ESCAPE)
		keyboardEvents.removeLast();

	emit textChanged("Finished recording");
	emit finishedRecording();
}

void EQInputRecorderWorker::startRealPlayBack()
{
	auto mouseMoveEventsIt = mouseMoveEvents.begin();
	auto mouseClickEventsIt = mouseClickEvents.begin();
	auto keyboardEventsIt = keyboardEvents.begin();

	long currentPlaybackTime{};
	clock_t playbackStart{ std::clock() };

	EQMouseMoveEvent* nextMouseMoveEvent{};
	EQMouseClickEvent* nextMouseClickEvent{};
	EQKeyboardEvent* nextKeyboardEvent{};
	INPUT input{};

	emit textChanged("Playback started");

	do
	{
		currentPlaybackTime = std::clock() - playbackStart;

		while (mouseMoveEventsIt != mouseMoveEvents.end() && mouseMoveEventsIt->eventPlayTime() <= currentPlaybackTime)
		{
			mouseMoveEventsIt->play();
			++mouseMoveEventsIt;
		}

		while (mouseClickEventsIt != mouseClickEvents.end() && mouseClickEventsIt->eventPlayTime() <= currentPlaybackTime)
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

	} while (currentPlaybackTime < recordingTime && !userStopPlayback());
	
	emit textChanged("Finished playback");
	emit finishedPlayback();
}

bool EQInputRecorderWorker::userStopPlayback() const
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

EQInputRecorderWorker::~EQInputRecorderWorker() 
{
	//mouseEventsWorker->stopListening();
	// TODO
	//keyboardEvents.stopListening();
	mouseEventsThread.quit();
	mouseEventsThread.wait();
}