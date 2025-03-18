#include "guideselectwindow.h"
#include "ui_guideselectwindow.h"
#include "photowindow.h"
#include "imagefilter2.h"
#include <QUdpSocket>

GuideSelectWindow::GuideSelectWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GuideSelectWindow),
    timer(new QTimer())
{
    ui->setupUi(this);

    udp_guide = new UDPListenerThread(this);
    connect(udp_guide, &UDPListenerThread::guidecomplete, this, &GuideSelectWindow::go_to_nextWindow);
    QUdpSocket udpSocket;

    QByteArray datagram = "SELG";
    QHostAddress targetIp("192.168.10.2");
    quint16 targetPort = 25000;

    udpSocket.writeDatagram(datagram, targetIp, targetPort);
    udp_guide->start();

    // connect(timer, SIGNAL(timeout()), this, SLOT(go_to_nextWindow()));
    // timer->start(1000*4);
}

void GuideSelectWindow::go_to_nextWindow()
{
    if(udp_guide)
    {
        udp_guide->quit();
        udp_guide->wait();
        delete udp_guide;
    }
    PhotoWindow *photoWindow = new PhotoWindow();
    qDebug() << "go to next";
    this->hide();
    photoWindow->show();

    
    // timer->stop();
}

GuideSelectWindow::~GuideSelectWindow()
{
    delete ui;
    delete timer;
}


void GuideSelectWindow::on_btnNone_clicked()
{
    // handle on clicked "None" guide
    QUdpSocket udpSender;
    QHostAddress serverAddress("192.168.10.2"); // 서버 IP
    quint16 serverPort = 25000;
    qDebug() << "NONE";


    udpSender.writeDatagram("GUIDEN", serverAddress, serverPort);
}

void GuideSelectWindow::on_btnHeart_clicked()
{
    // handle on clicked "Heart" guide
    QUdpSocket udpSender;
    QHostAddress serverAddress("192.168.10.2"); // 서버 IP
    quint16 serverPort = 25000;
    qDebug() << "HEART";


    udpSender.writeDatagram("GUIDEH", serverAddress, serverPort);
}

void GuideSelectWindow::on_btnStar_clicked()
{
    // handle on clicked "Star" guide
    QUdpSocket udpSender;
    QHostAddress serverAddress("192.168.10.2"); // 서버 IP
    quint16 serverPort = 25000;
    qDebug() << "STAR";


    udpSender.writeDatagram("GUIDES", serverAddress, serverPort);
}

void GuideSelectWindow::on_btnLG_clicked()
{
    // handle on clicked "LG logo" guide
    QUdpSocket udpSender;
    QHostAddress serverAddress("192.168.10.2"); // 서버 IP
    quint16 serverPort = 25000;
    qDebug() << "LG_LOGO";

    udpSender.writeDatagram("GUIDEL", serverAddress, serverPort);
}
