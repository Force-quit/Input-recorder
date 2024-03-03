#include "../Headers/EQInputRecorder.h"
#include <QBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QIcon>
#include <QPushButton>
#include <QCheckBox>
#include "../Headers/EQInputRecorderWorker.h"

EQInputRecorder::EQInputRecorder()
	: QMainWindow(),
	mRecordingButton{},
	mPlaybackButton{},
	mSaveButton{},
	mLoadButton{},
	mInputRecorderWorker{ new EQInputRecorderWorker },
	mWorkerThread()
{
	QWidget* wCentralWidget{ new QWidget };
	QVBoxLayout* wCentralLayout{ new QVBoxLayout };
	wCentralWidget->setLayout(wCentralLayout);

	wCentralLayout->addWidget(initOutputGroupBox());

	QHBoxLayout* wRecordAndPlaybackLayout{ new QHBoxLayout };
	wRecordAndPlaybackLayout->addWidget(initRecordingGroupBox());
	wRecordAndPlaybackLayout->addWidget(initPlaybackGroupBox());
	wCentralLayout->addLayout(wRecordAndPlaybackLayout);

	wCentralLayout->addLayout(initSaveAndLoad());

	connect(&mWorkerThread, &QThread::finished, mInputRecorderWorker, &QObject::deleteLater);
	connect(mInputRecorderWorker, &EQInputRecorderWorker::finishedRecording, this, &EQInputRecorder::bringWindowInFront);
	connect(mInputRecorderWorker, &EQInputRecorderWorker::finishedPlayback, this, &EQInputRecorder::bringWindowInFront);

	mInputRecorderWorker->moveToThread(&mWorkerThread);
	mWorkerThread.start();

	setWindowIcon(QIcon(":/images/writing.png"));
	setCentralWidget(wCentralWidget);
}

void EQInputRecorder::disableButtons()
{
	mPlaybackButton->setEnabled(false);
	mRecordingButton->setEnabled(false);
	mSaveButton->setEnabled(false);
	mLoadButton->setEnabled(false);
}

void EQInputRecorder::enableButtons()
{
	mPlaybackButton->setEnabled(true);
	mRecordingButton->setEnabled(true);
	mSaveButton->setEnabled(true);
	mLoadButton->setEnabled(true);
}

void EQInputRecorder::save()
{
	//if (mInputRecorderWorker->)
}

void EQInputRecorder::load()
{
}

QGroupBox* EQInputRecorder::initOutputGroupBox()
{
	QGroupBox* outputGroupBox{ new QGroupBox("Output") };
	QHBoxLayout* groupBoxLayout{ new QHBoxLayout };
	groupBoxLayout->setAlignment(Qt::AlignLeft);

	QLabel* outputLabel{ new QLabel("Current status :") };
	QLabel* ouputText{ new QLabel("Inactive") };
	connect(mInputRecorderWorker, &EQInputRecorderWorker::displayText, ouputText, &QLabel::setText);

	groupBoxLayout->addWidget(outputLabel);
	groupBoxLayout->addWidget(ouputText);
	outputGroupBox->setLayout(groupBoxLayout);
	return outputGroupBox;
}

QGroupBox* EQInputRecorder::initRecordingGroupBox()
{
	QGroupBox* recordingGroupBox{ new QGroupBox("Record") };
	QVBoxLayout* groupBoxLayout{ new QVBoxLayout };
	groupBoxLayout->setAlignment(Qt::AlignCenter);

	mRecordingButton = new QPushButton("Start recording");
	QLabel* recordingShortcutLabel{ new QLabel("Stop recording : ESC") };

	groupBoxLayout->addWidget(mRecordingButton);
	groupBoxLayout->addWidget(recordingShortcutLabel);
	recordingGroupBox->setLayout(groupBoxLayout);

	connect(mRecordingButton, &QPushButton::clicked, mInputRecorderWorker, &EQInputRecorderWorker::startRecording);
	connect(mRecordingButton, &QPushButton::clicked, this, &EQInputRecorder::disableButtons);
	connect(mInputRecorderWorker, &EQInputRecorderWorker::finishedRecording, this, &EQInputRecorder::enableButtons);

	return recordingGroupBox;
}

QGroupBox* EQInputRecorder::initPlaybackGroupBox()
{
	QGroupBox* currentRecordingGroupBox{ new QGroupBox("Playback") };
	QVBoxLayout* groupBoxLayout{ new QVBoxLayout };
	groupBoxLayout->setAlignment(Qt::AlignCenter);

	mPlaybackButton = new QPushButton("Start playback");
	QLabel* playbackShortcutLabel{ new QLabel("Stop playback : ESC") };
	QCheckBox* loopCheckbox{ new QCheckBox("Looping") };

	groupBoxLayout->addWidget(mPlaybackButton);
	groupBoxLayout->addWidget(loopCheckbox);
	groupBoxLayout->addWidget(playbackShortcutLabel);
	currentRecordingGroupBox->setLayout(groupBoxLayout);

	connect(mPlaybackButton, &QPushButton::clicked, mInputRecorderWorker, &EQInputRecorderWorker::startPlayback);
	connect(loopCheckbox, &QCheckBox::stateChanged, [&](int state) {
		mInputRecorderWorker->setPlaybackLoop(state);
		});
	connect(mPlaybackButton, &QPushButton::clicked, this, &EQInputRecorder::disableButtons);
	connect(mInputRecorderWorker, &EQInputRecorderWorker::finishedPlayback, this, &EQInputRecorder::enableButtons);


	return currentRecordingGroupBox;
}

QHBoxLayout* EQInputRecorder::initSaveAndLoad()
{
	QHBoxLayout* saveAndLoadLayout{ new QHBoxLayout };

	mSaveButton = new QPushButton("Save session");
	saveAndLoadLayout->addWidget(mSaveButton);

	mLoadButton = new QPushButton("Load session");
	saveAndLoadLayout->addWidget(mLoadButton);

	connect(mSaveButton, &QPushButton::clicked, this, &EQInputRecorder::save);
	connect(mLoadButton, &QPushButton::clicked, this, &EQInputRecorder::load);

	return saveAndLoadLayout;
}

void EQInputRecorder::bringWindowInFront()
{
	activateWindow();
}

EQInputRecorder::~EQInputRecorder()
{
	mWorkerThread.requestInterruption();
	mWorkerThread.quit();
	mWorkerThread.wait();
}