#ifndef PHOTOWINDOW_H
#define PHOTOWINDOW_H

#include <QWidget>
#include <QDateTime>
#include <QDebug>
#include <QPixmap>
#include <QLabel>
#include "camerathread.h"
#include "udp_listener_thread.h"
#include "camerasoundplayer.h"

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


private slots:
    void change_timeText();
    void go_to_nextWindow();
    void handle_data(const uchar *data, int width, int height);
    void save_current_frame();


private:
    Ui::PhotoWindow *ui;
    QTimer *timer;
    QTimer *nextTimer;
    void rise_count();
    void toggle_grid(QLabel *lbl);

    void sendImageToServer(const QString &filePath);
    CameraThread *camera;
    QPixmap overlay_pixmap;
    UDPListenerThread *udp_listener;
    CameraSoundPlayer mCameraSoundPlayer;
    bool yuyv2rgb(const uchar *yuyv, int width, int height, uchar *rgb);
    void yuyv_to_rgb_pixel(const uchar *yuyv, uchar *rgb);
    uchar *image_buf;
    int front_index;
};

#endif // PHOTOWINDOW_H
