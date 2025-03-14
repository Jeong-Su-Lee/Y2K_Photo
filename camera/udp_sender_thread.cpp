
#include "udp_sender_thread.h"
#include <QUdpSocket>
#include <QBuffer>
#include <QDebug>

SenderThread::SenderThread(const QString& clientId, QObject* parent)
    : QThread(parent), running(true), clientId(clientId)  // 전달받은 clientId 저장
{
}
SenderThread::~SenderThread()
{
    // 필요하다면 여기서 자원 정리
}
void SenderThread::enqueueImage(const QImage &image)
{
    QMutexLocker locker(&mutex);
    imageQueue.enqueue(image);
    condition.wakeOne();
}

void SenderThread::stop()
{
    QMutexLocker locker(&mutex);
    running = false;
    condition.wakeOne();
}

void SenderThread::run()
{
    QUdpSocket udpSocket;
    QHostAddress serverAddress("192.168.10.2");
    quint16 serverPort = 25000;
    QString prefix = "CAP0";
    if (clientId == "CLI1")
    {
        prefix = "CAP1";
    }
    else if (clientId == "CLI2")
    {
        prefix = "CAP2";
    }

    while (true) {
        QImage image;
        {
            QMutexLocker locker(&mutex);
            if (imageQueue.isEmpty()) {
                if (!running) break;
                condition.wait(&mutex);
            }

            if (!imageQueue.isEmpty())
                image = imageQueue.dequeue();
        }

        if (!image.isNull()) {
            QByteArray imageData;
            QBuffer buffer(&imageData);
            buffer.open(QIODevice::WriteOnly);
            image.save(&buffer, "JPG");

            int offset = 0;
            const int chunkSize = 1024;
            while (offset < imageData.size()) {
                QByteArray chunk = imageData.mid(offset, chunkSize);

                // 클라이언트 ID + 실제 데이터
                QByteArray packet;
                packet.append(prefix.toUtf8()); // ex: "CAP1"
                packet.append(chunk);             // 이미지 일부

                udpSocket.writeDatagram(packet, serverAddress, serverPort);
                offset += chunkSize;
            }

            udpSocket.writeDatagram("EOF", serverAddress, serverPort);
            qDebug() << "[SenderThread] 이미지 전송 완료";
        }
    }

    qDebug() << "[SenderThread] 스레드 종료";
}
