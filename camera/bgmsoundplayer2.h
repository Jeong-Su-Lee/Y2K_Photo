#ifndef BGMSOUNDPLAYER2_H
#define BGMSOUNDPLAYER2_H


#include <QObject>
#include <QProcess>
#include "bgmsoundthread2.h"


class BgmSoundPlayer2 : public QObject
{
    Q_OBJECT

public:
    explicit BgmSoundPlayer2(QObject *parent = nullptr);
    ~BgmSoundPlayer2();
    void startMusic();
    void stopMusic();

private:
    void createNewThread();
    BgmSoundThread2 *soundThread;
};

extern BgmSoundPlayer2 erevePlayer;

#endif // BGMSOUNDPLAYER2_H
