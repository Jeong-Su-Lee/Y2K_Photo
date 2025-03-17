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

    void on_btnNone_clicked();

    void on_btnHeart_clicked();

    void on_btnStar_clicked();

    void on_btnLG_clicked();

private:
    Ui::GuideSelectWindow *ui;
    QTimer *timer;
};

#endif // GUIDESELECTWINDOW_H
