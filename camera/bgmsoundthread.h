#ifndef BGMSOUNDTHREAD_H
#define BGMSOUNDTHREAD_H

#include <QThread>
#include <cstdlib>
#include <QProcess>
#include <QTimer>
#include <QEventLoop>
#include <QProcess>

class BgmSoundThread : public QThread
{
    Q_OBJECT

public:
    BgmSoundThread(QObject *parent = nullptr);
    ~BgmSoundThread();
    void startMusic();
    void stopMusic();

protected:
    void run() override;

private:
    QProcess *playProcess;
};

#endif // BGMSOUNDTHREAD_H
