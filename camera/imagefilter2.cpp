#include "imagefilter2.h"
#include "ui_imagefilter2.h"
#include "imagedecoration2.h"
#include <QPainter>
#include <QPixmap>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QResizeEvent>
#include <QImage>
#include <QIcon>


ImageFilter2::ImageFilter2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageFilter2)
{
    ui->setupUi(this);

    // 필터 버튼 연결
    QVector<QPushButton*> filterButtons = {
        ui->filterButton1, ui->filterButton2, ui->filterButton3,
        ui->filterButton4, ui->filterButton5, ui->filterButton6
    };

    filterColors = {
        QColor(74, 79, 79, 100), QColor(249, 203, 203, 100), QColor(249, 222, 149, 100),
        QColor(242, 242, 242, 100), QColor(249, 149, 151, 100), QColor(174, 225, 239, 100)
    };

    ui->resetButton->setStyleSheet("background-color: #fff; border-radius: 30px; border: 2px solid #ccc; font-size: 40px; font-weight: bold;");
    connect(ui->resetButton, &QPushButton::clicked, this, &ImageFilter2::resetFilter);

    ui->bwButton->setStyleSheet("background-color: black; border-radius: 30px; border: 2px solid white;");
    connect(ui->bwButton, &QPushButton::clicked, this, &ImageFilter2::applyBWFilter);


    for (int i = 0; i < filterButtons.size(); ++i) {
        filterButtons[i]->setStyleSheet("background-color: " + filterColors[i].name() + "; border-radius: 30px; border: 2px solid white;");
        connect(filterButtons[i], &QPushButton::clicked, this, [=]() { applyFilter(i); });
    }

    if (!image.load("/mnt/sd/final.jpg")) {
        qWarning("Failed to load image!");
    }
    image = image.scaled(785, 600, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->lblImg->setPixmap(QPixmap::fromImage(image));
    filteredImage = image;
}

ImageFilter2::~ImageFilter2()
{
    delete ui;
}

void ImageFilter2::on_saveButton_clicked()
{
    ui->saveButton->setEnabled(false);
    saveImage();
}


void ImageFilter2::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    update();
}

void ImageFilter2::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    int imageY = (height() - filteredImage.height()) / 2;
    painter.fillRect(rect(), Qt::white);
    painter.drawImage(0, imageY, filteredImage);
}

void ImageFilter2::applyFilter(int filterIndex) {
    filteredImage = image;
    QPainter painter(&filteredImage);


    if (filterIndex >= 0 && filterIndex < filterColors.size()) {
        painter.fillRect(filteredImage.rect(), filterColors[filterIndex]);
    }
//    update();
    ui->lblImg->setPixmap(QPixmap::fromImage(filteredImage));

}


void ImageFilter2::applyBWFilter() {
    filteredImage = image.convertToFormat(QImage::Format_Grayscale8);
    ui->lblImg->setPixmap(QPixmap::fromImage(filteredImage));
}


void ImageFilter2::resetFilter() {
    filteredImage = image;
    ui->lblImg->setPixmap(QPixmap::fromImage(filteredImage));
}

void ImageFilter2::saveImage() {
    int displayHeight = height();
    int imageHeight = filteredImage.height();
    int topBottomMargin = (displayHeight - imageHeight) / 2;
    if (topBottomMargin < 0) topBottomMargin = 0;

    int canvasWidth = filteredImage.width();
    int canvasHeight = imageHeight + (2 * topBottomMargin);
    QImage finalImage(QSize(canvasWidth, canvasHeight), QImage::Format_ARGB32_Premultiplied);
    finalImage.fill(Qt::white);

    QPainter painter(&finalImage);
    painter.drawImage(0, topBottomMargin, filteredImage);
    finalImage.save("/mnt/sd/filtered_image.jpg", "JPG");

    ImageDecoration2 *imageDecoration = new ImageDecoration2();
    this->hide();
    imageDecoration->show();
}


