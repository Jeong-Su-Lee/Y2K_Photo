#include "camerasoundplayer.h"
#include "camerasoundthread.h"
#include <QProcess>

CameraSoundPlayer::CameraSoundPlayer(QObject *parent) : QObject(parent)
{
}

void CameraSoundPlayer::startCameraSound()
{
    CameraSoundThread *thread = new CameraSoundThread();
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);
    thread->start();
}
