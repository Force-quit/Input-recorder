#pragma once

#include <Windows.h>
#include <QObject>
#include <QString>

#include "PressEventWorker.h"
#include "MouseMoveEventsWorker.h"

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
	bool playbackOnce();

	clock_t mGlobalClock;
	clock_t mPreviousRecordingTime;
	bool mPlaybackLooping;

	MouseMoveEventsWorker mMouseMoveWorker;
	PressEventWorker mPressEventWorker;
};