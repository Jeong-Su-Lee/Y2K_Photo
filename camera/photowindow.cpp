#include "photowindow.h"
#include "ui_photowindow.h"
#include "endingwindow.h"
#include "imagefilter.h"
#include <QString>
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

    // FIXME: to use server timer.
    connect(timer, SIGNAL(timeout()), this, SLOT(change_timeText()));
    timer->start(1000);

    ui->lblTime->setText(QString::number(TIME_LIMIT));


    image_buf = new uchar[640 * 480 * 3];
    camera = new CameraThread(this);
    connect(camera, SIGNAL(send_data(const uchar*,int,int)), this, SLOT(handle_data(const uchar*,int,int)));
    camera->start();

    udp_listener = new UDPListenerThread(this);
    connect(udp_listener, &UDPListenerThread::captureRequested, this, &PhotoWindow::save_current_frame);
    udp_listener->start();

    overlay_pixmap = QPixmap("/mnt/nfs/Guidline/guild_line_1.png");
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
        case 8:
            udp_listener->terminate();
            return go_to_nextWindow();
    }

    ui->lblCount->setText(QString::number(curCount+1));
}

void PhotoWindow::change_timeText()
{
    int curTime = ui->lblTime->text().toInt();
    if (curTime) {
        ui->lblTime->setText(QString::number(curTime-1));
    } else {

        ui->lblTime->setText(QString::number(TIME_LIMIT));
    }
}

void PhotoWindow::go_to_nextWindow()
{
    ImageFilter *imageFilter = new ImageFilter();
    this->hide();
    imageFilter->show();

    timer->stop();
    camera->setExitFlag();
}


PhotoWindow::~PhotoWindow()
{
    delete ui;
    delete timer;
    delete nextTimer;
}




void PhotoWindow::handle_data(const uchar *data, int width, int height)
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
    ui->lblImg1->setPixmap(pixmap);
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
    QString filename = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_zzz") + ".jpg";
    QImage image(image_buf, 640, 480, QImage::Format_RGB888);
    mCameraSoundPlayer.startMusic();
    if (image.save(filename,"JPG")) {
        qDebug() << "이미지 저장됨:" << filename;
        sendImageToServer(filename); // 여기서 전송
    } else {
        qDebug() << "이미지 저장 실패";
    }

    rise_count();
    change_timeText();
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

