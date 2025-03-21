#ifndef CAMERASOUNDTHREAD_H
#define CAMERASOUNDTHREAD_H

#include <QThread>
#include <cstdlib>
#include <QProcess>
#include <QTimer>
#include <QEventLoop>
#include <QProcess>

class CameraSoundThread : public QThread
{
    Q_OBJECT

public:
    CameraSoundThread(QObject *parent = nullptr);
    ~CameraSoundThread();
    void startMusic();
    void stopMusic();

protected:
    void run() override;

private:
    QProcess *playProcess;
};

#endif // CAMERASOUNDTHREAD_H
