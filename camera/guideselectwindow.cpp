#include "guideselectwindow.h"
#include "ui_guideselectwindow.h"
#include "photowindow.h"
#include "imagefilter2.h"

GuideSelectWindow::GuideSelectWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GuideSelectWindow),
    timer(new QTimer())
{
    ui->setupUi(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(go_to_nextWindow()));
    timer->start(1000*4);
}

void GuideSelectWindow::go_to_nextWindow()
{
    PhotoWindow *photoWindow = new PhotoWindow();
    this->hide();
    photoWindow->show();

    timer->stop();
}

GuideSelectWindow::~GuideSelectWindow()
{
    delete ui;
    delete timer;
}


void GuideSelectWindow::on_btnNone_clicked()
{
    // handle on clicked "None" guide
}

void GuideSelectWindow::on_btnHeart_clicked()
{
    // handle on clicked "Heart" guide
}

void GuideSelectWindow::on_btnStar_clicked()
{
    // handle on clicked "Star" guide
}

void GuideSelectWindow::on_btnLG_clicked()
{
    // handle on clicked "LG logo" guide
}
