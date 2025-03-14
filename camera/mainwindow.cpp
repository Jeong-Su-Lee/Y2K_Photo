 #include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QUdpSocket>
#include <QPainter>
#include <QBuffer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    image_buf = new uchar[640 * 480 * 3];
    camera = new CameraThread(this);
    connect(camera, SIGNAL(send_data(const uchar*,int,int)), this, SLOT(handle_data(const uchar*,int,int)));
    camera->start();

    udp_listener = new UDPListenerThread(this);
    connect(udp_listener, &UDPListenerThread::captureRequested, this, &MainWindow::save_current_frame);
    connect(udp_listener, &UDPListenerThread::clientIdReceived, this, &MainWindow::onClientIdReceived);

    udp_listener->start();



    overlay_pixmap = QPixmap("/mnt/nfs/Guidline/guild_line_1.png");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (senderThread) {
        senderThread->stop();
        senderThread->wait();  // 스레드가 안전하게 종료되도록 대기
        delete senderThread;
        senderThread = nullptr;
    }

    QMainWindow::closeEvent(event);  // 기본 종료 이벤트 호출
}

void MainWindow::onClientIdReceived(const QString& id) {
    myclientId = id;
    qDebug() << "클라이언트 ID 할당됨:" << myclientId;
    senderThread = new SenderThread(myclientId, this);
    senderThread->start();
}

void MainWindow::handle_data(const uchar *data, int width, int height)
{
    //qDebug() << "handle_data" << data << width << height;
    yuyv2rgb(data, width, height, image_buf);
    QPixmap pixmap = QPixmap::fromImage(QImage(image_buf, width, height, QImage::Format_RGB888));
    if(!overlay_pixmap.isNull()){
        QPainter painter(&pixmap);
        painter.setOpacity(0.5); // 0.0 ~1.0
        painter.drawPixmap(0,0,overlay_pixmap); //draw guide line over frame
        painter.end();
    }
    if (myclientId == "CLI1")
    {
        ui->lblImg->setPixmap(pixmap);
    }
    else if (myclientId == "CLI2")
    {
        ui->lblImg2->setPixmap(pixmap);
    }

}

bool MainWindow::yuyv2rgb(const uchar *yuyv, int width, int height, uchar *rgb)
{
    long yuv_size = height * width * 2;
    long rgb_size = height * width * 3;

    if (yuyv == NULL || rgb == NULL)
    return false;

    for (int i = 0, j = 0; i < rgb_size && j < yuv_size; i += 6, j += 4)
    {
    yuyv_to_rgb_pixel(&yuyv[j], &rgb[i]);
    }
    return true;
}

void MainWindow::yuyv_to_rgb_pixel(const uchar *yuyv, uchar *rgb)
{
    int y, v, u;
    float r, g, b;

    y = yuyv[0]; //y0
    u = yuyv[1]; //u0
    v = yuyv[3]; //v0

    r = y + 1.4065 * (v - 128);			     //r0
    g = y - 0.3455 * (u - 128) - 0.7169 * (v - 128); //g0
    b = y + 1.1790 * (u - 128);			     //b0

    if (r < 0)
    r = 0;
    else if (r > 255)
    r = 255;
    if (g < 0)
    g = 0;
    else if (g > 255)
    g = 255;
    if (b < 0)
    b = 0;
    else if (b > 255)
    b = 255;

    rgb[0] = (unsigned char)r;
    rgb[1] = (unsigned char)g;
    rgb[2] = (unsigned char)b;

    //second pixel
    u = yuyv[1]; //u0
    y = yuyv[2]; //y1
    v = yuyv[3]; //v0

    r = y + 1.4065 * (v - 128);			     //r1
    g = y - 0.3455 * (u - 128) - 0.7169 * (v - 128); //g1
    b = y + 1.1790 * (u - 128);			     //b1

    if (r < 0)
    r = 0;
    else if (r > 255)
    r = 255;
    if (g < 0)
    g = 0;
    else if (g > 255)
    g = 255;
    if (b < 0)
    b = 0;
    else if (b > 255)
    b = 255;

    rgb[3] = (unsigned char)r;
    rgb[4] = (unsigned char)g;
    rgb[5] = (unsigned char)b;
}


void MainWindow::save_current_frame()
{
    QImage image(image_buf, 640, 480, QImage::Format_RGB888);
    mCameraSoundPlayer.startMusic();
    // senderThread->enqueueImage(image);
    QUdpSocket udpSocket;
    QHostAddress serverAddress("192.168.10.2");
    quint16 serverPort = 25000;
    QString prefix = "CAP0";
    if (myclientId == "CLI1")
    {
        prefix = "CAP1";
    }
    else if (myclientId == "CLI2")
    {
        prefix = "CAP2";
    }
    if (!image.isNull())
    {
        QByteArray imageData;
        QBuffer buffer(&imageData);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "JPG");

        int offset = 0;
        const int chunkSize = 1024;
        while (offset < imageData.size())
        {
            QByteArray chunk = imageData.mid(offset, chunkSize);

            // 클라이언트 ID + 실제 데이터
            QByteArray packet;
            packet.append(prefix.toUtf8()); // ex: "CAP1"
            packet.append(chunk);             // 이미지 일부

            udpSocket.writeDatagram(packet, serverAddress, serverPort);
            offset += chunkSize;
        }

        udpSocket.writeDatagram("EOF", serverAddress, serverPort);
        qDebug() << "이미지 전송 완료";
    }
}

//void MainWindow::sendImageToServer(const QString& filePath)
//{
//    QFile file(filePath);
//    if (!file.open(QIODevice::ReadOnly)) {
//        qDebug() << "파일 열기 실패:" << filePath;
//        return;
//    }

//    QUdpSocket udpSender;
//    QByteArray buffer;
//    QHostAddress serverAddress("192.168.10.2"); // 서버 IP
//    quint16 serverPort = 25000;

//    while (!file.atEnd()) {
//        buffer = file.read(1024); // 1024 바이트씩 전송
//        udpSender.writeDatagram(buffer, serverAddress, serverPort);
//    }
//    udpSender.writeDatagram("EOF", serverAddress, serverPort);
//    file.close();
//    qDebug() << "이미지 전송 완료.";
//}

