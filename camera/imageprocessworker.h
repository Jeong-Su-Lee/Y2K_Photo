#ifndef IMAGEPROCESSORWORKER_H
 #define IMAGEPROCESSORWORKER_H

 #include <QObject>
 #include <QPixmap>
 #include <QImage>

 class ImageProcessorWorker : public QObject
 {
     Q_OBJECT
 public:
     explicit ImageProcessorWorker(QObject *parent = nullptr);
     bool yuyv2rgb(const uchar *yuyv, int width, int height, uchar *rgb);
     void yuyv_to_rgb_pixel(const uchar *yuyv, uchar *rgb);

 public slots:
     void processFrame(QImage image, const QString &guideName, const QString &clientId);

 signals:
     void frameProcessed(const QPixmap &pixmap, const QString &clientId);
 };

 #endif // IMAGEPROCESSORWORKER_H
