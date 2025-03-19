#include "bgmsoundplayer2.h"

BgmSoundPlayer2 erevePlayer;

BgmSoundPlayer2::BgmSoundPlayer2(QObject *parent)
    : QObject(parent), soundThread(nullptr)
{
}

BgmSoundPlayer2::~BgmSoundPlayer2()
{
    if (soundThread) {
        soundThread->stopMusic();
        soundThread->quit();
        soundThread->wait();
        delete soundThread;
    }
}

void BgmSoundPlayer2::createNewThread()
{
    if (soundThread) {
        soundThread->stopMusic();
        soundThread->quit();
        soundThread->wait();
        delete soundThread;
        soundThread = nullptr;
    }
    soundThread = new BgmSoundThread2(this);
    soundThread->start();
}

void BgmSoundPlayer2::startMusic()
{
    if (!soundThread || soundThread->isFinished()) {
        createNewThread();
    }
    soundThread->startMusic();
}

void BgmSoundPlayer2::stopMusic()
{
    if (soundThread) {
        soundThread->stopMusic();
    }
}

