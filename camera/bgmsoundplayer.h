#ifndef BGMSOUNDPLAYER_H
#define BGMSOUNDPLAYER_H

#include <QObject>
#include <QProcess>
#include "bgmsoundthread.h"


class BgmSoundPlayer : public QObject
{
    Q_OBJECT

public:
    explicit BgmSoundPlayer(QObject *parent = nullptr);
    ~BgmSoundPlayer();
    void startMusic();
    void stopMusic();

private:
    void createNewThread();
    BgmSoundThread *soundThread;
};

#endif // BGMSOUNDPLAYER_H
