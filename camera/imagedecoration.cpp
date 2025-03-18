#include "imagedecoration.h"
#include "endingwindow.h"
#include <QPainter>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QColorDialog>
#include <QSlider>
#include <QResizeEvent>

ImageDecoration::ImageDecoration(QWidget *parent) : QWidget(parent), currentColor(Qt::black), currentBrushSize(5) {
    setMinimumSize(1024, 600);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setStyleSheet("background-color: white;");

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 왼쪽 캔버스 공간
    leftWidget = new QWidget(this);
    leftWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    leftWidget->setStyleSheet("background: transparent;"); // 배경을 투명하게 설정
    mainLayout->addWidget(leftWidget);

    // 오른쪽 UI 공간
    QWidget *rightWidget = new QWidget(this);
    rightWidget->setFixedSize(239, 600);
    rightWidget->setStyleSheet("background-color: white; border-left: 2px solid #ccc;"); // 왼쪽 경계선 추가
    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setAlignment(Qt::AlignCenter);

    QLabel *titleLabel = new QLabel("Decorate Photos", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 22px; font-weight: bold; font-family: Arial, sans-serif;");
    rightLayout->addWidget(titleLabel);
    rightLayout->addStretch();

    // 색상 선택 UI (원형 버튼)
    QGridLayout *colorLayout = new QGridLayout();
    colorLayout->setAlignment(Qt::AlignCenter);
    QVector<QColor> colors = {
        QColor(79, 79, 79),   // 색1 (기본 펜 색)
        QColor(159, 225, 153), // 색2
        QColor(249, 222, 149), // 색3
        QColor(249, 149, 151), // 색4
        QColor(210, 239, 246)  // 색5
    };
    for (int i = 0; i < colors.size(); ++i) {
        QPushButton *colorButton = new QPushButton(this);
        colorButton->setStyleSheet(QString("background-color: rgb(%1, %2, %3); border-radius: 22px; width: 44px; height: 44px; border: 2px solid #ccc;")
                                   .arg(colors[i].red()).arg(colors[i].green()).arg(colors[i].blue()));
        connect(colorButton, &QPushButton::clicked, this, [=]() { changeColor(colors[i]); });
        colorLayout->addWidget(colorButton, i / 3, i % 3, Qt::AlignCenter);
    }
    rightLayout->addLayout(colorLayout);
    rightLayout->addStretch();

    // 브러시 크기 조절 UI
    QLabel *brushSizeLabel = new QLabel("Brush Size", this);
    brushSizeLabel->setAlignment(Qt::AlignCenter);
    brushSizeLabel->setStyleSheet("font-size: 18px; font-family: Arial, sans-serif;");
    rightLayout->addWidget(brushSizeLabel);

    brushSizeSlider = new QSlider(Qt::Horizontal, this);
    brushSizeSlider->setFixedSize(200, 40);
    brushSizeSlider->setRange(1, 20);
    brushSizeSlider->setValue(currentBrushSize);
    brushSizeSlider->setStyleSheet("border: none;");
    connect(brushSizeSlider, &QSlider::valueChanged, this, &ImageDecoration::changeBrushSize);
    rightLayout->addWidget(brushSizeSlider);
    rightLayout->addStretch();

    // 기능 버튼 UI
    QString buttonStyle = "QPushButton { background-color: #007BFF; color: white; font-size: 16px; font-weight: bold; font-family: Arial, sans-serif; border-radius: 10px; padding: 10px; } QPushButton:hover { background-color: #0056b3; }";

    QPushButton *clearButton = new QPushButton("Clear", this);
    clearButton->setFixedSize(200, 50);
    clearButton->setStyleSheet(buttonStyle);
    connect(clearButton, &QPushButton::clicked, this, &ImageDecoration::clearDrawing);

    QPushButton *undoButton = new QPushButton("Undo", this);
    undoButton->setFixedSize(200, 50);
    undoButton->setStyleSheet(buttonStyle);
    connect(undoButton, &QPushButton::clicked, this, &ImageDecoration::undo);

    QPushButton *redoButton = new QPushButton("Redo", this);
    redoButton->setFixedSize(200, 50);
    redoButton->setStyleSheet(buttonStyle);
    connect(redoButton, &QPushButton::clicked, this, &ImageDecoration::redo);

    QPushButton *saveButton = new QPushButton("Save and Done", this);
    saveButton->setFixedSize(200, 50);
    saveButton->setStyleSheet(buttonStyle);
    connect(saveButton, &QPushButton::clicked, this, &ImageDecoration::saveImage);

    QVBoxLayout *buttonLayout = new QVBoxLayout();
    buttonLayout->setAlignment(Qt::AlignCenter);
    buttonLayout->addWidget(clearButton);
    buttonLayout->addWidget(undoButton);
    buttonLayout->addWidget(redoButton);
    buttonLayout->addWidget(saveButton);

    rightLayout->addLayout(buttonLayout);
    mainLayout->addWidget(rightWidget);

    // 이미지 로드
    if (!image.load("/mnt/sd/filtered_image.jpg")) {
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
    QPen pen(currentColor, currentBrushSize);
    painter.setPen(pen);
    painter.drawLine(lastPoint, event->pos());
    lastPoint = event->pos();
    currentLine.append({event->pos(), pen});
    update();
}

void ImageDecoration::mouseReleaseEvent(QMouseEvent *event) {
    if (!currentLine.isEmpty()) {
        undoStack.append(currentLine);
        redoStack.clear();
    }
    lastPoint = QPoint();
}

void ImageDecoration::clearDrawing() {
    canvas.fill(Qt::transparent);
    undoStack.clear();
    redoStack.clear();
    update();
}

void ImageDecoration::undo() {
    if (!undoStack.isEmpty()) {
        redoStack.append(undoStack.takeLast());
        redrawCanvas();
    }
}

void ImageDecoration::redo() {
    if (!redoStack.isEmpty()) {
        undoStack.append(redoStack.takeLast());
        redrawCanvas();
    }
}

void ImageDecoration::redrawCanvas() {
    canvas.fill(Qt::transparent);
    QPainter painter(&canvas);
    for (const auto &line : undoStack) {
        for (int i = 1; i < line.size(); ++i) {
            painter.setPen(line[i].second);
            painter.drawLine(line[i - 1].first, line[i].first);
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
    finalImage.save("decorated_image.jpg", "JPG");

    EndingWindow* endingWindow = new EndingWindow();
    this->hide();
    endingWindow->show();
}

void ImageDecoration::changeColor(const QColor &color) {
    currentColor = color;
}


void ImageDecoration::changeBrushSize(int size) {
    currentBrushSize = size;
}
