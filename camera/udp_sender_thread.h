#ifndef SENDER_THREAD_H
#define SENDER_THREAD_H

#include <QThread>
#include <QImage>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>

class SenderThread : public QThread
{
    Q_OBJECT

public:
    SenderThread(const QString& clientId, QObject* parent = nullptr);
    ~SenderThread();
    void enqueueImage(const QImage &image);
    void stop();

protected:
    void run() override;

private:
    QQueue<QImage> imageQueue;
    QMutex mutex;
    QWaitCondition condition;
    bool running = true;

    QString clientId;
};

#endif // SENDER_THREAD_H
