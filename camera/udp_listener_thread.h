#ifndef UDP_LISTENER_THREAD_H
#define UDP_LISTENER_THREAD_H

#include <QThread>
#include <QUdpSocket>
#include <QHostAddress>

class UDPListenerThread : public QThread
{
    Q_OBJECT
public:
    explicit UDPListenerThread(QObject *parent = nullptr);
    ~UDPListenerThread();

signals:
    void captureRequested();  // UDP 패킷 수신 시 MainWindow에 신호 보내기
    void clientIdReceived(const QString& id);

protected:
    void run() override;

private:
    QUdpSocket *udpSocket;
    bool running;
};

#endif // UDP_LISTENER_THREAD_H
