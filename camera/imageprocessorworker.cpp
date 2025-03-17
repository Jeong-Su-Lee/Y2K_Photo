#include "imageprocessorworker.h"
#include <QColor>
#include <QFile>
#include <QDebug>
#include <QPainter>

// yuyv2rgb 함수는 외부에 따로 정의되어 있어야 해
extern void yuyv2rgb(const uchar *yuyv, int width, int height, uchar *rgb);

ImageProcessorWorker::ImageProcessorWorker(QObject *parent)
    : QObject(parent)
{
}

void ImageProcessorWorker::processFrame(const QByteArray &data, int width, int height, const QString &guideName, const QString &clientId)
{
    // 1. YUYV → RGB888
    QByteArray rgbBuf(width * height * 3, 0);
    yuyv2rgb(reinterpret_cast<const uchar*>(data.constData()), width, height, reinterpret_cast<uchar*>(rgbBuf.data()));

    QImage image(reinterpret_cast<uchar*>(rgbBuf.data()), width, height, QImage::Format_RGB888);
    image = image.mirrored(true, false);

    // // // 2. 가이드 이미지 처리
    // static int number_of_guide = 0;
    // QString guideNum = QString::number(number_of_guide);
    // QString filename = "/mnt/nfs/guide/guide_" + guideName + "/" + guideNum + "_" + guideName + ".png";

    // if (QFile::exists(filename)) {
    //     QImage guideImage(filename);
    //     guideImage = guideImage.convertToFormat(QImage::Format_ARGB32);

    //     for (int y = 0; y < guideImage.height(); ++y) {
    //         QRgb *line = reinterpret_cast<QRgb*>(guideImage.scanLine(y));
    //         for (int x = 0; x < guideImage.width(); ++x) {
    //             QColor color = QColor::fromRgba(line[x]);
    //             color.setAlphaF(color.alphaF() * 0.5); // 반투명
    //             line[x] = color.rgba();
    //         }
    //     }

    //     QPainter painter(&image);
    //     painter.drawImage(0, 0, guideImage);
    // }

    QPixmap finalPixmap = QPixmap::fromImage(image);

    emit frameProcessed(finalPixmap, clientId);
}

bool ImageProcessorWorker::yuyv2rgb(const uchar *yuyv, int width, int height, uchar *rgb)
{
    long yuv_size = height * width * 2;
    long rgb_size = height * width * 3;

    if (yuyv == NULL || rgb == NULL)
    return false;

    for (int i = 0, j = 0; i < rgb_size && j < yuv_size; i += 6, j += 4)
    {
    yuyv_to_rgb_pixel(&yuyv[j], &rgb[i]);
    }
    return true;
}

void ImageProcessorWorker::yuyv_to_rgb_pixel(const uchar *yuyv, uchar *rgb)
{
    int y, v, u;
    float r, g, b;

    y = yuyv[0]; //y0
    u = yuyv[1]; //u0
    v = yuyv[3]; //v0

    r = y + 1.4065 * (v - 128);			     //r0
    g = y - 0.3455 * (u - 128) - 0.7169 * (v - 128); //g0
    b = y + 1.1790 * (u - 128);			     //b0

    if (r < 0)
    r = 0;
    else if (r > 255)
    r = 255;
    if (g < 0)
    g = 0;
    else if (g > 255)
    g = 255;
    if (b < 0)
    b = 0;
    else if (b > 255)
    b = 255;

    rgb[0] = (unsigned char)r;
    rgb[1] = (unsigned char)g;
    rgb[2] = (unsigned char)b;

    //second pixel
    u = yuyv[1]; //u0
    y = yuyv[2]; //y1
    v = yuyv[3]; //v0

    r = y + 1.4065 * (v - 128);			     //r1
    g = y - 0.3455 * (u - 128) - 0.7169 * (v - 128); //g1
    b = y + 1.1790 * (u - 128);			     //b1

    if (r < 0)
    r = 0;
    else if (r > 255)
    r = 255;
    if (g < 0)
    g = 0;
    else if (g > 255)
    g = 255;
    if (b < 0)
    b = 0;
    else if (b > 255)
    b = 255;

    rgb[3] = (unsigned char)r;
    rgb[4] = (unsigned char)g;
    rgb[5] = (unsigned char)b;
}
