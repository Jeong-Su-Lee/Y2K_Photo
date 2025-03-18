#include "imagefilter.h"
#include "imagedecoration.h"
#include <QPainter>
#include <QPixmap>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QResizeEvent>
#include <QImage>
#include <QIcon>

ImageFilter::ImageFilter(QWidget *parent) : QWidget(parent) {
    setMinimumSize(1024, 600);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    setStyleSheet("background-image: url(:/images/imagefilter.png); background-position: center; background-repeat: no-repeat;");

    QPixmap bgImage(":images/imagefilter.png");  // 배경 이미지 로드
    bgImage = bgImage.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    QPalette palette;
    palette.setBrush(QPalette::Window, bgImage);
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    leftWidget = new QWidget(this);
    leftWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    leftWidget->setStyleSheet("background: transparent;");
    mainLayout->addWidget(leftWidget);

    QWidget *rightWidget = new QWidget(this);
    rightWidget->setFixedSize(239, 600);
    rightWidget->setStyleSheet("background-color: white; border-left: 2px solid #ccc;");
    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setAlignment(Qt::AlignCenter);

    QGridLayout *filterLayout = new QGridLayout();
    QVector<QColor> filterColors = {
        QColor(74, 79, 79), QColor(249, 203, 203), QColor(249, 222, 149),
        QColor(242, 242, 242), QColor(249, 149, 151), QColor(174, 225, 239)
    };

    for (int i = 0; i < filterColors.size(); ++i) {
        QPushButton *filterButton = new QPushButton(this);
        filterButton->setFixedSize(44, 44);
        filterButton->setStyleSheet("background-color: " + filterColors[i].name() + "; border-radius: 22px; border: 2px solid #ccc;");
        connect(filterButton, &QPushButton::clicked, this, [=]() { applyFilter(i); });
        filterButtons.append(filterButton);
        filterLayout->addWidget(filterButton, i / 3, i % 3, Qt::AlignCenter);
    }
    rightLayout->addLayout(filterLayout);
    rightLayout->addStretch();

    QPushButton *saveButton = new QPushButton(this);
    saveButton->setFixedSize(164, 54);
    saveButton->setIcon(QIcon(QPixmap(":images/filternextbtn.png")));
    saveButton->setIconSize(saveButton->size());
    saveButton->setFlat(true);
    saveButton->setStyleSheet("border: none; outline: none;");
    connect(saveButton, &QPushButton::clicked, this, &ImageFilter::saveImage);
    rightLayout->addWidget(saveButton);
    rightLayout->addStretch();

    mainLayout->addWidget(rightWidget);

    if (!image.load("/mnt/sd/final.jpg")) {
        qWarning("Failed to load image!");
    }
    image = image.scaled(785, 600, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    filteredImage = image;
}

void ImageFilter::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    update();
}

void ImageFilter::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    int imageY = (height() - filteredImage.height()) / 2;
    painter.fillRect(rect(), Qt::white);
    painter.drawImage(0, imageY, filteredImage);
}

void ImageFilter::applyFilter(int filterIndex) {
    filteredImage = image;
    QPainter painter(&filteredImage);
    QVector<QColor> filterColors = {
        QColor(74, 79, 79, 100), QColor(249, 203, 203, 100), QColor(249, 222, 149, 100),
        QColor(242, 242, 242, 100), QColor(249, 149, 151, 100), QColor(174, 225, 239, 100)
    };

    if (filterIndex >= 0 && filterIndex < filterColors.size()) {
        painter.fillRect(filteredImage.rect(), filterColors[filterIndex]);
    }
    update();
}

void ImageFilter::saveImage() {
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
    finalImage.save("filtered_image.jpg", "JPG");

    ImageDecoration *imageDecoration = new ImageDecoration();
    this->hide();
    imageDecoration->show();
}
