#ifndef PHOTOWINDOW_H
#define PHOTOWINDOW_H

#include <QWidget>
#include <QDateTime>
#include <QDebug>
#include <QPixmap>
#include <QString>
#include <QLabel>
#include "camerathread.h"
#include "udp_listener_thread.h"
#include "udp_sender_thread.h"
#include "camerasoundplayer.h"
#include "imageprocessworker.h"
#include <QThread>

#define TIME_LIMIT 8

namespace Ui {
class PhotoWindow;
}

class PhotoWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PhotoWindow(QWidget *parent = 0);
    ~PhotoWindow();
    QString myclientId; // 전역 멤버로 선언
    QThread *imageThread;
    ImageProcessorWorker *imageWorker;


private slots:
    void change_timeText(int timeCount);
    void go_to_nextWindow();
    void handle_data(const uchar *data, int width, int height);
    void save_current_frame();
    void displayReceivedImage(const QImage &image);


private:
    Ui::PhotoWindow *ui;
    QTimer *timer;
    QTimer *nextTimer;
    void rise_count();
    void toggle_grid(QLabel *lbl);

    void sendImageToServer(const QString &filePath);
    CameraThread *camera;
    SenderThread *senderThread;
    QPixmap overlay_pixmap;
    UDPListenerThread *udp_listener;
    CameraSoundPlayer mCameraSoundPlayer;
    bool yuyv2rgb(const uchar *yuyv, int width, int height, uchar *rgb);
    void yuyv_to_rgb_pixel(const uchar *yuyv, uchar *rgb);
    uchar *image_buf;
    int front_index;
    int number_of_guide =2;
    QString guidename;

private slots:
    void onClientIdReceived(const QString& id);
    void setGuideFromChar(QChar guideChar);
    void onFrameProcessed(const QPixmap &pixmap);
signals:
    void requestFrameProcessing(QByteArray frameData, int width, int height, QString guideName, QString clientId);
protected:
    void closeEvent(QCloseEvent *event) override;


};

#endif // PHOTOWINDOW_H
