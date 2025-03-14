#include "bgmsoundthread.h"


BgmSoundThread::BgmSoundThread(QObject *parent)
    : QThread(parent), playProcess(new QProcess(this))
{
}

BgmSoundThread::~BgmSoundThread()
{
    stopMusic();
    delete playProcess;
}

void BgmSoundThread::run()
{
    exec();
}

void BgmSoundThread::startMusic()
{
    if (playProcess->state() == QProcess::Running) {
        playProcess->terminate();
        playProcess->waitForFinished();
    }
    playProcess->start("sh", QStringList() << "-c" << "./aplay -Dhw:0,0 /mnt/nfs/test_contents/test.wav");
}

void BgmSoundThread::stopMusic()
{
    if (playProcess->state() == QProcess::Running) {
        playProcess->terminate();
        playProcess->waitForFinished();
    }
}
