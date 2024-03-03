#pragma once

#include <QMainWindow>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QThread>
#include <QBoxLayout>
#include "EQInputRecorderWorker.h"

class EQInputRecorder : public QMainWindow
{
	Q_OBJECT

public:
	EQInputRecorder();
	~EQInputRecorder();

private slots:
	void bringWindowInFront();
	void disableButtons();
	void enableButtons();
	void save();
	void load();

private:
	QGroupBox* initOutputGroupBox();
	QGroupBox* initRecordingGroupBox();
	QGroupBox* initPlaybackGroupBox();
	QHBoxLayout* initSaveAndLoad();

	QPushButton* mRecordingButton;
	QPushButton* mPlaybackButton;
	QPushButton* mSaveButton;
	QPushButton* mLoadButton;
	EQInputRecorderWorker* mInputRecorderWorker;
	QThread mWorkerThread;
};