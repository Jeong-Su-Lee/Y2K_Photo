#ifndef GUIDESELECTWINDOW_H
#define GUIDESELECTWINDOW_H

#include <QWidget>
#include <QTimer>

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
    void go_to_nextWindow();


private:
    Ui::GuideSelectWindow *ui;
    QTimer *timer;
};

#endif // GUIDESELECTWINDOW_H
