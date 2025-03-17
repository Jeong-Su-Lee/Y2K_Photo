#include "photowindow.h"
#include "ui_photowindow.h"
#include "endingwindow.h"
#include "imagefilter.h"
#include <QString>
#include <QBuffer>
#include <QFile>
#include <QUdpSocket>
#include <QPainter>

PhotoWindow::PhotoWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PhotoWindow),
    timer(new QTimer),
    nextTimer(new QTimer)
{
    ui->setupUi(this);

    ui->lblTime->setText(QString::number(TIME_LIMIT));


    image_buf = new uchar[640 * 480 * 3];
    camera = new CameraThread(this);
    connect(camera, SIGNAL(send_data(const uchar*,int,int)), this, SLOT(handle_data(const uchar*,int,int)));
    camera->start();

    udp_listener = new UDPListenerThread(this);
    QUdpSocket udpSocket;

    QByteArray datagram = "CONN";
    QHostAddress targetIp("192.168.10.2");
    quint16 targetPort = 25000;

    qint64 bytes = udpSocket.writeDatagram(datagram, targetIp, targetPort);

    qDebug() << "UDP 전송됨:" << datagram << ", 바이트 수:" << bytes;

    if (bytes == -1) {
        qDebug() << "UDP 전송 에러:" << udpSocket.errorString();
    }
    connect(udp_listener, &UDPListenerThread::captureRequested, this, &PhotoWindow::save_current_frame);
    connect(udp_listener, &UDPListenerThread::clientIdReceived, this, &PhotoWindow::onClientIdReceived);
    connect(udp_listener, &UDPListenerThread::timeCountReceived, this, &PhotoWindow::change_timeText);
    connect(udp_listener, &UDPListenerThread::GuideReceived, this, &PhotoWindow::setGuideFromChar);



    connect(udp_listener, &UDPListenerThread::imageReceived, this, &PhotoWindow::displayReceivedImage);
    connect(udp_listener, &UDPListenerThread::finalImageReceived, this, &PhotoWindow::go_to_nextWindow);

    udp_listener->start();

    imageThread = new QThread(this);
    imageWorker = new ImageProcessorWorker();

    imageWorker->moveToThread(imageThread);
    connect(imageThread, &QThread::finished, imageWorker, &QObject::deleteLater);

   // 프레임 처리 요청
    connect(this, &PhotoWindow::requestFrameProcessing, imageWorker, &ImageProcessorWorker::processFrame);

   // 처리 완료 수신
    connect(imageWorker, &ImageProcessorWorker::frameProcessed, this, &PhotoWindow::onFrameProcessed);

    imageThread->start();
}
void PhotoWindow::onFrameProcessed(const QPixmap &pixmap)
 {
    if (myclientId == "CLI1") {
        ui->lblImg->setPixmap(pixmap);
    } else if (myclientId == "CLI2") {
        ui->lblImg2->setPixmap(pixmap);
    }
 }

void PhotoWindow::toggle_grid(QLabel *lbl){
    QString styleSheet = lbl->styleSheet();
    bool isBackgroundExist = styleSheet.split('\n').last().split(':').first() == "background";

    if (isBackgroundExist) {
        // remove background color
        int lastNewlineIndex = styleSheet.lastIndexOf('\n');

        if (lastNewlineIndex != -1) {
            lbl->setStyleSheet(styleSheet.left(lastNewlineIndex));
        }
    } else {
        lbl->setStyleSheet(styleSheet + "\nbackground: #AEE1EF;");
    }
}

void PhotoWindow::rise_count(){
    int curCount = ui->lblCount->text().toInt();

    switch (curCount) {
        case 1:
            toggle_grid(ui->lbl1);
            toggle_grid(ui->lbl2);
            toggle_grid(ui->lbl3);
            toggle_grid(ui->lbl4);
            break;
        case 2:
            toggle_grid(ui->lbl3);
            toggle_grid(ui->lbl4);
            toggle_grid(ui->lbl5);
            toggle_grid(ui->lbl6);
            break;
        case 3:
            toggle_grid(ui->lbl5);
            toggle_grid(ui->lbl6);
            toggle_grid(ui->lbl7);
            toggle_grid(ui->lbl8);
            break;
        case 4:
            toggle_grid(ui->lbl7);
            toggle_grid(ui->lbl8);
            toggle_grid(ui->lbl9);
            toggle_grid(ui->lbl10);
            break;
        case 5:
            toggle_grid(ui->lbl9);
            toggle_grid(ui->lbl10);
            toggle_grid(ui->lbl11);
            toggle_grid(ui->lbl12);
            break;
        case 6:
            toggle_grid(ui->lbl11);
            toggle_grid(ui->lbl12);
            toggle_grid(ui->lbl13);
            toggle_grid(ui->lbl14);
            break;
        case 7:
            toggle_grid(ui->lbl13);
            toggle_grid(ui->lbl14);
            toggle_grid(ui->lbl15);
            toggle_grid(ui->lbl16);
            break;
        default:
            return;
    }

    ui->lblCount->setText(QString::number(curCount+1));
}

