#ifndef ENDINGWINDOW_H
#define ENDINGWINDOW_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class EndingWindow;
}

class EndingWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EndingWindow(QWidget *parent = 0);
    ~EndingWindow();

private slots:
    void on_btnEnding_clicked();
    void go_to_firstWindow();
    void change_timeText();

private:
    Ui::EndingWindow *ui;
    QTimer *timer;
    QTimer *nextTimer;
};

#endif // ENDINGWINDOW_H
