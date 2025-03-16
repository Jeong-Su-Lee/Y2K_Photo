#include "guideselectwindow.h"
#include "ui_guideselectwindow.h"
#include "photowindow.h"
#include "imagefilter.h"

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
//    ImageFilter *imageFilter = new ImageFilter();
//    this->hide();
//    imageFilter->show();

    timer->stop();
}

GuideSelectWindow::~GuideSelectWindow()
{
    delete ui;
    delete timer;
}
