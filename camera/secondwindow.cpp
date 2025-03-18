#include "secondwindow.h"
#include "ui_secondwindow.h"
#include "guideselectwindow.h"

SecondWindow::SecondWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SecondWindow),
    timer(new QTimer),
    nextTimer(new QTimer)
{
    ui->setupUi(this);
    udp_temporal = new UDPListenerThread(this);
    connect(udp_temporal, &UDPListenerThread::connCompleteReceived, this, &SecondWindow::changeWindow);
    QUdpSocket udpSocket;

    QByteArray datagram = "TEMP";
    QHostAddress targetIp("192.168.10.2");
    quint16 targetPort = 25000;

    udpSocket.writeDatagram(datagram, targetIp, targetPort);
    udp_temporal->start();

    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimerAlarm()));
    timer->start(1000);

    connect(nextTimer, SIGNAL(timeout()), this, SLOT(changeWindow()));
    nextTimer->start(1000*3);
}

void SecondWindow::slotTimerAlarm()
{
    int cur_len = ui->lblSecond->text().size();

    if (cur_len == 27) ui->lblSecond->setText("Waiting for the other user..");
    else if (cur_len == 28) ui->lblSecond->setText("Waiting for the other user...");
    else if (cur_len == 29) ui->lblSecond->setText("Waiting for the other user.");
}

void SecondWindow::changeWindow()
{
    GuideSelectWindow *guideSelectWindow = new GuideSelectWindow();
    this->hide();
    guideSelectWindow->show();

    timer->stop();
    nextTimer->stop();
    delete udp_temporal;
}

SecondWindow::~SecondWindow()
{
    delete ui;
    delete timer;
    delete nextTimer;
}
