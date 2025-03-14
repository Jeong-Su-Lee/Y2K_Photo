#include "photowindow.h"
#include "ui_photowindow.h"
#include "endingwindow.h"
#include <QString>

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

//    connect(nextTimer, SIGNAL(timeout()), this, SLOT(go_to_nextWindow()));
//    nextTimer->start(1000 * TIME_LIMIT);
}


void PhotoWindow::change_timeText()
{
    int curTime = ui->lblTime->text().toInt();
    if (curTime) {
        ui->lblTime->setText(QString::number(curTime-1));
    } else {
        int curCount = ui->lblCount->text().toInt();

        if (curCount == 8) return go_to_nextWindow();

        ui->lblCount->setText(QString::number(curCount+1));
        ui->lblTime->setText(QString::number(TIME_LIMIT));
    }
}

void PhotoWindow::go_to_nextWindow()
{
    EndingWindow *endingWindow = new EndingWindow();
    this->hide();
    endingWindow->show();

    timer->stop();
//    nextTimer->stop();
}


PhotoWindow::~PhotoWindow()
{
    delete ui;
    delete timer;
    delete nextTimer;
}
