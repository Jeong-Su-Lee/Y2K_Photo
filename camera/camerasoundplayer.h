#ifndef CAMERASOUNDPLAYER_H
#define CAMERASOUNDPLAYER_H

#include <QObject>
#include <QProcess>
#include "camerasoundthread.h"


class CameraSoundPlayer : public QObject
{
    Q_OBJECT

public:
    explicit CameraSoundPlayer(QObject *parent = nullptr);
    ~CameraSoundPlayer();
    void startMusic();
    void stopMusic();

private:
    void createNewThread();
    CameraSoundThread *soundThread;
};

#endif // CAMERASOUNDPLAYER_H
