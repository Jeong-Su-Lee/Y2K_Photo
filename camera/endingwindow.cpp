#include "endingwindow.h"
#include "ui_endingwindow.h"
#include "firstwindow.h"
#include <QString>
#include <string>
#include <QPixmap>
#include <QFile>
#include <QUdpSocket>
#include <QBuffer>
#include <QImage>
#include <QProcess>
#include <QApplication>

EndingWindow::EndingWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EndingWindow),
    timer(new QTimer),
    nextTimer(new QTimer)
{
    ui->setupUi(this);

    QImage image;
    if (!image.load("/mnt/sd/decorated_image.jpg")) {
        qWarning("Failed to load image!");
    }
    image = image.scaled(430, 329, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->lblImg->setPixmap(QPixmap::fromImage(image));


    connect(timer, SIGNAL(timeout()), this, SLOT(change_timeText()));
    timer->start(1000);

    connect(nextTimer, SIGNAL(timeout()), this, SLOT(go_to_firstWindow()));
    nextTimer->start(1000*59 + 10);

    yFreeStyle.startMusic();
}

EndingWindow::~EndingWindow()
{
    delete ui;
    delete timer;
    delete nextTimer;
}


void EndingWindow::on_btnEnding_clicked()
{
    qDebug() << "클릭 첫 회";
    ui->btnEnding->setEnabled(false);
    yFreeStyle.stopMusic();

    QHostAddress serverAddress("192.168.10.2"); // 서버 IP
    quint16 serverPort = 25000;

    QByteArray imageData;
    QBuffer buffer(&imageData);
    QImage image;
    QUdpSocket udpSocket;
    if (!image.load("/mnt/sd/decorated_image.jpg")){
        qDebug() << "fail to load image";
    }
    int offset = 0;
    const int chunkSize = 1024;
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "JPG");
    buffer.close();
    while (offset < imageData.size())
    {
        QByteArray chunk = imageData.mid(offset, chunkSize);

        // 클라이언트 ID + 실제 데이터
        QByteArray packet;
        packet.append("DECO"); // ex: "CAP1"
        packet.append(chunk);             // 이미지 일부

        udpSocket.writeDatagram(packet, serverAddress, serverPort);
        offset += chunkSize;
    }

    udpSocket.writeDatagram("EOFDECO", serverAddress, serverPort);

    // FirstWindow *firstWindow = new FirstWindow();
    // firstWindow->show();

    timer->stop();
    nextTimer->stop();
    QProcess::startDetached("sh", QStringList() << "-c" << "camera");
    QApplication::quit();
}


void EndingWindow::go_to_firstWindow()
{
    this->on_btnEnding_clicked();
}

void EndingWindow::change_timeText()
{
    QString curText = ui->btnEnding->text().split(' ')[1];

    int nextTime = curText.toInt()-1;
    ui->btnEnding->setText(QString("Exit... %1").arg(nextTime));

}
