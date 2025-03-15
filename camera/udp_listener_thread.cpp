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
                // Client 번호 받음
                if (datagram.startsWith("CLI")) {
                    emit clientIdReceived(QString::fromUtf8(datagram));  // 시그널로 MainWindow에 전달
                }
                // Capture 신호 받아서 사진 저장
                else if (QString(datagram).trimmed() == "CAPT"){
                    emit captureRequested();
                }
                // 남은 시간 받는 부분
                else if (datagram.startsWith("CNT")) {
                    // 시간 받아서 화면에 띄워주는 함수로 분기 필요
                    continue;
                }
                // 마지막이므로 카메라 끄고, 이미지 받을 준비함
                else if (QString(datagram).trimmed() == "FINAL"){
                    continue;
                }
                // 이미지 스트림 마지막 부분 받아서 화면에 띄워줌
                else if (QString(datagram).startsWith("EOFIMG"))
                {
                    // 종료 패킷 도착

//                    qDebug() << "UDP IMG complete ";

                    QImage image;

                    if (image.loadFromData(incomingImageBuffer, "JPG")) {
                        emit imageReceived(image); // MainWindow로 시그널 emit
                    } else {
//                        qDebug() << "[UDP] 이미지 디코딩 실패";
                    }
                    receivingImage = false;
                    incomingImageBuffer.clear();
                }
                // 헤더 정보 없을 시, 이미지로 판단해서, 이미지 버퍼에 저장함.
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
