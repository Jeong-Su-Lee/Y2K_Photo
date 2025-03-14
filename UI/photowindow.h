#ifndef PHOTOWINDOW_H
#define PHOTOWINDOW_H

#include <QWidget>

#define TIME_LIMIT 10

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


private:
    Ui::PhotoWindow *ui;
    QTimer *timer;
    QTimer *nextTimer;
};

#endif // PHOTOWINDOW_H
