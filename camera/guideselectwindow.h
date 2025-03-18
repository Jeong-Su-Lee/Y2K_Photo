#ifndef GUIDESELECTWINDOW_H
#define GUIDESELECTWINDOW_H

#include <QWidget>
#include <QTimer>
#include"udp_listener_thread.h"

namespace Ui {
class GuideSelectWindow;
}

class GuideSelectWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GuideSelectWindow(QWidget *parent = 0);
    ~GuideSelectWindow();


private slots:
    void on_btnNone_clicked();

    void on_btnHeart_clicked();

    void on_btnStar_clicked();

    void on_btnLG_clicked();

    void go_to_nextWindow();

private:
    UDPListenerThread *udp_guide;
    Ui::GuideSelectWindow *ui;
    QTimer *timer;
};

#endif // GUIDESELECTWINDOW_H
