#include "imagedecoration2.h"
#include "ui_imagedecoration2.h"
#include "endingwindow.h"
#include "bgmsoundplayer2.h"
#include <QPainter>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QColorDialog>
#include <QSlider>
#include <QResizeEvent>


ImageDecoration2::ImageDecoration2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageDecoration2),
    currentBrushSize(5),
    currentPenType(BasicPen)
{
    ui->setupUi(this);

    connect(ui->basicPenButton, &QPushButton::clicked, this, &ImageDecoration2::setPenTypeBasic);
    connect(ui->wormPenButton, &QPushButton::clicked, this, &ImageDecoration2::setPenTypeWorm);

    QVector<QPushButton*> colorButtons = {
        ui->colorButton1, ui->colorButton2, ui->colorButton3,
        ui->colorButton4, ui->colorButton5, ui->colorButton6
    };

    QVector<QColor> colors = {
        QColor(79, 79, 79), QColor(159, 225, 153), QColor(249, 222, 149),
        QColor(249, 149, 151), QColor(210, 239, 246), QColor(180, 200, 255)
    };
    currentColor = colors[0]; // 기본 펜 색상을 UI의 첫 번째 색상으로 설정

    for (int i = 0; i < colors.size(); ++i) {
        colorButtons[i]->setStyleSheet(QString("background-color: rgb(%1, %2, %3); border-radius: 25px; border: 2px solid white;")
                                   .arg(colors[i].red()).arg(colors[i].green()).arg(colors[i].blue()));
        connect(colorButtons[i], &QPushButton::clicked, this, [=]() { changeColor(colors[i]); });
    }


    ui->brushSizeSlider->setValue(currentBrushSize);
    ui->brushSizeSlider->setStyleSheet("border: none;");
    connect(ui->brushSizeSlider, &QSlider::valueChanged, this, &ImageDecoration2::changeBrushSize);

    connect(ui->resetButton, &QPushButton::clicked, this, &ImageDecoration2::clearDrawing);
    connect(ui->undoButton, &QPushButton::clicked, this, &ImageDecoration2::undo);
    connect(ui->saveButton, &QPushButton::clicked, this, &ImageDecoration2::saveImage);


    if (!image.load("/mnt/sd/filtered_image.jpg")) {
        qWarning("Failed to load image!");
    }
    image = image.scaled(785, 600, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    canvas = QImage(785, 600, QImage::Format_ARGB32_Premultiplied);
    canvas.fill(Qt::transparent);

    ui->lblCanvas->setPixmap(QPixmap::fromImage(image));

}

ImageDecoration2::~ImageDecoration2()
{
    delete ui;
}


void ImageDecoration2::updateCanvas()
{
    QImage combinedImage = image;
    QPainter painter(&combinedImage);
    painter.drawImage(0, 0, canvas); // 이미지 위에 그리기
    ui->lblCanvas->setPixmap(QPixmap::fromImage(combinedImage));
}


void ImageDecoration2::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    updateCanvas();
}


void ImageDecoration2::mousePressEvent(QMouseEvent *event) {
    lastPoint = event->pos();
    currentLine.clear();
    currentLine.append({event->pos(), QPen(currentColor, currentBrushSize)});
    updateCanvas();
}


void ImageDecoration2::mouseMoveEvent(QMouseEvent *event) {
    if (lastPoint.isNull()) return;  // 드로잉이 시작되지 않았다면 무시

    QPainter painter(&canvas);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPen pen(currentColor, currentBrushSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QVector<QPair<QPoint, QPen>> newSegment;

    switch (currentPenType) {
        case BasicPen: {
            painter.setPen(pen);
            painter.drawLine(lastPoint, event->pos());
            newSegment.append({lastPoint, pen});
            newSegment.append({event->pos(), pen});
            break;
        }
        case WormPen: {
            QColor borderColor = (currentColor == Qt::white || currentColor == QColor(79, 79, 79)) ? Qt::white : Qt::black;
            int borderSize = currentBrushSize + 4;
            QPen borderPen(borderColor, borderSize);
            borderPen.setCapStyle(Qt::RoundCap);

            painter.setPen(borderPen);
            painter.drawLine(lastPoint, event->pos());
            newSegment.append({lastPoint, borderPen});
            newSegment.append({event->pos(), borderPen});

            painter.setPen(pen);
            painter.drawLine(lastPoint, event->pos());
            newSegment.append({lastPoint, pen});
            newSegment.append({event->pos(), pen});
            break;
        }
    }

    currentLine.append(newSegment);
    lastPoint = event->pos();

    updateCanvas();
}

void ImageDecoration2::mouseReleaseEvent(QMouseEvent *event) {
    if (!currentLine.isEmpty()) {
        undoStack.append(currentLine);  // 현재 선 그룹을 undoStack에 저장
        currentLine.clear(); // 현재 선 초기화
    }
    lastPoint = QPoint();
}

void ImageDecoration2::clearDrawing() {
    canvas.fill(Qt::transparent);
    undoStack.clear();
    updateCanvas();
}

void ImageDecoration2::undo() {
    if (!undoStack.isEmpty()) {
        undoStack.pop_back();  // 마지막으로 그린 선 그룹 전체 삭제
        redrawCanvas(); // 모든 선을 다시 그림
    }
}


void ImageDecoration2::redrawCanvas() {
    canvas.fill(Qt::transparent);

    QPainter painter(&canvas);
    painter.setRenderHint(QPainter::Antialiasing, true);

    for (const auto &lineGroup : undoStack) {
        if (!lineGroup.isEmpty()) {
            for (int i = 1; i < lineGroup.size(); ++i) {
                const auto &startPoint = lineGroup[i - 1].first;
                const auto &endPoint = lineGroup[i].first;
                const QPen &pen = lineGroup[i].second;

                painter.setPen(pen);
                painter.drawLine(startPoint, endPoint);
            }
        }
    }


    updateCanvas();
}

void ImageDecoration2::saveImage() {
    int imageY = (canvas.height() - image.height()) / 2;
    QImage finalImage(canvas.size(), QImage::Format_ARGB32_Premultiplied);
    finalImage.fill(Qt::white);
    QPainter painter(&finalImage);
    painter.drawImage(0, imageY, image);
    painter.drawImage(0, imageY, canvas);
    finalImage.save("/mnt/sd/decorated_image.jpg", "JPG");

    erevePlayer.stopMusic();
    EndingWindow *endingWindow = new EndingWindow();
    this->hide();
    endingWindow->show();
}

void ImageDecoration2::changeColor(const QColor &color) {
    currentColor = color;
}


void ImageDecoration2::changeBrushSize(int size) {
    currentBrushSize = size;
}

void ImageDecoration2::setPenTypeBasic() {
    if (currentPenType == BasicPen) return;
    currentPenType = BasicPen;
    ui->basicPenButton->setIcon(QIcon(":/images/solidactive.png"));
    ui->wormPenButton->setIcon(QIcon(":/images/dotinactive.png"));
}

void ImageDecoration2::setPenTypeWorm() {
    if (currentPenType == WormPen) return;
    currentPenType = WormPen;
    ui->basicPenButton->setIcon(QIcon(":/images/solidinactive.png"));
    ui->wormPenButton->setIcon(QIcon(":/images/dotactive.png"));

}
