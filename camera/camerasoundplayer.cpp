#include "camerasoundplayer.h"
#include "camerasoundthread.h"


CameraSoundPlayer::CameraSoundPlayer(QObject *parent)
    : QObject(parent), soundThread(nullptr)
{
}

CameraSoundPlayer::~CameraSoundPlayer()
{
    if (soundThread) {
        soundThread->stopMusic();
        soundThread->quit();
        soundThread->wait();
        delete soundThread;
    }
}

void CameraSoundPlayer::createNewThread()
{
    if (soundThread) {
        soundThread->stopMusic();
        soundThread->quit();
        soundThread->wait();
        delete soundThread;
        soundThread = nullptr;
    }
    soundThread = new CameraSoundThread(this);
    soundThread->start();
}

void CameraSoundPlayer::startMusic()
{
    if (!soundThread || soundThread->isFinished()) {
        createNewThread();
    }
    soundThread->startMusic();
}

void CameraSoundPlayer::stopMusic()
{
    if (soundThread) {
        soundThread->stopMusic();
    }
}
