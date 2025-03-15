 #include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QUdpSocket>
#include <QPainter>
#include <QBuffer>
#include <QString>

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

    connect(udp_listener, &UDPListenerThread::imageReceived, this, &MainWindow::displayReceivedImage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::displayReceivedImage(const QImage &image) {
    QPixmap pixmap = QPixmap::fromImage(image);
//    qDebug() << "UDP IMG Display ";
    if (myclientId == "CLI1")
    {
        ui->lblImg2->setPixmap(pixmap);
    }
    else if (myclientId == "CLI2")
    {
        ui->lblImg->setPixmap(pixmap);
    }
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
    QString guidename = "heart"; // 가이드 뭐인지 받는 부분 필요
    if (myclientId == "CLI1"){
        number_of_guide = 1;
    }

    QString guidenum = QString::number(number_of_guide); // 몇번 클라이언트인지에 따라 count 수 다르게 할 필요 있음
    QString filename = "/mnt/nfs/guide/guide_" + guidename + "/" + guidenum + "_" +  guidename + ".png";
    overlay_pixmap = QPixmap(filename);
    QImage image_overlay = overlay_pixmap.toImage().convertToFormat(QImage::Format_ARGB32);

    // 2. 알파 조절
    for (int y = 0; y < image_overlay.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(image_overlay.scanLine(y));
        for (int x = 0; x < image_overlay.width(); ++x) {
            QColor color = QColor::fromRgba(line[x]);
            color.setAlphaF(color.alphaF() * 0.5); // 기존 알파에 비례
            line[x] = color.rgba();
        }
    }

    // 3. 다시 QPixmap으로 변환
    QPixmap overlay_pixmap = QPixmap::fromImage(image_overlay);
    if (myclientId == "CLI1"){
        ui->guideimg->setPixmap(overlay_pixmap);
    }
    else if(myclientId == "CLI2"){
        ui->guideimg2->setPixmap(overlay_pixmap);
    }

}

void MainWindow::handle_data(const uchar *data, int width, int height)
{
    //qDebug() << "handle_data" << data << width << height;
    yuyv2rgb(data, width, height, image_buf);
    QImage image(image_buf, width, height, QImage::Format_RGB888);
    QPixmap pixmap = QPixmap::fromImage(image);
    if (myclientId == "CLI1")
    {
        ui->lblImg->setPixmap(pixmap);
        senderThread->enqueueImage(QImage(image_buf, width, height, QImage::Format_RGB888));
    }
    else if (myclientId == "CLI2")
    {
        ui->lblImg2->setPixmap(pixmap);
        senderThread->enqueueImage(QImage(image_buf, width, height, QImage::Format_RGB888));
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
    QString prefix_eof = "EOFCAP0";
    if (myclientId == "CLI1")
    {
        prefix = "CAP1";
        prefix_eof = "EOFCAP1";
    }
    else if (myclientId == "CLI2")
    {
        prefix = "CAP2";
        prefix_eof = "EOFCAP2";
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

        udpSocket.writeDatagram(prefix_eof.toUtf8(), serverAddress, serverPort);
        // qDebug() << "이미지 전송 완료";
    }
    QString guidename = "heart"; // 가이드 뭐인지 받는 부분 필요
    number_of_guide += 2;
    QString guidenum = QString::number(number_of_guide); // 몇번 클라이언트인지에 따라 count 수 다르게 할 필요 있음
    QString filename = "/mnt/nfs/guide/guide_" + guidename + "/" + guidenum + "_" +  guidename + ".png";
    overlay_pixmap = QPixmap(filename);
    QImage image_overlay = overlay_pixmap.toImage().convertToFormat(QImage::Format_ARGB32);

    // 2. 알파 조절
    for (int y = 0; y < image_overlay.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(image_overlay.scanLine(y));
        for (int x = 0; x < image_overlay.width(); ++x) {
            QColor color = QColor::fromRgba(line[x]);
            color.setAlphaF(color.alphaF() * 0.5); // 기존 알파에 비례
            line[x] = color.rgba();
        }
    }

    // 3. 다시 QPixmap으로 변환
    QPixmap overlay_pixmap = QPixmap::fromImage(image_overlay);
    if (myclientId == "CLI1"){
        ui->guideimg->setPixmap(overlay_pixmap);
    }
    else if(myclientId == "CLI2"){
        ui->guideimg2->setPixmap(overlay_pixmap);
    }
}

