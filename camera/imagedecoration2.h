#ifndef IMAGEDECORATION2_H
#define IMAGEDECORATION2_H

#include <QWidget>
#include <QImage>
#include <QPoint>
#include <QVector>
#include <QColor>
#include <QPen>
#include <QPair>


namespace Ui {
class ImageDecoration2;
}

class ImageDecoration2 : public QWidget
{
    Q_OBJECT

public:
    explicit ImageDecoration2(QWidget *parent = 0);
    ~ImageDecoration2();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void clearDrawing();
    void undo();
    void saveImage();
    void changeColor(const QColor &color);
    void changeBrushSize(int size);
    void setPenTypeBasic();
    void setPenTypeWorm();


private:
    Ui::ImageDecoration2 *ui;

    enum PenType {
        BasicPen,
        WormPen
    };

    QImage image;
    QImage canvas;
    QPoint lastPoint;
    QColor currentColor;
    int currentBrushSize;

    QVector<QVector<QPair<QPoint, QPen>>> undoStack;
    QVector<QPair<QPoint, QPen>> currentLine;

    PenType currentPenType;

    void redrawCanvas();
    void updateCanvas();
};

#endif // IMAGEDECORATION2_H
