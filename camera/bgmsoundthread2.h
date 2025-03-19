#ifndef BGMSOUNDTHREAD2_H
#define BGMSOUNDTHREAD2_H


#include <QThread>
#include <cstdlib>
#include <QProcess>
#include <QTimer>
#include <QEventLoop>
#include <QProcess>

class BgmSoundThread2 : public QThread
{
    Q_OBJECT

public:
    BgmSoundThread2(QObject *parent = nullptr);
    ~BgmSoundThread2();
    void startMusic();
    void stopMusic();

protected:
    void run() override;

private:
    QProcess *playProcess;
};

#endif // BGMSOUNDTHREAD2_H
