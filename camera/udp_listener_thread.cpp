#include "udp_listener_thread.h"
#include <QHostAddress>
#include <QDebug>

#define SERVER_IP   "192.168.10.2"
#define SERVER_PORT 25000

UDPListenerThread::UDPListenerThread(QObject *parent)
    : QThread(parent)
{
    udpSocket = new QUdpSocket(this);

    QByteArray datagram = "hello";
    QHostAddress targetIp("192.168.10.2");
    quint16 targetPort = 25000;

    qint64 bytes = udpSocket->writeDatagram(datagram, targetIp, targetPort);

    qDebug() << "UDP 전송됨:" << datagram << ", 바이트 수:" << bytes;

    if (bytes == -1) {
        qDebug() << "UDP 전송 에러:" << udpSocket->errorString();
    }
}

UDPListenerThread::~UDPListenerThread()
{
    running = false;
    if (udpSocket) {
        udpSocket->close();
        delete udpSocket;
    }
    wait(); // 스레드 종료 대기
}

void UDPListenerThread::run()
{
    udpSocket = new QUdpSocket();

    quint16 port = 25000; // 수신 포트 번호
    if (!udpSocket->bind(QHostAddress::Any, port)) {
        qDebug() << "UDP 포트 바인딩 실패";
        return;
    }

    qDebug() << "UDP 수신 대기 중... 포트:" << port;

    while (running) {
        if (udpSocket->waitForReadyRead(100)) {
            while (udpSocket->hasPendingDatagrams()) {
                QByteArray datagram;
                datagram.resize(int(udpSocket->pendingDatagramSize()));
                udpSocket->readDatagram(datagram.data(), datagram.size());

                qDebug() << "UDP 신호 수신됨: " << datagram;

                // 신호 발생
                emit captureRequested();
            }
        }
    }
}
