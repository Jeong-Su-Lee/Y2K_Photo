#ifndef IMAGEFILTER2_H
#define IMAGEFILTER2_H

#include <QWidget>

namespace Ui {
class ImageFilter2;
}

class ImageFilter2 : public QWidget
{
    Q_OBJECT

public:
    explicit ImageFilter2(QWidget *parent = 0);
    ~ImageFilter2();

private slots:
    void on_saveButton_clicked();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private slots:
    void applyFilter(int filterIndex);
    void saveImage();
    void applyBWFilter();
    void resetFilter();

private:
    Ui::ImageFilter2 *ui;
    QImage image;
    QImage filteredImage;
    QVector<QColor> filterColors;
};

#endif // IMAGEFILTER2_H


