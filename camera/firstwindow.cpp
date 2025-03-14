#include "firstwindow.h"
#include "ui_firstwindow.h"
#include "secondwindow.h"
#include "endingwindow.h"

FirstWindow::FirstWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FirstWindow)
{
    ui->setupUi(this);
}

FirstWindow::~FirstWindow()
{
    delete ui;
}

void FirstWindow::on_btnFirst_clicked()
{
    SecondWindow *secondWindow = new SecondWindow();
    this->hide();
    secondWindow->show();
}
