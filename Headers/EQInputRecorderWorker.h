#pragma once

#include <Windows.h>
#include <QObject>
#include <QString>

#include "MouseMoveEventsWorker.h"
#include "MouseClickEventsWorker.h"
#include "KeyboardEventsWorker.h"

class EQInputRecorderWorker  : public QObject
{
	Q_OBJECT

public:
	EQInputRecorderWorker();

	void setPlaybackLoop(bool playbackLoop);

public slots:
	void startRecording();
	void startPlayback();

signals:
	void displayText(const QString& newText);
	void finishedRecording();
	void finishedPlayback();

private:
	static constexpr uint8_t COUNTDOWN{ 3 };

	enum class Sequence
	{
		PLAYBACK,
		RECORDING
	};

	void prepareFor(Sequence sequence);

	void record();
	void playback();

	clock_t mGlobalClock;
	clock_t mPreviousRecordingTime;
	bool mLooping;

	MouseClickEventsWorker mMouseClickWorker;
	MouseMoveEventsWorker mMouseMoveWorker;
	KeyboardEventsWorker mKeyboardWorker;
};