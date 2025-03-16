#ifndef IMAGEDECORATION_H
#define IMAGEDECORATION_H

#include <QWidget>
#include <QImage>
#include <QPoint>
#include <QVector>
#include <QColor>
#include <QPen>
#include <QPair>

class QLabel;
class QSlider;

class ImageDecoration : public QWidget {
    Q_OBJECT

public:
    explicit ImageDecoration(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void clearDrawing();
    void undo();
    void redo();
    void saveImage();
    void changeColor(const QColor &color);
    void changeBrushSize(int size);

private:
    QImage image;
    QImage canvas;
    QPoint lastPoint;
    QColor currentColor;
    int currentBrushSize;
    QSlider *brushSizeSlider;
    QWidget *leftWidget;

    QVector<QVector<QPair<QPoint, QPen>>> undoStack;
    QVector<QVector<QPair<QPoint, QPen>>> redoStack;
    QVector<QPair<QPoint, QPen>> currentLine;

    void redrawCanvas();
};

#endif // IMAGEDECORATION_H