void PhotoWindow::change_timeText(const int timeCount)
{
    ui->lblTime->setText(QString::number(timeCount-1));
}

void PhotoWindow::go_to_nextWindow()
{

    ImageFilter *imageFilter = new ImageFilter();
    this->hide();
    imageFilter->show();

    timer->stop();
    camera->setExitFlag();
    udp_listener->terminate();
}


PhotoWindow::~PhotoWindow()
{
    delete ui;
    delete timer;
    delete nextTimer;
}


void PhotoWindow::displayReceivedImage(const QImage &image) {
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

void PhotoWindow::closeEvent(QCloseEvent *event)
{
   if (senderThread) {
       senderThread->stop();
       senderThread->wait();  // 스레드가 안전하게 종료되도록 대기
       delete senderThread;
       senderThread = nullptr;
   }

   QWidget::closeEvent(event);  // 기본 종료 이벤트 호출
}

void PhotoWindow::onClientIdReceived(const QString& id) {
   myclientId = id;
   qDebug() << "클라이언트 ID 할당됨:" << myclientId;
   senderThread = new SenderThread(myclientId, this);
   senderThread->start();
//    QString guidename = "heart"; // 가이드 뭐인지 받는 부분 필요
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


void PhotoWindow::handle_data(const uchar *data, int width, int height)
{
   //qDebug() << "handle_data" << data << width << height;
   yuyv2rgb(data, width, height, image_buf);
   QImage image(image_buf, width, height, QImage::Format_RGB888);
   QPixmap pixmap = QPixmap::fromImage(image);
   if (myclientId == "CLI1")
   {
    //    ui->lblImg->setPixmap(pixmap);
       senderThread->enqueueImage(image);
   }
   else if (myclientId == "CLI2")
   {
    //    ui->lblImg2->setPixmap(pixmap);
       senderThread->enqueueImage(image);
   }
   emit requestFrameProcessing(QByteArray(reinterpret_cast<const char*>(data), width * height * 2), width, height, guidename, myclientId);

}


bool PhotoWindow::yuyv2rgb(const uchar *yuyv, int width, int height, uchar *rgb)
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

void PhotoWindow::yuyv_to_rgb_pixel(const uchar *yuyv, uchar *rgb)
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

void PhotoWindow::save_current_frame()
{
   QImage image(image_buf, 320, 240, QImage::Format_RGB888);
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
       image = image.mirrored(true, false);
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
//    QString guidename = "heart"; // 가이드 뭐인지 받는 부분 필요
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

    rise_count();
    ui->lblTime->setText(QString::number(TIME_LIMIT));
}

void PhotoWindow::sendImageToServer(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "파일 열기 실패:" << filePath;
        return;
    }

    QUdpSocket udpSender;
    QByteArray buffer;
    QHostAddress serverAddress("192.168.10.2"); // 서버 IP
    quint16 serverPort = 25000;

    while (!file.atEnd()) {
        buffer = file.read(1024); // 1024 바이트씩 전송
        udpSender.writeDatagram(buffer, serverAddress, serverPort);
    }
    udpSender.writeDatagram("EOF", serverAddress, serverPort);
    file.close();
    qDebug() << "이미지 전송 완료.";
}

void PhotoWindow::setGuideFromChar(QChar guideChar)
{
    if(guideChar == "H"){
        guidename = "heart";
    }
    else if(guideChar == "L"){
        guidename = "lg_logo";
    }
    else if(guideChar == "S"){
        guidename = "star";
    }
    else{
        guidename ="none";
    }
    qDebug() << "[MainWindow] 가이드 이미지 이름 설정됨:" << guidename;
}
