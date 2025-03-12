#ifndef CAMERASOUNDTHREAD_H
#define CAMERASOUNDTHREAD_H

#include <QThread>
#include <cstdlib>
#include <QProcess>
#include <QTimer>
#include <QEventLoop>

class CameraSoundThread : public QThread
{
    Q_OBJECT

public:
    CameraSoundThread(QObject *parent = nullptr);
    ~CameraSoundThread();

protected:
    void run() override;
};

#endif // CAMERASOUNDTHREAD_H
