#ifndef UDP_LISTENER_THREAD_H
#define UDP_LISTENER_THREAD_H

#include <QThread>
#include <QUdpSocket>
#include <QHostAddress>
#include <QByteArray>

class UDPListenerThread : public QThread
{
    Q_OBJECT
public:
    explicit UDPListenerThread(QObject *parent = nullptr);
    ~UDPListenerThread();

signals:
    void captureRequested();  // UDP 패킷 수신 시 MainWindow에 신호 보내기
    void clientIdReceived(const QString& id);
    void imageReceived(const QImage &image);

protected:
    void run() override;

private:
    QUdpSocket *udpSocket;
    bool running;
    QByteArray incomingImageBuffer;
    bool receivingImage = false;
};

#endif // UDP_LISTENER_THREAD_H
