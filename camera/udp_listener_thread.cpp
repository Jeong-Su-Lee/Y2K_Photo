#include "udp_listener_thread.h"
#include <QHostAddress>
#include <QDebug>
#include <QImage>

#define SERVER_IP   "192.168.10.2"
#define SERVER_PORT 25000

UDPListenerThread::UDPListenerThread(QObject *parent)
    : QThread(parent)
{
    udpSocket = new QUdpSocket(this);

    QByteArray datagram = "CONN";
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
    incomingImageBuffer.clear();

    while (running) {
        if (udpSocket->waitForReadyRead(100)) {
            while (udpSocket->hasPendingDatagrams()) {
                QByteArray datagram;
                datagram.resize(int(udpSocket->pendingDatagramSize()));
                udpSocket->readDatagram(datagram.data(), datagram.size());
                // 신호 발생
                if (datagram.startsWith("CLI")) {
                    emit clientIdReceived(QString::fromUtf8(datagram));  // 시그널로 MainWindow에 전달
                }
                else if (QString(datagram).trimmed() == "CAPT"){
                    emit captureRequested();
                }
                else if (QString(datagram).startsWith("EOFIMG"))
                {
                    // 종료 패킷 도착
                    receivingImage = false;
                    qDebug() << "UDP IMG complete ";

                    QImage image;
                    if (image.loadFromData(incomingImageBuffer, "JPG")) {
                        emit imageReceived(image); // MainWindow로 시그널 emit
                    } else {
                        qDebug() << "[UDP] 이미지 디코딩 실패";
                    }
                    incomingImageBuffer.clear();
                }
                else // if (datagram.startsWith("IMG"))
                {
                    // 시작 패킷
//                    qDebug() << "UDP IMG recieve ";
                    receivingImage = true;
                    incomingImageBuffer.append(datagram); // "IMG1" 이후가 JPG 데이터
                }

//                qDebug() << "UDP 신호 수신됨: " << datagram;


            }
        }
    }
}
