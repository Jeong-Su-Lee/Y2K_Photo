#include "bgmsoundplayer.h"


BgmSoundPlayer::BgmSoundPlayer(QObject *parent)
    : QObject(parent), soundThread(nullptr)
{
}

BgmSoundPlayer::~BgmSoundPlayer()
{
    if (soundThread) {
        soundThread->stopMusic();
        soundThread->quit();
        soundThread->wait();
        delete soundThread;
    }
}

void BgmSoundPlayer::createNewThread()
{
    if (soundThread) {
        soundThread->stopMusic();
        soundThread->quit();
        soundThread->wait();
        delete soundThread;
        soundThread = nullptr;
    }
    soundThread = new BgmSoundThread(this);
    soundThread->start();
}

void BgmSoundPlayer::startMusic()
{
    if (!soundThread || soundThread->isFinished()) {
        createNewThread();
    }
    soundThread->startMusic();
}

void BgmSoundPlayer::stopMusic()
{
    if (soundThread) {
        soundThread->stopMusic();
    }
}
