#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QDebug>
#include <QPixmap>
#include <QCloseEvent>

#include "camerathread.h"
#include "udp_listener_thread.h"
#include "camerasoundplayer.h"
#include "udp_sender_thread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString myclientId; // 전역 멤버로 선언


public slots:
    void handle_data(const uchar *data, int width, int height);
    void save_current_frame();

private:
    void sendImageToServer(const QString &filePath);
    Ui::MainWindow *ui;
    CameraThread *camera;
    SenderThread *senderThread;
    QPixmap overlay_pixmap;
    UDPListenerThread *udp_listener;
    CameraSoundPlayer mCameraSoundPlayer;
    bool yuyv2rgb(const uchar *yuyv, int width, int height, uchar *rgb);
    void yuyv_to_rgb_pixel(const uchar *yuyv, uchar *rgb);
    uchar *image_buf;
    int front_index;

private slots:
    void onClientIdReceived(const QString& id);
protected:
    void closeEvent(QCloseEvent *event) override;

};

#endif // MAINWINDOW_H
