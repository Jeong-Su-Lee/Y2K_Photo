#include "imagedecoration.h"
#include <QPainter>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QColorDialog>
#include <QSlider>
#include <QResizeEvent>

ImageDecoration::ImageDecoration(QWidget *parent) : QWidget(parent), currentBrushSize(5), currentPenType(BasicPen) {
    setMinimumSize(1024, 600);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setStyleSheet("background-color: white;");

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
    rightLayout->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    rightLayout->setSpacing(20);

    QLabel *titleLabel = new QLabel("Decorate Photos", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 22px; font-weight: bold; font-family: Arial, sans-serif;");
    rightLayout->addWidget(titleLabel);

    QHBoxLayout *penLayout = new QHBoxLayout();
    penLayout->setSpacing(5);
    penLayout->setAlignment(Qt::AlignCenter);

    QPushButton *basicPenButton = new QPushButton("Basic", this);
    QPushButton *wormPenButton = new QPushButton("Worm", this);

    basicPenButton->setStyleSheet("padding: 5px; font-size: 14px;");
    wormPenButton->setStyleSheet("padding: 5px; font-size: 14px;");

    connect(basicPenButton, &QPushButton::clicked, this, [=]() { currentPenType = BasicPen; });
    connect(wormPenButton, &QPushButton::clicked, this, [=]() { currentPenType = WormPen; });

    penLayout->addWidget(basicPenButton);
    penLayout->addWidget(wormPenButton);
    rightLayout->addLayout(penLayout);


    QGridLayout *colorLayout = new QGridLayout();
    colorLayout->setSpacing(10);
    colorLayout->setAlignment(Qt::AlignCenter);

    QVector<QColor> colors = {
        QColor(79, 79, 79), QColor(159, 225, 153), QColor(249, 222, 149),
        QColor(249, 149, 151), QColor(210, 239, 246), QColor(180, 200, 255)
    };

    currentColor = colors[0]; // 기본 펜 색상을 UI의 첫 번째 색상으로 설정

    for (int i = 0; i < colors.size(); ++i) {
        QPushButton *colorButton = new QPushButton(this);
        colorButton->setFixedSize(60, 60);
        colorButton->setStyleSheet(QString("background-color: rgb(%1, %2, %3); border-radius: 30px; border: 2px solid #ccc;")
                                   .arg(colors[i].red()).arg(colors[i].green()).arg(colors[i].blue()));
        connect(colorButton, &QPushButton::clicked, this, [=]() { changeColor(colors[i]); });
        colorLayout->addWidget(colorButton, i / 3, i % 3, Qt::AlignCenter);
    }

    rightLayout->addLayout(colorLayout);
    rightLayout->addStretch();

    QLabel *brushSizeLabel = new QLabel("Brush Size", this);
    brushSizeLabel->setAlignment(Qt::AlignCenter);
    brushSizeLabel->setStyleSheet("font-size: 18px; font-family: Arial, sans-serif;");
    rightLayout->addWidget(brushSizeLabel);

    brushSizeSlider = new QSlider(Qt::Horizontal, this);
    brushSizeSlider->setFixedSize(200, 70);
    brushSizeSlider->setRange(1, 20);
    brushSizeSlider->setValue(currentBrushSize);
    brushSizeSlider->setStyleSheet("border: none;");
    connect(brushSizeSlider, &QSlider::valueChanged, this, &ImageDecoration::changeBrushSize);
    rightLayout->addWidget(brushSizeSlider);
    rightLayout->addStretch();

    QPushButton *clearButton = new QPushButton("Clear", this);
    QPushButton *undoButton = new QPushButton("Undo", this);
    QPushButton *saveButton = new QPushButton("Save and Done", this);

    QString buttonStyle = "QPushButton { background-color: #007BFF; color: white; font-size: 16px; font-weight: bold; font-family: Arial, sans-serif; border-radius: 10px; padding: 10px; } QPushButton:hover { background-color: #0056b3; }";
    clearButton->setFixedSize(200, 50);
    clearButton->setStyleSheet(buttonStyle);
    connect(clearButton, &QPushButton::clicked, this, &ImageDecoration::clearDrawing);

    undoButton->setFixedSize(200, 50);
    undoButton->setStyleSheet(buttonStyle);
    connect(undoButton, &QPushButton::clicked, this, &ImageDecoration::undo);

    saveButton->setFixedSize(200, 50);
    saveButton->setStyleSheet(buttonStyle);
    connect(saveButton, &QPushButton::clicked, this, &ImageDecoration::saveImage);

    QVBoxLayout *buttonLayout = new QVBoxLayout();
    buttonLayout->setAlignment(Qt::AlignCenter);
    buttonLayout->addWidget(clearButton);
    buttonLayout->addWidget(undoButton);
    buttonLayout->addWidget(saveButton);

    rightLayout->addLayout(buttonLayout);
    mainLayout->addWidget(rightWidget);

    if (!image.load("/mnt/sd/test_image_jpgtype.jpg")) {
        qWarning("Failed to load image!");
    }
    image = image.scaled(785, 600, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    canvas = QImage(785, 600, QImage::Format_ARGB32_Premultiplied);
    canvas.fill(Qt::transparent);
}


void ImageDecoration::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    update();
}

void ImageDecoration::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    int imageY = (height() - image.height()) / 2;
    painter.drawImage(0, imageY, image);
    painter.drawImage(0, imageY, canvas);
}

void ImageDecoration::mousePressEvent(QMouseEvent *event) {
    if (!leftWidget->geometry().contains(event->pos())) return;
    lastPoint = event->pos();
    currentLine.clear();
    currentLine.append({event->pos(), QPen(currentColor, currentBrushSize)});
}

void ImageDecoration::mouseMoveEvent(QMouseEvent *event) {
    if (!leftWidget->geometry().contains(event->pos()) || lastPoint.isNull()) return;

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
    update();
}


void ImageDecoration::mouseReleaseEvent(QMouseEvent *event) {
    if (!currentLine.isEmpty()) {
        undoStack.append(currentLine);  // 현재 선 그룹을 undoStack에 저장
        currentLine.clear(); // 현재 선 초기화
    }
    lastPoint = QPoint();
}

void ImageDecoration::clearDrawing() {
    canvas.fill(Qt::transparent);
    undoStack.clear();
    update();
}

void ImageDecoration::undo() {
    if (!undoStack.isEmpty()) {
        undoStack.pop_back();  // 마지막으로 그린 선 그룹 전체 삭제
        redrawCanvas(); // 모든 선을 다시 그림
    }
}

void ImageDecoration::redrawCanvas() {
    canvas.fill(Qt::transparent);  // 캔버스를 다시 초기화

    QPainter painter(&canvas);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 기존 undoStack을 다시 그리기
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

    update();
}

void ImageDecoration::saveImage() {
    int imageY = (canvas.height() - image.height()) / 2;
    QImage finalImage(canvas.size(), QImage::Format_ARGB32_Premultiplied);
    finalImage.fill(Qt::white);
    QPainter painter(&finalImage);
    painter.drawImage(0, imageY, image);
    painter.drawImage(0, imageY, canvas);
    finalImage.save("/mnt/sd/decorated_image.jpg", "JPG");
}

void ImageDecoration::changeColor(const QColor &color) {
    currentColor = color;
}


void ImageDecoration::changeBrushSize(int size) {
    currentBrushSize = size;
}
