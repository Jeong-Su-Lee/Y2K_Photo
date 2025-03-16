#ifndef IMAGEFILTER_H
#define IMAGEFILTER_H

#include <QWidget>
#include <QImage>
#include <QPushButton>

class QLabel;

class ImageFilter : public QWidget {
    Q_OBJECT

public:
    explicit ImageFilter(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void applyFilter(int filterIndex);
    void saveImage();

private:
    QImage image;
    QImage filteredImage;
    QWidget *leftWidget;
    QVector<QPushButton*> filterButtons;
};

#endif // IMAGEFILTER_H
