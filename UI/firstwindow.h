#ifndef FIRSTWINDOW_H
#define FIRSTWINDOW_H

#include <QWidget>

namespace Ui {
class FirstWindow;
}

class FirstWindow : public QWidget
{
    Q_OBJECT

public:
    explicit FirstWindow(QWidget *parent = 0);
    ~FirstWindow();

private slots:
    void on_btnFirst_clicked();

private:
    Ui::FirstWindow *ui;
};

#endif // FIRSTWINDOW_H
