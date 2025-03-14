#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class SecondWindow;
}

class SecondWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SecondWindow(QWidget *parent = 0);
    ~SecondWindow();

private slots:
    void slotTimerAlarm();
    void changeWindow();

private:
    Ui::SecondWindow *ui;
    QTimer *timer;
    QTimer *nextTimer;
};

#endif // SECONDWINDOW_H
