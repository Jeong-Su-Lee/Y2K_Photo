#include "firstwindow.h"
#include "ui_firstwindow.h"
#include "secondwindow.h"
#include "endingwindow.h"
#include "bgmsoundplayer2.h"

FirstWindow::FirstWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FirstWindow)
{
    ui->setupUi(this);
    erevePlayer.startMusic();
}

FirstWindow::~FirstWindow()
{
    delete ui;
}

void FirstWindow::on_btnFirst_clicked()
{
    ui->btnFirst->setEnabled(false);
    SecondWindow *secondWindow = new SecondWindow();
    this->hide();
    secondWindow->show();
}
