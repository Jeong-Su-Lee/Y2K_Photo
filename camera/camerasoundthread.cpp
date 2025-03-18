#include "camerasoundthread.h"


CameraSoundThread::CameraSoundThread(QObject *parent)
    : QThread(parent), playProcess(new QProcess(this))
{
}

CameraSoundThread::~CameraSoundThread()
{
    stopMusic();
    delete playProcess;
}

void CameraSoundThread::run()
{
    exec();
}

void CameraSoundThread::startMusic()
{
    if (playProcess->state() == QProcess::Running) {
        playProcess->terminate();
        playProcess->waitForFinished();
    }
    playProcess->start("sh", QStringList() << "-c" << "./aplay -Dhw:0,0 /mnt/nfs/camera_capture_1.wav");
}

void CameraSoundThread::stopMusic()
{
    if (playProcess->state() == QProcess::Running) {
        playProcess->terminate();
        playProcess->waitForFinished();
    }
}
