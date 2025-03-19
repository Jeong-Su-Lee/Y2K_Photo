#include "bgmsoundthread2.h"


BgmSoundThread2::BgmSoundThread2(QObject *parent)
    : QThread(parent), playProcess(new QProcess(this))
{
}

BgmSoundThread2::~BgmSoundThread2()
{
    stopMusic();
    delete playProcess;
}

void BgmSoundThread2::run()
{
    exec();
}

void BgmSoundThread2::startMusic()
{
    if (playProcess->state() == QProcess::Running) {
        playProcess->terminate();
        playProcess->waitForFinished();
    }
    playProcess->start("sh", QStringList() << "-c" << "aplay -Dhw:0,0 /mnt/sd/sound/ereve_bgm.wav");
}

void BgmSoundThread2::stopMusic()
{
    if (playProcess->state() == QProcess::Running) {
        playProcess->terminate();
        playProcess->waitForFinished();
    }
}
